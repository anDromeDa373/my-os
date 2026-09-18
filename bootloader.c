#define NULL ((void*)0)

typedef unsigned long long UINTN;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;
typedef unsigned char UINT8;
typedef UINTN EFI_STATUS;
typedef void *EFI_HANDLE;

typedef struct EFI_SYSTEM_TABLE EFI_SYSTEM_TABLE;

typedef struct {
    UINT64 Signature;
    UINT32 Revision;
    UINT32 HeaderSize;
    UINT32 CRC32;
    UINT32 Reserved;
} EFI_TABLE_HEADER;

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    void *Reset;
    EFI_STATUS (*OutputString)(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, UINT16 *String);
};

typedef struct {
    UINT32 Data1;
    UINT16 Data2;
    UINT16 Data3;
    UINT8  Data4[8];
} EFI_GUID;

typedef struct EFI_FILE_PROTOCOL EFI_FILE_PROTOCOL;
struct EFI_FILE_PROTOCOL {
    UINT64 Revision;
    EFI_STATUS (*Open)(EFI_FILE_PROTOCOL *This, EFI_FILE_PROTOCOL **NewHandle, UINT16 *FileName, UINT64 OpenMode, UINT64 Attributes);
    EFI_STATUS (*Close)(EFI_FILE_PROTOCOL *This);
    EFI_STATUS (*Delete)(EFI_FILE_PROTOCOL *This);
    EFI_STATUS (*Read)(EFI_FILE_PROTOCOL *This, UINTN *BufferSize, void *Buffer);
};

typedef struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;
struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
    UINT64 Revision;
    EFI_STATUS (*OpenVolume)(EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This, EFI_FILE_PROTOCOL **Root);
};

typedef struct EFI_LOADED_IMAGE_PROTOCOL EFI_LOADED_IMAGE_PROTOCOL;
struct EFI_LOADED_IMAGE_PROTOCOL {
    UINT32 Revision;
    EFI_HANDLE ParentHandle;
    EFI_SYSTEM_TABLE *SystemTable;
    EFI_HANDLE DeviceHandle;
};

typedef struct {
    EFI_TABLE_HEADER Hdr;
    void *RaiseTPL;
    void *RestoreTPL;
    EFI_STATUS (*AllocatePages)(int Type, int MemoryType, UINTN Pages, UINT64 *Memory);
    EFI_STATUS (*FreePages)(UINT64 Memory, UINTN Pages);
    EFI_STATUS (*GetMemoryMap)(UINTN *MemoryMapSize, void *MemoryMap, UINTN *MapKey, UINTN *DescriptorSize, UINT32 *DescriptorVersion);
    EFI_STATUS (*AllocatePool)(int PoolType, UINTN Size, void **Buffer);
    EFI_STATUS (*FreePool)(void *Buffer);
    void *CreateEvent;
    void *SetTimer;
    void *WaitForEvent;
    void *SignalEvent;
    void *CloseEvent;
    void *CheckEvent;
    void *InstallProtocolInterface;
    void *ReinstallProtocolInterface;
    void *UninstallProtocolInterface;
    EFI_STATUS (*HandleProtocol)(EFI_HANDLE Handle, EFI_GUID *Protocol, void **Interface);
    void *Reserved;
    void *RegisterProtocolNotify;
    EFI_STATUS (*LocateHandle)(int SearchType, EFI_GUID *Protocol, void *SearchKey, UINTN *BufferSize, EFI_HANDLE *Buffer);
    EFI_STATUS (*LocateDevicePath)(EFI_GUID *Protocol, void **DevicePath, EFI_HANDLE *Device);
    void *InstallConfigurationTable;
    void *LoadImage;
    void *StartImage;
    void *Exit;
    void *UnloadImage;
    EFI_STATUS (*ExitBootServices)(EFI_HANDLE ImageHandle, UINTN MapKey);
    void *GetNextMonotonicCount;
    void *Stall;
    void *SetWatchdogTimer;
    void *ConnectController;
    void *DisconnectController;
    void *OpenProtocol;
    void *CloseProtocol;
    void *OpenProtocolInformation;
    void *ProtocolsPerHandle;
    void *LocateHandleBuffer;
    EFI_STATUS (*LocateProtocol)(EFI_GUID *Protocol, void *Registration, void **Interface);
} EFI_BOOT_SERVICES;

