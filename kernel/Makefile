CXX = clang++
LD = ld.lld

CXXFLAGS = -target x86_64-elf -ffreestanding -mno-red-zone -fPIC -fno-rtti -fno-exceptions -std=c++17 -Wall -MMD -MP

OBJS = main.o pixel_writer.o

kernel.elf: $(OBJS)
	$(LD) -s -shared -Bsymbolic -e KernelMain -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o kernel.elf