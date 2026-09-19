#include "pixel_writer.hpp"

// (x, y) 座標のメモリ位置を計算（1ピクセル=4バイト）
uint8_t* PixelWriter::PixelAt(int x, int y) {
    return config_.frame_buffer + 4 * (config_.pixels_per_scan_line * y + x);
}

// 指定した矩形領域を指定色で塗りつぶす
void PixelWriter::FillRectangle(int x, int y, int w, int h, const PixelColor& c) {
    for (int dx = 0; dx < w; ++dx) {
        for (int dy = 0; dy < h; ++dy) {
            Write(x + dx, y + dy, c);
        }
    }
}

// RGB形式の書き込み： [0]=R, [1]=G, [2]=B
void RGBPixelWriter::Write(int x, int y, const PixelColor& c) {
    auto p = PixelAt(x, y);
    p[0] = c.r;
    p[1] = c.g;
    p[2] = c.b;
}

// BGR形式の書き込み： [0]=B, [1]=G, [2]=R
void BGRPixelWriter::Write(int x, int y, const PixelColor& c) {
    auto p = PixelAt(x, y);
    p[0] = c.b;
    p[1] = c.g;
    p[2] = c.r;
}