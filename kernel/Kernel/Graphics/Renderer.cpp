
#include "Renderer.h"

BasicRenderer::BasicRenderer(Framebuffer* targetFramebuffer, PSF1_FONT* psf1_Font) {
    TargetFramebuffer = targetFramebuffer;
    PSF1_Font = psf1_Font;
    CursorPosition = {0, 0};
    Color = 0xffffffff;
}

void BasicRenderer::PutChar(char chr, unsigned int XOffset, unsigned int YOffset) {
    unsigned int* PixelPtr = (unsigned int*)TargetFramebuffer->BaseAddress;
    char* FontPtr = (char*)PSF1_Font->glyphBuffer + (chr * PSF1_Font->psf1_Header->charsize);
    for (unsigned long Y = YOffset; Y < YOffset + 16; Y++){
        for (unsigned long X = XOffset; X < XOffset+8; X++){
            if ((*FontPtr & (0b10000000 >> (X - XOffset))) > 0){
                    *(unsigned int*)(PixelPtr + X + (Y * TargetFramebuffer->PixelsPerScanLine)) = Color;
                }

        }
        FontPtr++;
    }
}

void BasicRenderer::Print(const char* String)
{
    
    char* Char = (char*)String;
    while(*Char != 0){
        PutChar(*Char, CursorPosition.X, CursorPosition.Y);
        CursorPosition.X+=8;
        if(CursorPosition.X + 8 > TargetFramebuffer->Width)
        {
            CursorPosition.X = 0;
            CursorPosition.Y += 16;
        }
        Char++;
    }
}

