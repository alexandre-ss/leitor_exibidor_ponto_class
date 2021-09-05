#ifndef LEITOR_H
#define LEITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ClassFile.h"
#include "exibidor.h"

#define u1_READ(arq) getc(arq)
#define u2_READ(arq) (getc(arq) << 8) | getc(arq)
#define u4_READ(arq) (getc(arq) << 24) | (getc(arq) << 16 | getc(arq) << 8 | getc(arq))

ClassFile*                read_file (char *);
cp_info*                  read_constant_pool (FILE *fp, cp_info *cp, u2 cp_count);
u2*                       read_interfaces (FILE *fp, u2* interface, u2 i_count);
field_info*               read_fields (FILE *fp, field_info *field, u2 f_count, cp_info *cp);
method_info*              read_methods (FILE *fp, method_info* method, u2 method_count, cp_info *cp);
code_attribute*           read_code (FILE * fp, cp_info *cp);
line_number_table*        read_line_number_table(FILE * fp, cp_info * cp);
exception_table*          read_exception_table (FILE * fp, u2 size) ;
attribute_info*           read_attributes (FILE *fp, attribute_info* a, cp_info *cp);
exception_attribute*      read_exceptions_attribute (FILE *fp);
constant_value_attribute* read_constant_value (FILE *fp);
signature_attribute*      read_signature (FILE *fp);
inner_classes_attribute*  read_inner_classes (FILE *fp, cp_info *cp);
classes*                  read_classes (FILE *fp);
stack_map_table_attribute* read_stack_map_table (FILE *fp);
stack_map_frame*          read_stack_map_frame (FILE *fp);
// verification_type_info*   readVerificationTypeInfo (FILE *fp);
source_file_attribute*    read_source_file (FILE  *fp);

#endif // LEITOR_H