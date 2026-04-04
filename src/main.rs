#![no_std]
#![no_main]

use core::panic::PanicInfo;

// 過去の C コードにあった 64バイトのオフセットを構造体で定義
#[repr(C)]
struct SimpleTextOutputProtocol {
    _unused: usize,
    // MS ABI (win64) で呼び出す関数ポインタ
    output_string: extern "win64" fn(*const SimpleTextOutputProtocol, *const u16) -> usize,
}

#[repr(C)]
struct SystemTable {
    _padding: [u8; 64], // Cコードの char _padding[64] と同等
    con_out: *const SimpleTextOutputProtocol,
}

#[no_mangle]
pub extern "win64" fn efi_main(_image_handle: *const core::ffi::c_void, system_table: *const SystemTable) -> usize {
    unsafe {
        // ポインタを経由して ConOut (SimpleTextOutputProtocol) を取得
        let st = &*system_table;
        let con_out = &*st.con_out;

        // L"..." 相当の 16bit 文字列（ヌル終端が必要）
        let msg = [
            'H' as u16, 'e' as u16, 'l' as u16, 'l' as u16, 'o' as u16, ' ' as u16,
            'f' as u16, 'r' as u16, 'o' as u16, 'm' as u16, ' ' as u16,
            'R' as u16, 'u' as u16, 's' as u16, 't' as u16, ' ' as u16,
            'P' as u16, 'o' as u16, 'i' as u16, 'n' as u16, 't' as u16, 'e' as u16, 'r' as u16, '!' as u16,
            '\r' as u16, '\n' as u16, 0u16,
        ];

        // 関数ポインタを直接叩く
        (con_out.output_string)(st.con_out, msg.as_ptr());
    }

    loop {}
}

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}