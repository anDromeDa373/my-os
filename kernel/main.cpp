#include "pixel_writer.hpp"
#include <stddef.h>
#include <stdint.h>

void operator delete(void* p) noexcept {}
void operator delete(void* p, size_t size) noexcept {}

void* operator new(size_t size, void* buf) noexcept {
    return buf;
}

extern "C" void __cxa_pure_virtual() {
    while (1) __asm__("hlt");
}

// デバッグ用の色四角を描画する関数
void DrawDebugSquare(const FrameBufferConfig& config, int x_offset, uint8_t r, uint8_t g, uint8_t b) {
    for (int dy = 0; dy < 30; ++dy) {
        for (int dx = 0; dx < 30; ++dx) {
            uint8_t* p = config.frame_buffer + 4 * (config.pixels_per_scan_line * (20 + dy) + (x_offset + dx));
            p[0] = r;
            p[1] = g;
            p[2] = b;
        }
    }
}

// ELF64 Dynamic セクションの構造体
struct Elf64_Dyn {
    int64_t d_tag;
    union {
        uint64_t d_val;
        uint64_t d_ptr;
    } d_un;
};

// ELF64 Relocation (Rela) 構造体
struct Elf64_Rela {
    uint64_t r_offset;
    uint64_t r_info;
    int64_t  r_addend;
};

// 【重要】
// hidden 属性を付与することで GOT を経由せず %rip 相対参照で直接アドレスを取得する。
// これにより GOT 再配置前の未補正状態でも安全に _DYNAMIC を参照できる。
extern "C" __attribute__((visibility("hidden"))) const Elf64_Dyn _DYNAMIC[];

constexpr int64_t DT_RELA   = 7;
constexpr int64_t DT_RELASZ = 8;

void PerformSelfRelocation(uint64_t base_addr, const FrameBufferConfig& config) {
    const Elf64_Rela* rela_start = nullptr;
    uint64_t rela_size = 0;

    // --- チェック 1: _DYNAMIC が読めているか ---
    // もし _DYNAMIC[0].d_tag が 0 以外（何らかのデータ）なら「白い四角」を描画
    if (_DYNAMIC[0].d_tag != 0) {
        DrawDebugSquare(config, 60, 255, 0, 0); // 白 (位置: 60)
    }

    for (const auto* dyn = _DYNAMIC; dyn->d_tag != 0; ++dyn) {
        if (dyn->d_tag == DT_RELA) {
            DrawDebugSquare(config, 400, 255, 0, 0);
            rela_start = reinterpret_cast<const Elf64_Rela*>(base_addr + dyn->d_un.d_ptr);
        } else if (dyn->d_tag == DT_RELASZ) {
            DrawDebugSquare(config, 430, 255, 255, 0);
            rela_size = dyn->d_un.d_val;
        }
    }

    // --- チェック 2: DT_RELA / DT_RELASZ が見つかったか ---
    if (base_addr == 0x100000) {
        DrawDebugSquare(config, 90, 0, 255, 0); // オレンジ (位置: 75)
    }

    if (rela_size > 0) {
        DrawDebugSquare(config, 120, 0, 0, 255); // ピンク (位置: 90)
    }

    // --- 本来の書き換え処理 ---
    int count = 0;
    if (rela_start && rela_size > 0) {
        size_t rela_count = rela_size / sizeof(Elf64_Rela);
        for (size_t i = 0; i < rela_count; ++i) {
            const auto& rela = rela_start[i];
            if (rela.r_info != 0) {
                DrawDebugSquare(config, 500, 0, 0, 255);
            }
            if ((rela.r_info & 0xffffffff) == 8) { // R_X86_64_RELATIVE
                auto* addr = reinterpret_cast<uint64_t*>(base_addr + rela.r_offset);
                *addr = base_addr + rela.r_addend;
                count++;
            }
        }
    }

    // --- チェック 3: 書き換えルーチンが 1 回以上実行されたか ---
    if (count > 0) {
        DrawDebugSquare(config, 150, 255, 0, 255); // 紫 (位置: 110)
    }
}

const PixelColor kWhite{255, 255, 255};
const PixelColor kRed{255, 0, 0};
const PixelColor kGreen{0, 255, 0};

alignas(RGBPixelWriter) char pixel_writer_buf[sizeof(RGBPixelWriter)];
PixelWriter* writer;

extern "C" void KernelMain(const FrameBufferConfig& frame_buffer_config) {
    DrawDebugSquare(frame_buffer_config, 180, 255, 0, 0);       // 赤
    DrawDebugSquare(frame_buffer_config, 210, 255, 255, 0);     // 黄

    uint64_t base_addr = reinterpret_cast<uint64_t>(_DYNAMIC) & ~0xfffULL;
    while (base_addr > 0) {
        const uint8_t* p = reinterpret_cast<const uint8_t*>(base_addr);
        if (p[0] == 0x7f && p[1] == 'E' && p[2] == 'L' && p[3] == 'F') {
            break; // ELF ヘッダの先頭アドレスを自動発見！
        }
        base_addr -= 0x1000;
    }

    // 計算した本当の base_addr で再配置を実行
    PerformSelfRelocation(base_addr, frame_buffer_config);

    DrawDebugSquare(frame_buffer_config, 240, 0, 255, 0);       // 緑

    if (frame_buffer_config.pixel_format == FrameBufferConfig::kPixelRGBResv8BitPerColor) {
        writer = new (pixel_writer_buf) RGBPixelWriter{frame_buffer_config};
    } else {
        writer = new (pixel_writer_buf) BGRPixelWriter{frame_buffer_config};
    }
    DrawDebugSquare(frame_buffer_config, 270, 0, 255, 255);   // 水色

    writer->Write(0, 0, kWhite);
    DrawDebugSquare(frame_buffer_config, 300, 0, 0, 0);     // 青

    for (uint32_t y = 0; y < frame_buffer_config.vertical_resolution; ++y) {
        for (uint32_t x = 0; x < frame_buffer_config.horizontal_resolution; ++x) {
            writer->Write(x, y, kWhite);
        }
    }

    writer->FillRectangle(200, 200, 100, 100, kRed);

    writer->DrawLine(0, 0, 200, 100, kRed);

    writer->DrawLine(100, 100, 200, 300, kRed);

    writer->DrawLine(600, 600, 400, 500, kRed);

    writer->WriteAscii(100, 100, 'Z', kRed);

    writer->WriteAscii(100, 200, 1, {255, 255, 255});

    writer->WriteAscii(200, 100, 0xB1, {0, 0, 0});

    writer->FillRectangle(200, 500, 8, 8, kRed);

    writer->WriteString(300, 300, "Hello, World!", {0, 0, 0});



    while (1) __asm__("hlt");
}