#pragma once
#include <stdint.h>
#include <stddef.h>

const char* to_string(uint64_t value);
const char* to_string(int64_t value);

const char* to_hstring(uint64_t value);
const char* to_hstring(uint32_t value);
const char* to_hstring(uint16_t value);
const char* to_hstring(uint8_t value);