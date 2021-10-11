#ifndef JVM_H
#define JVM_H

#include "ClassFile.h"
#include "lista_de_classes.h"
#include "pilha_de_frames.h"
#include "instrucoes.h"

typedef struct object_list {
    ClassFile *obj;
    u1 size_data;
    u4 *data;
    struct object_list *prox;
    struct object_list *ant;
} object_list;

typedef struct jvm {
    loaded_class *classes;
    stack_frame *frames;
    u4 pc;
    u2 exception;
    object_list *objects;
    char *exception_name;
} JVM;

/**
 * Inicializa a estrutura da JVM
 * @return estrutura da JVM alocada
 */
JVM *inicializar();

/** Executa a JVM */
void run_jvm();

#endif
