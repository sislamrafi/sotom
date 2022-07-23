ENTRY(Reset_Handler)

MEMORY
{
    FLASH(rx):ORIGIN =0x08000000,LENGTH =512K
    SRAM(rwx):ORIGIN =0x20000000,LENGTH =128K
}

SECTIONS
{
    .text :
    {
        _stext = .;
         *(.isr_vector)
         *(.text)
         *(.rodata)
         . = ALIGN(4);
         _etext = .;
    }> FLASH

    _la_data = LOADADDR(.data);
    .data :
    {
        _sdata = .;
        *(.data)
        . = ALIGN(4);
        _edata = .;
    }> SRAM AT> FLASH

    .bss :
    {
        _sbss = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    }> SRAM

}