struct EFI_SYSTEM_TABLE {
    EFI_TABLE_HEADER Hdr;
    UINT16 *FirmwareVendor;
    UINT32 FirmwareRevision;
    EFI_HANDLE ConsoleInHandle;
    void *ConIn;
    EFI_HANDLE ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    EFI_HANDLE StandardErrorHandle;
    void *StdErr;
    void *RuntimeServices;
    EFI_BOOT_SERVICES *BootServices;
};

typedef struct {
    UINT32 Version;
    UINT32 HorizontalResolution;
    UINT32 VerticalResolution;
    UINT32 PixelFormat;
    UINT32 BitMask[4];
    UINT32 PixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct {
    UINT32 MaxMode;
    UINT32 Mode;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
    UINTN SizeOfInfo;
    UINT64 FrameBufferBase;
    UINTN FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct EFI_GRAPHICS_OUTPUT_PROTOCOL {
    void *QueryMode;
    void *SetMode;
    void *Blt;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

typedef struct {
    UINT8* frame_buffer;
    UINT32 pixels_per_scan_line;
    UINT32 horizontal_resolution;
    UINT32 vertical_resolution;
    UINT32 pixel_format;
} FrameBufferConfig;

typedef void (*KernelMainAsync)(const FrameBufferConfig*);

typedef struct {
    unsigned char e_ident[16];
    UINT16 e_type;
    UINT16 e_machine;
    UINT32 e_version;
    UINT64 e_entry;
    UINT64 e_phoff;
    UINT64 e_shoff;
    UINT32 e_flags;
    UINT16 e_ehsize;
    UINT16 e_phentsize;
    UINT16 e_phnum;
    UINT16 e_shentsize;
    UINT16 e_shnum;
    UINT16 e_shstrndx;
} Elf64_Ehdr;

typedef struct {
    UINT32 p_type;
    UINT32 p_flags;
    UINT64 p_offset;
    UINT64 p_vaddr;
    UINT64 p_paddr;
    UINT64 p_filesz;
    UINT64 p_memsz;
    UINT64 p_align;
} Elf64_Phdr;

#define PT_LOAD 1

static EFI_GUID gEfiLoadedImageProtocolGuid = 
    {0x5b1b31a1, 0x9562, 0x11d2, {0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
static EFI_GUID gEfiSimpleFileSystemProtocolGuid = 
    {0x0964e5b2, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
static EFI_GUID gEfiGraphicsOutputProtocolGuid = 
    {0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}};

void PutS(EFI_SYSTEM_TABLE *ST, const UINT16 *str) {
    ST->ConOut->OutputString(ST->ConOut, (UINT16*)str);
}

void PutHex(EFI_SYSTEM_TABLE *ST, UINT64 val) {
    UINT16 buf[19];
    buf[0] = L'0'; buf[1] = L'x';
    for (int i = 15; i >= 0; i--) {
        UINT8 nibble = (val >> (i * 4)) & 0xF;
        buf[2 + (15 - i)] = (nibble < 10) ? (L'0' + nibble) : (L'A' + nibble - 10);
    }
    buf[18] = L'\0';
    PutS(ST, buf);
}

void *memset(void *s, int c, UINTN n) {
    unsigned char *p = (unsigned char *)s;
    while (n--) *p++ = (unsigned char)c;
    return s;
}

void *memcpy(void *dest, const void *src, UINTN n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    while (n--) *d++ = *s++;
    return dest;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    PutS(SystemTable, L"[1/6] Open Filesystem...\r\n");

    EFI_LOADED_IMAGE_PROTOCOL *loaded_image;
    EFI_STATUS st = SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&loaded_image);
    if (st != 0) { PutS(SystemTable, L"Err HandleProtocol LoadedImage\r\n"); while(1); }

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;
    st = SystemTable->BootServices->HandleProtocol(loaded_image->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&fs);
    if (st != 0) { PutS(SystemTable, L"Err HandleProtocol FileSystem\r\n"); while(1); }

    EFI_FILE_PROTOCOL *root;
    st = fs->OpenVolume(fs, &root);
    if (st != 0) { PutS(SystemTable, L"Err OpenVolume\r\n"); while(1); }

    PutS(SystemTable, L"[2/6] Open kernel.elf...\r\n");
    EFI_FILE_PROTOCOL *kernel_file;
    st = root->Open(root, &kernel_file, L"kernel.elf", 1, 0);
    if (st != 0) {
        PutS(SystemTable, L"[Error] kernel.elf missing! status=");
        PutHex(SystemTable, st);
        PutS(SystemTable, L"\r\n");
        while(1);
    }

    PutS(SystemTable, L"[3/6] Allocate Temp Buffer & Read...\r\n");
    UINT64 temp_buf_addr = 0x2000000; // 32MB付近へ安全に配置
    st = SystemTable->BootServices->AllocatePages(0, 2, 0x400, &temp_buf_addr);
    if (st != 0) { PutS(SystemTable, L"Err Allocate Temp Pages\r\n"); while(1); }

    UINTN file_size = 0x400000;
    st = kernel_file->Read(kernel_file, &file_size, (void*)temp_buf_addr);
    if (st != 0) { PutS(SystemTable, L"Err Read Kernel File\r\n"); while(1); }

    PutS(SystemTable, L"[4/6] Load ELF Segments...\r\n");
    Elf64_Ehdr *ehdr = (Elf64_Ehdr*)temp_buf_addr;
    Elf64_Phdr *phdr = (Elf64_Phdr*)(temp_buf_addr + ehdr->e_phoff);

    for (UINT16 i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            UINT64 seg_addr = phdr[i].p_vaddr;
            UINTN num_pages = (phdr[i].p_memsz + 4095) / 4096;
            st = SystemTable->BootServices->AllocatePages(2, 2, num_pages, &seg_addr);
            if (st != 0) {
                PutS(SystemTable, L"Err Alloc Segment Addr=");
                PutHex(SystemTable, seg_addr);
                PutS(SystemTable, L" Status=");
                PutHex(SystemTable, st);
                PutS(SystemTable, L"\r\n");
                while(1);
            }
            memcpy((void*)phdr[i].p_vaddr, (void*)(temp_buf_addr + phdr[i].p_offset), phdr[i].p_filesz);
            memset((void*)(phdr[i].p_vaddr + phdr[i].p_filesz), 0, phdr[i].p_memsz - phdr[i].p_filesz);
        }
    }

    PutS(SystemTable, L"[5/6] Setup GOP...\r\n");
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL;
    st = SystemTable->BootServices->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (void**)&gop);
    if (st != 0) { PutS(SystemTable, L"Err Locate GOP\r\n"); while(1); }

    FrameBufferConfig config;
    config.frame_buffer = (UINT8*)gop->Mode->FrameBufferBase;
    config.pixels_per_scan_line = gop->Mode->Info->PixelsPerScanLine;
    config.horizontal_resolution = gop->Mode->Info->HorizontalResolution;
    config.vertical_resolution = gop->Mode->Info->VerticalResolution;
    config.pixel_format = gop->Mode->Info->PixelFormat;

    PutS(SystemTable, L"[6/6] Jump to Kernel Main!\r\n");
    KernelMainAsync kernel_entry = (KernelMainAsync)ehdr->e_entry;
    kernel_entry(&config);

    while (1);
    return 0;
}