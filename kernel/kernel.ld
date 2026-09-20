SECTIONS
{
    . = 0x100000;

    .text : {
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .rela.dyn : {
        __rela_dyn_start = .;
        *(.rela.dyn)
        __rela_dyn_end = .;
    }

    .bss : {
        *(.bss*)
    }
}