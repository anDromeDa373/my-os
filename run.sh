#!/bin/bash
set -e

# 1. ビルド
cargo build --target x86_64-unknown-uefi

# 2. 物理構造の完全リセットと再構築
rm -rf disk
mkdir -p disk/EFI/BOOT
cp target/x86_64-unknown-uefi/debug/my_os.efi disk/EFI/BOOT/BOOTX64.EFI

# 3. startup.nsh の作成 (CRLFを強制し、エスケープを正しく行う)
# printf を使い、\r\n を明示的に送り込む
printf "fs0:\r\nEFI\\BOOT\\BOOTX64.EFI\r\n" > disk/startup.nsh

# 4. QEMU 起動
qemu-system-x86_64 \
    -m 1G \
    -drive if=pflash,format=raw,readonly=on,file=/opt/homebrew/share/qemu/edk2-x86_64-code.fd \
    -drive format=raw,file=fat:rw:disk \
    -net none