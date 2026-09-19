#pragma once

#include <stdint.h>

// 色を表す構造体
struct PixelColor {
    uint8_t r, g, b;
};

// 画面情報の構造体
struct FrameBufferConfig {
    uint8_t* frame_buffer;
    uint32_t pixels_per_scan_line;
    uint32_t horizontal_resolution;
    uint32_t vertical_resolution;
    enum PixelFormat {
        kPixelRGBResv8BitPerColor,
        kPixelBGRResv8BitPerColor,
    } pixel_format;
};

// --- 基底クラス（設計図の親） ---
class PixelWriter {
public:
    PixelWriter(const FrameBufferConfig& config) : config_{config} {}
    virtual ~PixelWriter() = default;

    // 純粋仮想関数：子クラスで必ず具体的な書き込みロジックを実装させる
    virtual void Write(int x, int y, const PixelColor& c) = 0;

    void FillRectangle(int x, int y, int w, int h, const PixelColor& c);

protected:
    // (x, y) 座標から VRAM のメモリ先頭アドレスを計算する補助関数
    uint8_t* PixelAt(int x, int y);

private:
    FrameBufferConfig config_;
};

// --- 派生クラス：RGB用 ---
class RGBPixelWriter : public PixelWriter {
public:
    using PixelWriter::PixelWriter; // 親クラスのコンストラクタを継承
    virtual void Write(int x, int y, const PixelColor& c) override;
};

// --- 派生クラス：BGR用 ---
class BGRPixelWriter : public PixelWriter {
public:
    using PixelWriter::PixelWriter; // 親クラスのコンストラクタを継承
    virtual void Write(int x, int y, const PixelColor& c) override;
};