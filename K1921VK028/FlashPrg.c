/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright (c) 2010 Keil - An ARM Company. All rights reserved.     */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.C:  Flash Programming Functions adapted                   */
/*               for K1921VK028 512KB Boot Flash                       */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.H"        // FlashOS Structures     
#include <stdint.h>

#define M8(adr)  (*((volatile unsigned char  *) (adr)))
#define M16(adr) (*((volatile unsigned short *) (adr)))
#define M32(adr) (*((volatile unsigned long  *) (adr)))
	
#define BFLASH_MAGIC_KEY		0xC0DE0000
#define BFLASH_READ				0xC0DE0001
#define BFLASH_WRITE			0xC0DE0002
#define BFLASH_SECTOR_ERASE	    0xC0DE0004
#define BFLASH_FULL_ERASE		0xC0DE000C
#define BFLASH_NVR				0x00000100

#define   __I     volatile const       /*!< Defines 'read only' permissions                 */
#define   __O     volatile             /*!< Defines 'write only' permissions                */
#define   __IO    volatile             /*!< Defines 'read / write' permissions              */

/* ================================================================================ */
/* ================                  BFLASH                  ================ */
/* ================================================================================ */

typedef struct {
  /*!< Data Register */
    __IO uint32_t DATA;                                              /*!< DATA : type used for word access */
} _MFLASH_DATA_TypeDef;
typedef struct {
                                                                     /*!< Address Register */
  __IO uint32_t ADDR;                                              /*!< ADDR    : type used for word access */

  __IO uint32_t DATA[4];                                           /*!< Command Register */
  __IO uint32_t Reserved0[12];
  
  __IO uint32_t CMD;                                               /*!< CMD    : type used for word access */
                                                              /*!< Status Register */
  __IO uint32_t STAT;                                              /*!< STAT    : type used for word access */

  __IO uint32_t CTRL;                                              /*!< CTRL    : type used for word access */
} BFLASH_TypeDef;


#define BFLASH_BASE            (0x40061000UL)
#define BFLASH                 ((BFLASH_TypeDef              *) BFLASH_BASE)

#define FULL_FLASH_ERASE_ADDR	0x40080014
// Define for Read SERVICE MODE
#define SERV_DETECT ((M32(FULL_FLASH_ERASE_ADDR) >> 31)&0x01)

/* 
   Mandatory Flash Programming Functions (Called by FlashOS):
                int Init        (unsigned long adr,   // Initialize Flash
                                 unsigned long clk,
                                 unsigned long fnc);
                int UnInit      (unsigned long fnc);  // De-initialize Flash
                int EraseSector (unsigned long adr);  // Erase Sector Function
                int ProgramPage (unsigned long adr,   // Program Page Function
                                 unsigned long sz,
                                 unsigned char *buf);

   Optional  Flash Programming Functions (Called by FlashOS):
                int BlankCheck  (unsigned long adr,   // Blank Check
                                 unsigned long sz,
                                 unsigned char pat);
                int EraseChip   (void);               // Erase complete Device
      unsigned long Verify      (unsigned long adr,   // Verify Function
                                 unsigned long sz,
                                 unsigned char *buf);

       - BlanckCheck  is necessary if Flash space is not mapped into CPU memory space
       - Verify       is necessary if Flash space is not mapped into CPU memory space
       - if EraseChip is not provided than EraseSector for all sectors is called
*/


/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

  /* Add your Code */
	/* Configurate time acces to FLASH */

  return (0);                                  // Finished without Errors
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {

  /* Add your Code */
	/* Configurate time acces to FLASH defualt value on 100MHz*/

  return (0);                                  // Finished without Errors
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) {
  /* Add your Code */
  if (SERV_DETECT) M32(FULL_FLASH_ERASE_ADDR)=1;
  else {
	BFLASH->CMD = BFLASH_FULL_ERASE;
	__nop();
	__nop();
	__nop();
	__nop();	
	while (BFLASH->STAT & 1);
  }	
  return (0);                                  // Finished without Errors
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {
  /* Add your Code */
	BFLASH->ADDR = adr;
	BFLASH->CMD = BFLASH_SECTOR_ERASE;
	__nop();
	__nop();
	__nop();
	__nop();	
	while (BFLASH->STAT & 1){};
  return (0);                                  // Finished without Errors
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {

  /* Add your Code */
           int  i,j;
                           // Number of 4*4-byte Buffers
  for (i = 0; i < ((sz + 15) / 16); i++)  {
		BFLASH->ADDR = adr;
		//Load Data in Registers
		for (j = 0; j < 4; j++) 
		{
			BFLASH->DATA[j] = *((unsigned long *) buf); 	// Write Data to Buffers 
		  buf += 4;											// Next Data
		}	
		adr += 16;                                 			// Next Address
		BFLASH->CMD = BFLASH_WRITE; // WRITE DATA to BOOTFLASH
	__nop();
	__nop();
    __nop();
	__nop();
    __nop();	
		while (BFLASH->STAT & 1);
	}
  return (0);                                  // Finished without Errors
}
