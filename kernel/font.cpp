#include "font.hpp"

extern "C" const uint8_t _binary_hankaku_bin_start[];
extern "C" const uint8_t _binary_hankaku_bin_size;

// ポインタ変数として定義する！
// これにより、この g_font_ptr 自体が .rela.dyn に登録され、
// PerformSelfRelocation(base_addr) によって起動時に自動で base_addr が足されます。
const uint8_t* volatile g_font_ptr = _binary_hankaku_bin_start;

const uint8_t* GetFont(char c) {
    // signed char から uint8_t にキャストして 0〜255 の正数にする
    auto u_c = static_cast<uint8_t>(c);
    auto index = 16 * static_cast<unsigned int>(u_c);

    // 2. サイズチェック（size シンボルを使う場合）
    auto size = reinterpret_cast<uintptr_t>(&_binary_hankaku_bin_size);
    if (index >= size) {
        return nullptr;
    }

    // 3. & を外して先頭ポインタからインデックス分進める
    return _binary_hankaku_bin_start + index;
}