ENTRY(Reset_Handler)

MEMORY
{
    FLASH(rx):ORIGIN =0x08000000,LENGTH =384K
    /* 
        SECTOR 7 WILL BE USED AS DATA SECTOR .
        REMOVE THIS LINE BELLOW IF YOU DON'T WANT
        TO USE FLASH RUNTIME OR YOUR PROGRAM NEEDS
        MORE THAN 384K FLASH
    */
    DATA(rw):ORIGIN =0x08060000,LENGTH =128K
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
    /* 
        SECTOR 7 WILL BE USED AS DATA SECTOR .
        REMOVE THIS .user_data SECTION BELLOW IF 
        YOU DON'T WANT TO USE FLASH RUNTIME OR 
        YOUR PROGRAM NEEDS MORE THAN 384K FLASH
    */
    .user_data :
    {
        KEEP(*(.user_data))
        . = ALIGN(4);
    } > DATA

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