/* 
 * Disassembly code for the PIC 12CXXX microcontrollers.
 * 
 * Written by Owen Klan  -  March 2003
 */

#include <stdio.h>
#include <stdlib.h>

#include "../picdasm.h"

static char *get_mnemonic(unsigned short instr, int type);
static char *get_fsr_name(int fsr);

#define MAX_INDEX     0x0B

/* Function that returns the name of a special purpose register. */
static char *get_fsr_name(int fsr)
{    
    char *fsr_names[MAX_INDEX + 1] = {
	"iap",			       /* Indirect Address Pointer (0x00) */
	  "tmr0",		       /* Timer/Counter 0          (0x01) */
	  "pcl",		       /* Program counter low byte (0x02) */
	  "status",		       /* Status register */
	  "fsel",		       /* File select */
	  "porta",		       /* Port A data */
	  "portb",		       /* Port B data */
	  "7",
	  "eedata",
	  "eeaddr",
	  "pclath",
	  "intcon"
    };
    
    if (fsr > MAX_INDEX) {    
	return NULL;
    } else {
	return fsr_names[fsr];
    }
}

/* Function that decodes an instruction and returns a pointer
 * to a malloc()'ed string containing the instruction description.
 * Returns NULL on error. */
char *disassemble_12cxxx (unsigned short instr, unsigned int options)
{
    char *instr_desc = NULL;
    
    if ((instr_desc = calloc(16, 1)) == NULL) {
	return NULL;
    }

    /* Check to see if this is a hard-coded instruction */
    if (HARD_CODED_12(instr)) {
	sprintf(instr_desc, "%-8s", get_mnemonic(instr, 0));
	return instr_desc;
    }
    
    /* Determine what group it falls under */
    switch (INSTR_GROUP_12(instr)) {
      case 0x00: {		       /* Byte-oriented */
	  if ((options & FSR_NAMES) == FSR_NAMES
	   && BYTE_GET_F_12(instr) <= MAX_INDEX) {
	      sprintf(instr_desc, "%-8s  %s, %c",
		      get_mnemonic(instr, BYTE_INSTR),
		      get_fsr_name(BYTE_GET_F_12(instr)),
		      BYTE_GET_D_12(instr) ? 'f' : 'W');

	  } else {
	      sprintf(instr_desc, "%-8s  %d, %c",
		      get_mnemonic(instr, BYTE_INSTR),
		      BYTE_GET_F_12(instr),
		      BYTE_GET_D_12(instr) ? 'f' : 'W');
	  }
	  break;
      }
      case 0x01: {		       /* Bit-oriented */
	  if ((options & FSR_NAMES) == FSR_NAMES
	   && BIT_GET_F_12(instr) <= MAX_INDEX) {
	      sprintf(instr_desc, "%-8s  %s, %d",
		      get_mnemonic(instr, BIT_INSTR),
		      get_fsr_name(BIT_GET_F_12(instr)),
		      BIT_GET_B_12(instr));
	  } else {
	      sprintf(instr_desc, "%-8s  %d, %d",
		      get_mnemonic(instr, BIT_INSTR),
		      BIT_GET_F_12(instr),
		      BIT_GET_B_12(instr));
	  }
	  break;
      }
      case 0x02: {		       /* Call or Goto */
	  sprintf(instr_desc, "%-8s  0x%X",
		  get_mnemonic(instr, 0),
		  (instr & 0x7FF));
	  break;
      }
      case 0x03: {		       /* Literal and control */
	  sprintf(instr_desc, "%-8s  0x%X",
		  get_mnemonic(instr, LC_INSTR),
		  (instr & 0xFF));
	  break;
      }
    }
    
    return instr_desc;
}

static char *get_mnemonic(unsigned short instr, int type)
{
    /* If it's a hard-coded instruction, check now */
    switch (instr) {
      case 0x00:
      case 0x20:
      case 0x40:
      case 0x60: return "nop";
      case 0x64: return "clrwdt";
      case 0x09: return "retfie";
      case 0x08: return "return";
      case 0x63: return "sleep";
    }
    
    if (type == BIT_INSTR) {
	switch ((instr & 0x3C00) >> 10) {
	  case 0x04: return "bcf";
	  case 0x05: return "bsf";
	  case 0x06: return "btfsc";
	  case 0x07: return "btfss";
	};
    } else if (type == BYTE_INSTR) {
	switch ((instr & 0x3F00) >> 8) {
	  case 0x07: return "addwf";
	  case 0x05: return "andwf";
	  case 0x01: {
	      if (BYTE_GET_D_12(instr)) { /* File reg. or W for destination? */
		  return "clrf";
	      } else {
		  return "clrw";
	      }
	  }
	  case 0x09: return "comf";
	  case 0x03: return "decf";
	  case 0x0B: return "decfsz";
	  case 0x0A: return "incf";
	  case 0x0F: return "incfsz";
	  case 0x04: return "iorwf";
	  case 0x08: return "movf";
	  case 0x00: return "movwf";
	  case 0x0D: return "rlf";
	  case 0x0C: return "rrf";
	  case 0x02: return "subwf";
	  case 0x0E: return "swapf";
	  case 0x06: return "xorwf";
	};
    } else if (type == LC_INSTR) {
	switch ((instr & 0x3F00) >> 8) {
	  case 0x3F:
	  case 0x3E: return "addlw";
	  case 0x39: return "andlw";
	  case 0x38: return "iorlw";
	  case 0x30:
	  case 0x31:
	  case 0x32:
	  case 0x33: return "movlw";
	  case 0x34:
	  case 0x35:
	  case 0x36:
	  case 0x37: return "retlw";
	  case 0x3C:
	  case 0x3D: return "sublw";
	  case 0x3A: return "xorlw";
	};
    } else {
	if ((instr & 0x3800) >> 11 == 0x05) {
	    return "goto";
	} else {
	    return "call";
	}
    }

    return "unknown";
}
