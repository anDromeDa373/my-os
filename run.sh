# 1. ビルド
# --target x86_64-unknown-uefi は PE 形式のバイナリを作るために必須
cargo build --target x86_64-unknown-uefi

# 2. ディスク構造の再構築（クリーンな状態を作る）
rm -rf disk
mkdir -p disk/EFI/BOOT
cp target/x86_64-unknown-uefi/debug/my_os.efi disk/EFI/BOOT/BOOTX64.EFI

# 3. startup.nsh の作成（\r\n で UEFI Shell の不機嫌を直す）
# \E がエスケープされないよう、先頭のバックスラッシュを抜いた相対パスにする
printf "fs0:\r\nEFI\\BOOT\\BOOTX64.EFI\r\n" > disk/startup.nsh

# 4. QEMU 起動（bootindex=0 で Boot Manager を黙らせる）
qemu-system-x86_64 \
    -m 1G \
    -drive if=pflash,format=raw,readonly=on,file=/opt/homebrew/share/qemu/edk2-x86_64-code.fd \
    -drive id=main_disk,if=none,format=raw,file=fat:rw:disk \
    -device virtio-blk-pci,drive=main_disk,bootindex=0 \
    -net none