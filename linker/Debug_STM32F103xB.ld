ENTRY(Reset_Handler)

MEMORY
{
    FLASH(rx):ORIGIN =0x08000000,LENGTH =64K
    SRAM(rwx):ORIGIN =0x20000000,LENGTH =20K
}

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    }> FLASH
    .text :
    {
        . = ALIGN(4);
		KEEP(*(.isr_vector))
		. = ALIGN(4);
		KEEP(*(.Reset_Handler))
		. = ALIGN(4);
        __preinit_array_start = .;
        KEEP(*(.preinit_array*))
        __preinit_array_end = .;

		__ctors_start__ = .;
        __init_array_start  = .;
		KEEP(SORT(*)(.init_array)) /* eabi uses .init_array for static constructor lists */
		__ctors_end__ = .;
        __init_array_end    = .;

		__dtors_start__ = .;
		__dtors_end__ = .;

		. = ALIGN(4);

        KEEP(*(.init))
        KEEP(*(.fini))
		*(.text)                   /* remaining code */
		*(.text.*)
		*(.rodata)                 /* read-only data (constants) */
		*(.rodata.*)

		*(.eh_frame_hdr)
		*(.eh_frame)
		*(.ARM.extab* .gnu.linkonce.armextab.*)

		. = ALIGN(4);
    }> FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } >FLASH
    __exidx_end = .;

    .init_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))   /* Pre-initializers */
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*))) /* Initializers */
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(.fini_array))   /* Destructors */
        PROVIDE_HIDDEN (__fini_array_end = .);
    } > FLASH

    _la_data = LOADADDR(.data);

    .data :
    {
        _sdata = .;
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        _edata = .;
    }> SRAM AT> FLASH

    .bss :
    {
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss.*)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
        end = .;
        __end__ = end;
    }> SRAM
}