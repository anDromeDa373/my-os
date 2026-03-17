// UEFIから呼ばれるエントリーポイント。ここもMS ABIで受け取る必要がある。
void __attribute__((ms_abi)) efi_main(void* image_handle, void* system_table) {
    struct {
        char _padding[64];
        struct {
            void* _unused;
            // output_stringを呼ぶ際、RDI/RSIではなくRCX/RDXレジスタを使うよう明示
            unsigned long long (__attribute__((ms_abi)) *output_string)(void*, unsigned short*);
        } *con_out;
    } *st = system_table;

    // L"..." (16bitワイド文字) で文字列を渡す
    st->con_out->output_string(st->con_out, (unsigned short*)L"Hello from Bare Metal!\r\n");

    // UEFIに制御を戻さずフリーズさせる
    while (1);
}