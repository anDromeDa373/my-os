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

    int step_x = (dx > dy) ? dx : dy;
    int step_y = (dx > dy) ? dy : dx;

    int sx = (x1 > x0) ? 1 : -1;
    int sy = (y1 > y0) ? 1 : -1;

    int inc_x = 0;
    int inc_y = 0;

    if (step_x == 0 || step_y == 0) {
        Write(x0, y0, color);
        return;
    }

    for (int i = 0; i <= step_x; ++i) {
        Write(x0, y0, color);
        x0 += sx;
        inc_y += step_y;
        if (inc_y >= step_x) {
            inc_y -= step_x;
            y0 += sy;
        }
    }

    for (int i = 0; i <= step_y; ++i) {
        Write(x0, y0, color);
        y0 += sy;
        inc_x += step_x;
        if (inc_x >= step_y) {
            inc_x -= step_y;
            x0 += sx;
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