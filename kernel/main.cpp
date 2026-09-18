using uint8_t = unsigned char;
using uint32_t = unsigned int;
using uint64_t = unsigned long long;

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

extern "C" void KernelMain(const FrameBufferConfig& frame_buffer_config) {
    for (uint32_t y = 0; y < frame_buffer_config.vertical_resolution; ++y) {
        for (uint32_t x = 0; x < frame_buffer_config.horizontal_resolution; ++x) {
            uint64_t pixel_index = y * frame_buffer_config.pixels_per_scan_line + x;
            uint8_t* p = &frame_buffer_config.frame_buffer[pixel_index * 4];
            p[0] = 255; // Red
            p[1] = 255; // Green
            p[2] = 255; // Blue
        }
    }
    while (1) __asm__("hlt");
}