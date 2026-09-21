#include "font.hpp"

extern "C" const uint8_t _binary_hankaku_bin_start[];
extern "C" const uint8_t _binary_hankaku_bin_size;

const uint8_t* GetFont(char c) {
    auto u_c = static_cast<uint8_t>(c);
    auto index = 16 * static_cast<unsigned int>(u_c);

    auto size = reinterpret_cast<uintptr_t>(&_binary_hankaku_bin_size);
    if (index >= size) {
        return nullptr;
    }

    return _binary_hankaku_bin_start + index;
}