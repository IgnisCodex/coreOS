#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "limine.h"
#include "Kernel/Kernel.h"
#include "Kernel/Graphics/Framebuffer.h"
#include "Kernel/Graphics/Font.h"

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent.

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

static volatile struct limine_module_request module_request = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 0
};

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

static volatile struct limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0, .response = NULL
};


// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .c file.

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

// Halt and catch fire function.
static void hcf(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}


bool checkStringEndsWith(const char *str, const char *end)
{
    const char *_str = str;
    const char *_end = end;

    while (*str != 0)
        str++;
    str--;

    while (*end != 0)
        end++;
    end--;

    while (true)
    {
        if (*str != *end)
            return false;

        str--;
        end--;

        if (end == _end || (str == _str && end == _end))
            return true;

        if (str == _str)
            return false;
    }

    return true;
}

static const char *get_memmap_type(uint64_t type) {
    switch (type) {
        case LIMINE_MEMMAP_USABLE:
            return "Usable";
        case LIMINE_MEMMAP_RESERVED:
            return "Reserved";
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
            return "ACPI reclaimable";
        case LIMINE_MEMMAP_ACPI_NVS:
            return "ACPI NVS";
        case LIMINE_MEMMAP_BAD_MEMORY:
            return "Bad memory";
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
            return "Bootloader reclaimable";
        case LIMINE_MEMMAP_KERNEL_AND_MODULES:
            return "Kernel and modules";
        case LIMINE_MEMMAP_FRAMEBUFFER:
            return "Framebuffer";
        default:
            return "???";
    }
}


void* startRAMAddr = NULL;
void* quickMalloc(uint64_t size)
{
    if (startRAMAddr == NULL){
        hcf();
    }
    void* temp = startRAMAddr;
    startRAMAddr = (void*)((uint64_t)startRAMAddr + size);
    return temp;
}




struct limine_file *getFile(const char *name)
{
    struct limine_module_response *module_response = module_request.response;
    for (size_t i = 0; i < module_response->module_count; i++)
    {
        struct limine_file *f = module_response->modules[i];
        if (checkStringEndsWith(f->path, name))
            return f;
    }
    return NULL;
}







// The following will be our kernel's entry point.
// If renaming _start() to something else, make sure to change the
// linker script accordingly.
extern "C" void _start(void) {
    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    Framebuffer fb;
    Framebuffer* framebuffer = &fb;
    {
        limine_framebuffer* lFb = framebuffer_request.response->framebuffers[0];
        framebuffer->BaseAddress = lFb->address;
        framebuffer->Width = lFb->width;
        framebuffer->Height = lFb->height;
        framebuffer->PixelsPerScanLine = lFb->pitch / 4;
        framebuffer->BufferSize = lFb->height * lFb->pitch;//lFb->edid_size;
    }


    /*
    ======
    Memmap
    ======
    */


    if (memmap_request.response == NULL) {
        hcf();
    }

    void* freeMemStart = NULL;
    uint64_t freeMemSize = 0;


    if (kernel_address_request.response == NULL) {
        hcf();
    }

    struct limine_kernel_address_response *kernel_address_response = kernel_address_request.response;
    void* kernelStart = (void*)kernel_address_response->physical_base;
    void* kernelStartV = (void*)kernel_address_response->virtual_base;

    uint64_t kernelSize = 1;


    struct limine_memmap_response *memmap_response = memmap_request.response;

    for (size_t i = 0; i < memmap_response->entry_count; i++)
    {
        struct limine_memmap_entry *entry = memmap_response->entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE) {
            if (entry->length > freeMemSize) {
                freeMemStart = (void*)entry->base;
                freeMemSize = entry->length;
            }
        }
        
        else if (entry->base == (uint64_t)kernelStart) {
            kernelSize = entry->length;
        }
    }

    if (freeMemStart == NULL) {
        hcf();
    }

    startRAMAddr = freeMemStart;



    if (module_request.response == NULL) {
        hcf();
    }




    PSF1_FONT font;
    {
        const char* filename = "zap-light16.psf";
        struct limine_file* file = getFile(filename);

        if (file == NULL) {
            hcf();
        }

        font.psf1_Header = (struct PSF1_HEADER *)file->address;

        if (font.psf1_Header->magic[0] != 0x36 || font.psf1_Header->magic[1] != 0x04) {
            hcf();
        }

        font.glyphBuffer = (void *)((uint64_t)file->address + sizeof(PSF1_HEADER));
    }

    main(framebuffer, &font, startRAMAddr, freeMemStart, freeMemSize, kernelStart, kernelSize, kernelStartV);

    // We're done, just hang...
    hcf();
}
