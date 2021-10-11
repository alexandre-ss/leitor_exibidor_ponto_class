#ifndef LISTA_DE_CLASSES_H
#define LISTA_DE_CLASSES_H

#include "ClassFile.h"
#include "leitor.h"
#include "exibidor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct loaded_class {
  ClassFile *class_file;
  struct loaded_class *next;
  struct loaded_class *prev;
} loaded_class;

loaded_class  *make_class_list();

loaded_class  *insert_class_element(loaded_class *, ClassFile *);

loaded_class  *remove_last_class_element(loaded_class *);

loaded_class  *remove_class_element(loaded_class *, char *);

loaded_class  *find_class_element(loaded_class *, char *);

cp_info       *find_class_constant_pool(loaded_class *, char *);

method_info   *find_class_method(loaded_class *, char *, char *);

field_info    *find_class_field(loaded_class *, char *, char *);

void print_class_list(loaded_class *);

void free_class_list(loaded_class *);


#endif