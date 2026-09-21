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

void PixelWriter::DrawLine(int x0, int y0, int x1, int y1, const PixelColor& color) {
    int dx = (x1 > x0) ? x1 - x0 : x0 - x1;
    int dy = (y1 > y0) ? y1 - y0 : y0 - y1;

    int sx = (x1 > x0) ? 1 : -1;
    int sy = (y1 > y0) ? 1 : -1;

    // 始点と終点が同じ点なら1ピクセル打って終了
    if (dx == 0 && dy == 0) {
        Write(x0, y0, color);
        return;
    }

    if (dx >= dy) {
        // 横長（x軸メインで進む）パターン
        int inc_y = 0;
        for (int i = 0; i <= dx; ++i) {
            Write(x0, y0, color);
            x0 += sx;
            inc_y += dy;
            if (inc_y >= dx) {
                inc_y -= dx;
                y0 += sy;
            }
        }
    } else {
        // 縦長（y軸メインで進む）パターン
        int inc_x = 0;
        for (int i = 0; i <= dy; ++i) {
            Write(x0, y0, color);
            y0 += sy;
            inc_x += dx;
            if (inc_x >= dy) {
                inc_x -= dy;
                x0 += sx;
            }
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