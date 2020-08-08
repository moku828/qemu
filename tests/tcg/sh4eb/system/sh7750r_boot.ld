OUTPUT_FORMAT("elf32-sh");
OUTPUT_ARCH(sh4);
ENTRY("start");
/* SH7750R Available Memory */
MEMORY
{
  BOOTROM (x) : ORIGIN = 0xA0000000, LENGTH = 16k	/* Boot ROM: 16k */
  SDRAM1 (wx) : ORIGIN = 0xA8000000, LENGTH = 16M	/* SDRAM(1): 16M */
  SDRAM2 (wx) : ORIGIN = 0xAC000000, LENGTH = 16M	/* SDRAM(2): 16M */
}


/* Section Definitions */
SECTIONS
{
  .text :
  {
	*(.text .text.*)	/* Program code */
	*(.rodata)		/* Read-only data (constants) */
	*(.rodata*)
	. = ALIGN(4);
	__text_end__ = . ;
  } > BOOTROM


  /* .data section which has initialized data */
  .data :
  {
	__data_start__ = . ;
	*(.data)
	*(.data.*)
	*(.zdata)
	. = ALIGN(4);
	__data_end__ = . ;
  } > SDRAM1


  /* .bss section which is to be initialized by 0 */
  .bss (NOLOAD) :
  {
	__bss_start__ = . ;
	*(.bss)
	*(.bss.*)
	*(COMMON)
	*(.got.*)
	. = ALIGN(4);
	__bss_end__ = . ;
  } > SDRAM1


  .stack (NOLOAD) :
  {
	__stack_start__ = . ;
	*(.STACK)
	*(.stack)
	. = ALIGN(4);
	__stack_end__ = . ;
  } > SDRAM1




}
