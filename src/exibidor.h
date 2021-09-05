#ifndef EXIBIDOR_H
#define EXIBIDOR_H

#include "ClassFile.h"
#include <stdio.h>

void      print_name_tag(u1 tag);
char*     decode_utf8_string(cp_info *cp);
char*     decodificaNIeNT(cp_info *cp, u2 index, u1 tipo);
char*     decodificaAccessFlags(u2 flag);
char*     organizandoFlags(char* flagsOrdemInversa);
int       decodificaIntegerInfo (cp_info * cp);
double    decodificaDoubleInfo (cp_info * cp);
uint64_t  decodificaLongInfo (cp_info * cp);
float     decodificaFloatInfo (cp_info * cp);
int       setaOffsetImpressao (int posicao, u1 offset);
void      printClassFile (ClassFile *);

#endif // EXIBIDOR_H
