SECTIONS
{
    . = 0x80000;
    .text : { *(.text) }
    
    . = ALIGN(0x8);
    bss_begin = .;
    .bss : { *(.bss*) } 
    bss_end = .;
    
}