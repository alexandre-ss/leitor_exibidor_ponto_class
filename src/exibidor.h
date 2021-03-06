#ifndef EXIBIDOR_H
#define EXIBIDOR_H

#include "ClassFile.h"
#include <math.h>
#include <stdio.h>
#include "leitor.h"

void    print_name_tag(u1 tag);
char*   decode_utf8_string(cp_info *cp);
char*   decode_name_index_and_type(cp_info *cp, u2 index, u1 type);
char*   decode_access_flags(u2 flag);
char*   decode_field_access_flags(u2 flag);
double  decode_double_info(cp_info *cp);
int64_t decode_long_info(cp_info *cp);
float   decode_float_info(cp_info *cp);
void    decode_opcode(u1 code);
// int     setaOffsetImpressao(int posicao, u1 offset);
void    print_class_file(ClassFile *cf);

#endif // EXIBIDOR_H
