#include "Kernel.h"
#include "Graphics/Renderer.h"
#include "Graphics/XToString.h"

extern "C" void main(Framebuffer* framebuffer, PSF1_FONT* psf1_font, void* freeMemStart, void* extraMemStart, uint64_t freeMemSize, void* kernelStart, uint64_t kernelSize, void* kernelStartV) {

    
    BasicRenderer newRenderer = BasicRenderer(framebuffer, psf1_font);
    // newRenderer.Print("coreOS Kernel Loaded...");

    // newRenderer.CursorPosition = {0, newRenderer.CursorPosition.Y + 16};

    newRenderer.CursorPosition.X = 0;
    newRenderer.CursorPosition.Y += 20;
    newRenderer.Print(to_hstring((uint64_t)0xf0));

    
    // FIXME: For some reason doubles do not work... cause OS to crash
    newRenderer.Print(to_string((double)13.14, 2));
    


    return;

}