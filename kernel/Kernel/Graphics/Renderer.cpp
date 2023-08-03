#include "Renderer.h"

BasicRenderer::BasicRenderer(Framebuffer* targetFramebuffer, PSF1_FONT* psf1_Font) {
    TargetFramebuffer = targetFramebuffer;
    PSF1_Font = psf1_Font;
    CursorPosition = {0, 0};
    Color = TRM_SET_DEFAULT;
    BoldColor = TRM_SET_BOLD;
    Bold = false;
    Spacing = 0;
    AutoReturn = false;
}

void BasicRenderer::Return() {
    CursorPosition = {0, CursorPosition.Y += (16 + Spacing)};
}

void BasicRenderer::PutPix(uint32_t X, uint32_t Y, uint32_t Color){
    *(uint32_t*)((uint64_t)TargetFramebuffer->BaseAddress + (X*4) + (Y * TargetFramebuffer->PixelsPerScanLine * 4)) = Color;
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

void BasicRenderer::Print(const char* String) {

    int OldColor = Color;

    if (Bold == true) {
        Color = BoldColor;
    }

    unsigned int CurrentPosX = CursorPosition.X;
    unsigned int CurrentPosY = CursorPosition.Y;
    
    char* Char = (char*)String;
    char* Char2 = (char*)String;

    while(*Char != 0){
        PutChar(*Char, CursorPosition.X, CursorPosition.Y);
        CursorPosition.X+=8;
        if(CursorPosition.X + 8 > TargetFramebuffer->Width)
        {
            CursorPosition.X = 0;
            CursorPosition.Y += (16 + Spacing);
        }
        Char++;
    }


    if (Bold == true) {

        CursorPosition.X = CurrentPosX + 1;

        while(*Char2 != 0){
    
            PutChar(*Char2, CursorPosition.X, CursorPosition.Y);
            CursorPosition.X+=8;
            if(CursorPosition.X + 8 > TargetFramebuffer->Width)
            {
                CursorPosition.X = 0;
                CursorPosition.Y += (16 + Spacing);
            }
            Char2++;
        }
    }





    if (AutoReturn == true) {
        Return();
    }


    Color = OldColor;    
}

void BasicRenderer::Print(const char* String, bool SetBold, bool Underline) {

    unsigned int CurrentPosX = CursorPosition.X;
    unsigned int CurrentPosY = CursorPosition.Y;
    int OldColor = Color;


    if (SetBold == true) {
        Bold = true;
    } else {
        Bold = false;
    }

    if (Bold == true) {
        Color = BoldColor;
    }

    if (Underline == true) {
        CursorPosition.Y += 14;
        char* Char = (char*)String;
        while(*Char != 0){
            for (int i = 0; i < 8; i++) {
                PutPix(CursorPosition.X, CursorPosition.Y, Color);
                CursorPosition.X++;
            }
            
            Char++;
        }

        if (Bold == true) {
            PutPix(CursorPosition.X, CursorPosition.Y, Color);
            CursorPosition.X++;
        }

    }

    CursorPosition.X = CurrentPosX;
    CursorPosition.Y = CurrentPosY;

    Print(String);

    if (SetBold == true) {
        Bold = false;
    }

    Color = OldColor;
}