#include "console.hpp"

Console::Console(PixelWriter* writer, int x, int y, int width, int height, const PixelColor& fg_color, const PixelColor& bg_color)
    : writer_(writer), x_(x), y_(y), width_(width), height_(height), fg_color_(fg_color), bg_color_(bg_color) {
    // Initialize the buffer with null terminators
    for (int row = 0; row < kRows; ++row) {
        for (int col = 0; col < kCols; ++col) {
            buffer_[row][col] = '\0';
        }
    }

    writer_->FillRectangle(x_, y_, width_, height_, bg_color_);
}

void Console::WriteString(const char* str) {
    while (*str) {
        char c = *str++;
        if (c == '\n') {
            cursor_row_++;
            cursor_col_ = 0;
            if (cursor_row_ >= kRows) {
                // Scroll up
                for (int row = 1; row < kRows; ++row) {
                    for (int col = 0; col < kCols; ++col) {
                        buffer_[row - 1][col] = buffer_[row][col];
                    }
                }
                // Clear the last row
                for (int col = 0; col < kCols; ++col) {
                    buffer_[kRows - 1][col] = '\0';
                }
                cursor_row_ = kRows - 1;
            }
        } else {
            if (cursor_col_ < kCols) {
                buffer_[cursor_row_][cursor_col_] = c;
                writer_->WriteAscii(x_ + cursor_col_ * 8, y_ + cursor_row_ * 16, c, fg_color_);
                cursor_col_++;
            } else {
                // If the line is full, move to the next line
                cursor_row_++;
                cursor_col_ = 0;
                if (cursor_row_ >= kRows) {
                    // Scroll up
                    for (int row = 1; row < kRows; ++row) {
                        for (int col = 0; col < kCols; ++col) {
                            buffer_[row - 1][col] = buffer_[row][col];
                        }
                    }
                    // Clear the last row
                    for (int col = 0; col < kCols; ++col) {
                        buffer_[kRows - 1][col] = '\0';
                    }
                    cursor_row_ = kRows - 1;
                }
                // Write the character on the new line
                buffer_[cursor_row_][cursor_col_] = c;
                writer_->WriteAscii(x_ + cursor_col_ * 8, y_ + cursor_row_ * 16, c, fg_color_);
                cursor_col_++;
            }
        }
    }
}