#include "console.hpp"

Console::Console(PixelWriter* writer, int x, int y, int width, int height,
                 const PixelColor& fg_color, const PixelColor& bg_color)
    : writer_(writer),
      x_(x),
      y_(y),
      width_(width),
      height_(height),
      fg_color_(fg_color),
      bg_color_(bg_color) {
  for (int row = 0; row < kRows; ++row) {
    for (int col = 0; col < kCols; ++col) {
      buffer_[row][col] = '\0';
    }
  }
  writer_->FillRectangle(x_, y_, width_, height_, bg_color_);
}

void Console::PutChar(char c) {
  if (c == '\n') {
    Newline();
  } else {
    if (cursor_col_ >= kCols) {
      Newline();
    }
    buffer_[cursor_row_][cursor_col_] = c;
    writer_->WriteAscii(x_ + cursor_col_ * 8, y_ + cursor_row_ * 16, c, fg_color_);
    cursor_col_++;
  }
}

void Console::WriteString(const char* str) {
  while (*str) {
    PutChar(*str++);
  }
}

void Console::Newline() {
  cursor_col_ = 0;
  if (cursor_row_ < kRows - 1) {
    cursor_row_++;
  } else {
    // 1. バッファを 1 行上に移動
    for (int row = 1; row < kRows; ++row) {
      for (int col = 0; col < kCols; ++col) {
        buffer_[row - 1][col] = buffer_[row][col];
      }
    }
    for (int col = 0; col < kCols; ++col) {
      buffer_[kRows - 1][col] = '\0';
    }

    // 2. 画面全体を背景色でクリア
    writer_->FillRectangle(x_, y_, width_, height_, bg_color_);

    // 3. 文字が存在する場所だけを描画（\0 の場所は何もしない！）
    for (int row = 0; row < kRows; ++row) {
      for (int col = 0; col < kCols; ++col) {
        char ch = buffer_[row][col];
        if (ch != '\0') {
          writer_->WriteAscii(x_ + col * 8, y_ + row * 16, ch, fg_color_);
        }
      }
    }
  }
}