CXX = clang++
LD = ld.lld

# -fPIC で位置独立コードを出力させ、再配置情報 (.rela.dyn) を生成させる
CXXFLAGS = -target x86_64-elf -ffreestanding -mno-red-zone -fPIC -fno-rtti -fno-exceptions -std=c++17 -Wall -MMD -MP

OBJS = main.o pixel_writer.o font.o hankaku.o

# kernel.ld を使わず、ブートローダー想定の標準レイアウトでリンク
kernel.elf: $(OBJS)
	$(LD) --entry KernelMain -shared -Bsymbolic -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

hankaku.o: hankaku.bin
	objcopy -I binary -O elf64-x86-64 -B i386 $< $@

clean:
	rm -f *.o *.d kernel.elf

-include $(OBJS:.o=.d)