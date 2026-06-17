# my_os — Rust 製 UEFI ブートローダ / OS プロトタイプ

UEFI 環境上で動作する Rust 製 OS の最小実装です。QEMU + OVMF (EDK2) 上で起動し、UEFI の **Simple Text Output Protocol** を直接呼び出してコンソールへメッセージを表示します。

## 概要

- **言語**: Rust (`no_std` / `no_main`)
- **ターゲット**: `x86_64-unknown-uefi`（PE 形式の `.efi` バイナリを生成）
- **エントリポイント**: `efi_main`（UEFI ファームウェアから呼び出される）
- **現在の動作**: 起動直後に `Hello from Rust Pointer!` を表示し、無限ループで待機

過去の C 言語版と同じアプローチで、`SystemTable` の 64 バイトオフセットから `ConOut` を取得し、`OutputString` 関数ポインタを MS ABI (`extern "win64"`) で直接呼び出しています。

## 必要なツールチェーン

| ツール | 用途 |
|--------|------|
| [Rust / rustup](https://rustup.rs/) | コンパイラとパッケージ管理 |
| `x86_64-unknown-uefi` ターゲット | UEFI 向け PE バイナリのビルド |
| [QEMU](https://www.qemu.org/) | x86_64 仮想マシンでの起動・検証 |
| OVMF (EDK2) ファームウェア | UEFI 環境の提供（QEMU 用） |

### 環境構築（macOS / Homebrew の例）

```bash
# Rust のインストール（未導入の場合）
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh

# UEFI ターゲットの追加
rustup target add x86_64-unknown-uefi

# QEMU と OVMF ファームウェア
brew install qemu
```

Linux ではディストリビューションのパッケージマネージャから `qemu-system-x86_64` と `ovmf`（または `edk2-ovmf`）をインストールしてください。OVMF の `.fd` ファイルのパスは `run.sh` 内の `-drive if=pflash,...` で指定しているパスに合わせて調整が必要な場合があります。

## ビルドと実行

プロジェクトルートで以下を実行します。

```bash
./run.sh
```

`run.sh` は次の処理を自動で行います。

1. `cargo build --target x86_64-unknown-uefi` で `.efi` をビルド
2. `disk/EFI/BOOT/BOOTX64.EFI` として FAT イメージ用ディレクトリを構成
3. UEFI Shell 用の `startup.nsh` を生成
4. QEMU を起動（OVMF + virtio-blk ディスク）

QEMU ウィンドウが開き、UEFI Shell 経由でブートローダが実行されます。コンソールにメッセージが表示されれば成功です。

### 手動ビルドのみ行う場合

```bash
cargo build --target x86_64-unknown-uefi
```

生成物: `target/x86_64-unknown-uefi/debug/my_os.efi`

## プロジェクト構成

```
my-os/
├── Cargo.toml      # クレート定義・依存関係
├── Cargo.lock
├── run.sh          # ビルド → ディスク構成 → QEMU 起動スクリプト
├── src/
│   └── main.rs     # efi_main エントリポイント
└── disk/           # run.sh 実行時に生成される FAT ブート用イメージ
    ├── EFI/BOOT/BOOTX64.EFI
    └── startup.nsh
```

## 技術メモ

- `#![no_std]` / `#![no_main]` により、通常の Rust 標準ライブラリや `main` 関数は使用しません。
- UEFI 呼び出し規約は **Microsoft x64 ABI**（`extern "win64"`）です。
- 文字列は UEFI 形式の **UTF-16 (UCS-2)** で、ヌル終端が必要です。
- `Cargo.toml` には `uefi` / `uefi-services` クレートが記載されていますが、現在の `main.rs` はプロトコル構造体を手動定義して直接呼び出しています。

## ライセンス

（未設定）
