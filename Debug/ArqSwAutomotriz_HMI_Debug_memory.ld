/*
 * GENERATED FILE - DO NOT EDIT
 * (c) Code Red Technologies Ltd, 2008-2013
 * (c) NXP Semiconductors 2013-2019
 * Generated linker script file for MKW36Z512xxx4
 * Created from memory.ldt by FMCreateLinkMemory
 * Using Freemarker v2.3.23
 * MCUXpresso IDE v10.2.0 [Build 759] [2018-05-15] on Jun 25, 2019 8:51:41 PM
 */

MEMORY
{
  /* Define each memory region */
  PROGRAM_FLASH (rx) : ORIGIN = 0x0, LENGTH = 0x40000 /* 256K bytes (alias Flash) */  
  FLEX_NVM (rx) : ORIGIN = 0x10000000, LENGTH = 0x40000 /* 256K bytes (alias Flash2) */  
  SRAM (rwx) : ORIGIN = 0x1fffc000, LENGTH = 0x10000 /* 64K bytes (alias RAM) */  
  FLEX_RAM (rwx) : ORIGIN = 0x14000000, LENGTH = 0x2000 /* 8K bytes (alias RAM2) */  
}

  /* Define a symbol for the top of each memory region */
  __base_PROGRAM_FLASH = 0x0  ; /* PROGRAM_FLASH */  
  __base_Flash = 0x0 ; /* Flash */  
  __top_PROGRAM_FLASH = 0x0 + 0x40000 ; /* 256K bytes */  
  __top_Flash = 0x0 + 0x40000 ; /* 256K bytes */  
  __base_FLEX_NVM = 0x10000000  ; /* FLEX_NVM */  
  __base_Flash2 = 0x10000000 ; /* Flash2 */  
  __top_FLEX_NVM = 0x10000000 + 0x40000 ; /* 256K bytes */  
  __top_Flash2 = 0x10000000 + 0x40000 ; /* 256K bytes */  
  __base_SRAM = 0x1fffc000  ; /* SRAM */  
  __base_RAM = 0x1fffc000 ; /* RAM */  
  __top_SRAM = 0x1fffc000 + 0x10000 ; /* 64K bytes */  
  __top_RAM = 0x1fffc000 + 0x10000 ; /* 64K bytes */  
  __base_FLEX_RAM = 0x14000000  ; /* FLEX_RAM */  
  __base_RAM2 = 0x14000000 ; /* RAM2 */  
  __top_FLEX_RAM = 0x14000000 + 0x2000 ; /* 8K bytes */  
  __top_RAM2 = 0x14000000 + 0x2000 ; /* 8K bytes */  
