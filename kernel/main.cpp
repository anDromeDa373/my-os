#include "pixel_writer.hpp"
#include <stddef.h>

void* operator new(size_t size, void* buf) noexcept {
    return buf;
}

extern "C" void __cxa_pure_virtual() {
    while (1) __asm__("hlt");
}

const PixelColor kWhite{255, 255, 255};
const PixelColor kRed{255, 0, 0};

// ★ alignas でメモリのアドレス境界を 8バイトに整列させる
alignas(RGBPixelWriter) char pixel_writer_buf[sizeof(RGBPixelWriter)];
PixelWriter* writer;

extern "C" void KernelMain(const FrameBufferConfig& frame_buffer_config) {
    if (frame_buffer_config.pixel_format == FrameBufferConfig::kPixelRGBResv8BitPerColor) {
        writer = new (pixel_writer_buf) RGBPixelWriter{frame_buffer_config};
    } else {
        writer = new (pixel_writer_buf) BGRPixelWriter{frame_buffer_config};
    }

    // ★ y を外側、x を内側にすることで、メモリに順番に高速アクセスして画面全体を白塗りにする
    for (uint32_t y = 0; y < frame_buffer_config.vertical_resolution; ++y) {
        for (uint32_t x = 0; x < frame_buffer_config.horizontal_resolution; ++x) {
            writer->Write(x, y, kWhite);
        }
    }

    // 赤い四角形を描画
    writer->FillRectangle(200, 200, 100, 100, kRed);

    while (1) __asm__("hlt");
}