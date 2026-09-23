#pragma once
#include "pixel_writer.hpp"

class Console {
    public:
        static const int kCols = 80;
        static const int kRows = 25;

        Console(PixelWriter* writer, int x, int y, int width, int height, const PixelColor& fg_color, const PixelColor& bg_color);
        
        void WriteString(const char* str);

        void PutChar(char c);

        void Newline();

    private:
        PixelWriter* writer_;
        int x_, y_, width_, height_;
        PixelColor fg_color_, bg_color_;
        int cursor_row_{0}, cursor_col_{0};
        char buffer_[kRows][kCols + 1]; // +1 for null terminator
};

