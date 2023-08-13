#pragma once
#include <stdint.h>

#include "Framebuffer.h"
#include "Maths.h"
#include "Font.h"
#include "Colours.h"

class BasicRenderer {
    public:
    BasicRenderer(Framebuffer* targetFramebuffer, PSF1_FONT* psf1_Font);
    Position CursorPosition;
    Framebuffer* TargetFramebuffer;
    PSF1_FONT* PSF1_Font;
    unsigned int Color;
    unsigned int BoldColor;
    unsigned int Spacing;
    bool AutoReturn;
    bool Bold;

    void PutChar(char chr, unsigned int XOffset, unsigned int YOffset);
    void PutPix(uint32_t X, uint32_t Y, uint32_t Color);
    void Print(const char *str);
    void Print(const char *str, bool Bold, bool Underline);
    void Scroll(uint32_t ScrollHeight);


    void Return();
};