#pragma once
#include <stddef.h>

struct Framebuffer {
    void *BaseAddress;
    size_t BufferSize;
    unsigned int Width;
    unsigned int Height;
    unsigned int Pitch;
    unsigned int PixelsPerScanLine;
};