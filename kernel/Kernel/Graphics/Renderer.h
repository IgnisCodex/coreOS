#pragma once
#include <stdint.h>

#include "Framebuffer.h"
#include "Maths.h"
#include "Font.h"

class BasicRenderer {
    public:
    BasicRenderer(Framebuffer* targetFramebuffer, PSF1_FONT* psf1_Font);
    Position CursorPosition;
    Framebuffer* TargetFramebuffer;
    PSF1_FONT* PSF1_Font;
    unsigned int Color;

    void PutChar(char chr, unsigned int XOffset, unsigned int YOffset);
    void Print(const char *str);
};