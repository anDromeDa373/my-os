import re

def convert():
    font_data = bytearray(256 * 16) # 4096バイトのバッファ
    current_code = -1
    line_count = 0

    with open("hankaku.txt", "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue

            # 行の中から "0x" ＋ 16進数の部分（例: "0x41"）だけを正規表現で探す
            match = re.search(r"0x([0-9a-fA-F]+)", line)
            if match and (line.startswith("0x") or line.startswith("char")):
                current_code = int(match.group(1), 16)
                line_count = 0
                continue

            # 8文字のドット絵行をパース
            if len(line) == 8 and 0 <= current_code < 256 and line_count < 16:
                val = 0
                for i, char in enumerate(line):
                    if char != '.':  # '.' 以外（'@'など）を1ビットとして立てる
                        val |= (1 << (7 - i))
                
                font_data[current_code * 16 + line_count] = val
                line_count += 1

    with open("hankaku.bin", "wb") as f:
        f.write(font_data)

    non_zero = sum(1 for b in font_data if b != 0)
    print(f"hankaku.bin を生成しました (サイズ: {len(font_data)} バイト)")
    print(f"0以外のデータバイト数: {non_zero} / 4096")

if __name__ == "__main__":
    convert()