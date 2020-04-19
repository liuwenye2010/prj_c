/*
 * Need modifying for a different chip 
 *   ARM_ROM.ORIGIN: starting address of ROM
 *   ARM_ROM.LENGTH: length of ROM
 *   ARM_RAM.ORIGIN: starting address of RAM 
 *   ARM_RAM.LENGTH: length of RAM
 *
 * The values below can be addressed in further linker scripts
 * using functions like 'ORIGIN(ARM_RAM)' or 'LENGTH(ARM_RAM)'.
 */

MEMORY
{

  ARM_ROM (rx)  : ORIGIN = 0x00000000, LENGTH =  10K
  ARM_RAM (xrw) : ORIGIN = 0x00080000, LENGTH =  120K

/*
  ARM_RAM_SEC0 (xrw) : ORIGIN = 0x00080000, LENGTH = 16K
  ARM_RAM_SEC1 (xrw) : ORIGIN = 0x00084000, LENGTH = 32K
  ARM_RAM_SEC2 (xrw) : ORIGIN = 0x0008C000, LENGTH = 54K 
*/
  

  
}

