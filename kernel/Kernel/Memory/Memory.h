#pragma once

#include "../Graphics/Renderer.h"
#include "EFIMemory.h"


struct BootInfo {
    Framebuffer* framebuffer;
    PSF1_FONT* psf1_Font;
};


uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize);

void PrintEFIMemData(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize, BasicRenderer* temp );

void _memset(void* start, uint8_t value, uint64_t num);

void _memcpy(void* src, void* dest, uint64_t size);

int _memcmp(void* src, void* dest, int amt);