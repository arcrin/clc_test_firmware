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

INCLUDE ../firmwareutil/hal/linker/Common.ld

