#pragma once

#include <stdint.h>
#include <stddef.h>
#include "Graphics/Framebuffer.h"
#include "Graphics/Font.h"
#include "Graphics/Maths.h"

void main(Framebuffer* framebuffer, PSF1_FONT* psf1_font, void* freeMemStart, void* extraMemStart, uint64_t freeMemSize, void* kernelStart, uint64_t kernelSize, void* kernelStartV);