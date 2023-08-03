#include "Kernel.h"
#include "Graphics/Renderer.h"
#include "Graphics/CStr.h"
#include "Memory/Memory.h"

extern "C" void main(BootInfo* bootInfo) {

    
    BasicRenderer newRenderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_Font);


    // Terminal Things
    newRenderer.Spacing = 2;
    newRenderer.AutoReturn = true;

    newRenderer.Print("coreOS Boot Initiated...");
    newRenderer.Return();
    newRenderer.Print("Boot Info", true, true);

    newRenderer.Color = TRM_WHITE;
    newRenderer.BoldColor = TRM_WHITE;
    newRenderer.AutoReturn = false;

    newRenderer.Print("Framebuffer", true, false);
    newRenderer.Return();

    newRenderer.Print("Base Address: ");
    newRenderer.Print(to_hstring((uint64_t)newRenderer.TargetFramebuffer->BaseAddress));
    newRenderer.Return();

    newRenderer.Print("Width: ");
    newRenderer.Print(to_string((uint64_t)newRenderer.TargetFramebuffer->Width));
    newRenderer.Return();

    newRenderer.Print("Height: ");
    newRenderer.Print(to_string((uint64_t)newRenderer.TargetFramebuffer->Height));
    newRenderer.Return();

    newRenderer.Print("Buffer Size: ");
    newRenderer.Print(to_string((uint64_t)newRenderer.TargetFramebuffer->BufferSize));
    newRenderer.Return();

    newRenderer.Print("Pixels Per Scan Line: ");
    newRenderer.Print(to_string((uint64_t)newRenderer.TargetFramebuffer->PixelsPerScanLine));
    newRenderer.Return();

    // Frame buffer test


    return;
}