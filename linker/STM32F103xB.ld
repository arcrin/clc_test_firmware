MEMORY
{
  TEXT (rwx)          : ORIGIN = 0x20000000, LENGTH = 0x00004800   /* SRAM1: 18K for code, bss, and data */
  ALGO_BUFFER (rwx)   : ORIGIN = 0x20004800, LENGTH = 0x00000400   /* SRAM2: 1K for program buffers */
  ALGO_PARAM (rwx)    : ORIGIN = 0x20004C00, LENGTH = 0x00000020   /* SRAM2: 32B for memprog param */
  ALGO_BDT (rwx)      : ORIGIN = 0x20004C20, LENGTH = 0x00000020   /* SRAM2: 32B for memprog bdts */
  HEAPSTACK (rwx)     : ORIGIN = 0x20004C40, LENGTH = 0x000003C0   /* SRAM2: ~1K for heap and stack */
}

stack_size = 0x300;
heap_size = 0x0C0;

PROVIDE ( _estack = ALIGN(ORIGIN(HEAPSTACK) + stack_size ,8) );
ENTRY(Reset_Handler)

SECTIONS
{
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
	} >TEXT

	/* .ARM.exidx is sorted, so has to go in its own output section.  */
	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} >TEXT
	__exidx_end = .;

	. = ALIGN(4);
	_etext = .;
	_sidata = _etext;          /* start of initialized data (in FLASH, LMA) */

	/**
	 * Initialized data section. Placed directly into text section which lives in RAM
	 */
	.data :
	{
		. = ALIGN(4);
		_sdata = .;                /* start of .data label (in TEXT, VMA) */
		*(.data)
		*(.data.*)
	} > TEXT

	/**
	 * .bss section - uninitialized data
	 */
	.bss (NOLOAD):
	{
		. = ALIGN(4);
		_sbss = .;                 /* start of .bss label (for startup) */
		*(.bss)
		*(.bss.*)
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;                 /* end of .bss label (for startup) */
	} >TEXT

	/**
	 * .noinit section - uninitialized data
	 */
	.noinit (NOLOAD):
	{
		. = ALIGN(4);
		_start_of_noinit = .;
		*(.noinit)
		*(.noinit.*)
		. = ALIGN(4);
		_end_of_noinit = .;
		/* _end = .;                   end of used ram (start of heap) */
		/* __end = .;                  the same */
	} >TEXT

	.user_stack (NOLOAD):
	{
	    . = ALIGN(8);
	    _sstack = .;
	    . = . + stack_size;
	    . = ALIGN(8);
	    _estack = .;
	    end = .;
	    __top_of_stack = .;
	} >HEAPSTACK

	.user_heap (NOLOAD):
	{
	    . = ALIGN(8);
	    _sheap = .;
	    . = . + heap_size;
	    . = ALIGN(8);
	    _eheap = .;
	} >HEAPSTACK


	/**
	 * After that it's only debugging information.
	 */

	DISCARD :    /* remove the debugging information from the standard libraries */
	{
		libc.a ( * )
		libm.a ( * )
		libgcc.a ( * )
	}

	/* Stabs debugging sections.  */
	.stab          0 : { *(.stab) }
	.stabstr       0 : { *(.stabstr) }
	.stab.excl     0 : { *(.stab.excl) }
	.stab.exclstr  0 : { *(.stab.exclstr) }
	.stab.index    0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment       0 : { *(.comment) }
	/**
	 * DWARF debug sections.
	 * Symbols in the DWARF debugging sections are relative to the beginning
	 * of the section so we begin them at 0.
	 */

	/* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }

	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }

	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }

	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }

	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
}


PROVIDE (__idata_start = _sidata);         /* start of initializers */
PROVIDE (__data_start = _sdata);           /* start of initialized data section */
PROVIDE (__data_end = _edata);             /* end of initialized data section */
PROVIDE (__bss_start = _sbss);             /* start of zero-filled data section */
PROVIDE (__bss_end = _ebss);               /* end of zero-filled data section */
