// main.c
// UEFI（PCの起動システム）がこの関数を最初に呼び出します
void efi_main(void* image_handle, void* system_table) {
    // UEFI System Tableという「道具箱」から、文字出力機能の住所を計算します
    // 構造体の細かい定義を省くためのハック的な書き方です
    struct {
        char _padding[64];
        struct {
            void* _unused;
            long long (*output_string)(void*, unsigned short*);
        } *con_out;
    } *st = system_table;

    // 画面に文字を出します。L"..." はUEFIが要求する16bit文字形式です
    st->con_out->output_string(st->con_out, (unsigned short*)L"Hello from Bare Metal!\r\n");

    // 止まらないとすぐにPCが再起動してしまうので、無限ループさせます
    while (1);
}