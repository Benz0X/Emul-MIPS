
/**
 * @file relocator.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Nov  5 20:11:00 2013
 * @brief MIPS 32 bits relocation.
 *
 * MIPS 32 bits relocation.
 */

#ifndef _RELOCATOR_H_
#define _RELOCATOR_H_

#ifdef __cplusplus
extern "C" {
#endif


#define R_MIPS_NONE      0
#define R_MIPS_16        1
#define R_MIPS_32        2
#define R_MIPS_REL32     3
#define R_MIPS_26        4
#define R_MIPS_HI16      5
#define R_MIPS_LO16      6
#define R_MIPS_GPREL16   7
#define R_MIPS_LITERAL   8
#define R_MIPS_GOT16     9
#define R_MIPS_PC16     10
#define R_MIPS_CALL16   11
#define R_MIPS_GPREL32  12
#define R_MIPS_GOTHI16  21
#define R_MIPS_GOTLO16  22
#define R_MIPS_CALLHI16 30
#define R_MIPS_CALLLO16 31

static char *MIPS32_REL[] = {
    "R_MIPS_NONE", "R_MIPS_16", "R_MIPS_32", "R_MIPS_REL32",
    "R_MIPS_26", "R_MIPS_HI16", "R_MIPS_LO16", "R_MIPS_GPREL16",
    "R_MIPS_LITERAL", "R_MIPS_GOT16", "R_MIPS_PC16", "R_MIPS_CALL16",
    "R_MIPS_GPREL32", "", "", "", "", "", "", "", "", "R_MIPS_GOTHI16",
    "R_MIPS_GOTLO16", "", "", "", "", "", "", "", "R_MIPS_CALLHI16",
    "R_MIPS_CALLLO16"
};

#ifdef __cplusplus
}
#endif

#endif /* _RELOCATOR_H_ */
