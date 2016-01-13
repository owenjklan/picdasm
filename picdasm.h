/* Include file for the PIC disassembler.
 * 
 * Written by Owen Klan  -  March 2003
 */

#ifndef __PIC_DASM_H__
#define __PIC_DASM_H__

/* Flags for the options */
#define FSR_NAMES      0x00000001      /* Map FSR #'s to names? */

/* PIC Family instruction sets */
#define PIC_12CXXX     1
#define PIC_16FXX      2

/* This macro obtains the instruction group specifier for 14-bit
 * instruction words . */
#define INSTR_GROUP_14(i)  (((i) & 0x3000) >> 12)

#define INSTR_GROUP_12(i)  (((i) & 0x0000) >> 00)

/* Instruction types for mnemonic retrieval */
#define BIT_INSTR   1
#define BYTE_INSTR  2
#define LC_INSTR    3

#define DBIT_W   0
#define DBIT_F   1

/*
 * Bit-field macros for 12-bit instruction components, such as 12C508
 */
/* Macros for getting bit fields out of bit-oriented instructions */
#define BIT_GET_F_12(i)    ((i) & 0x1F)
#define BIT_GET_B_12(i)    (((i) & 0xE0) >> 5)
/* Macros for getting bit fields out of byte-oriented instructions */
#define BYTE_GET_F_12(i)   ((i) & 0x1F)
#define BYTE_GET_D_12(i)   ((i) & 0x20)


/*
 * Bit-field macros for 14-bit instruction components, such as 16F84
 */
/* Macros for getting bit fields out of bit-oriented instructions */
#define BIT_GET_F_14(i)    ((i) & 0x7F)
#define BIT_GET_B_14(i)    (((i) & 0x380) >> 7)
/* Macros for getting bit fields out of byte-oriented instructions */
#define BYTE_GET_F_14(i)   ((i) & 0x7F)
#define BYTE_GET_D_14(i)   ((i) & 0x80)


/* This returns true if the instruction is hard-coded. */
#define HARD_CODED_14(x)   ((unsigned short)(x == 0x00)  \
				|| (unsigned short)((x) == 0x20)  \
				|| (unsigned short)((x) == 0x40)  \
				|| (unsigned short)(x) == 0x60    \
				|| (unsigned short)(x) == 0x64    \
				|| (unsigned short)(x) == 0x09    \
				|| (unsigned short)(x) == 0x08    \
				|| (unsigned short)(x) == 0x63)

#define HARD_CODED_12(x)   ((unsigned short)(x == 0x00)  \
				|| (unsigned short)((x) == 0x40)  \
				|| (unsigned short)((x) == 0x04)  \
				|| (unsigned short)((x) == 0x03)  \
				|| (unsigned short)((x) == 0x02))

/* Function prototypes */
extern int  pdasm_get_version();
  
/* Function prototypes for actual disassembly routines. */
/* From 16f84/16f84.c */
extern char *disassemble_16f84 (unsigned short instr, unsigned int options);

#endif				       /* __PIC_DASM_H__ */
