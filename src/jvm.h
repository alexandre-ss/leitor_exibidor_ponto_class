#ifndef JVM_H
#define JVM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "ClassFile.h"
#include "lista_de_classes.h"
#include "pilha_de_frames.h"
#include "instrucoes.h"
#include "pilha_de_operandos.h"
#include "interpretador.h"

/**
 * @struct  jvm
 * @brief   Estrutura da JVM
 */
typedef struct jvm {
    loaded_class    *classes;           /**< Classes carregadas */
    stack_frame     *frames;            /**< Pilha de Frames da JVM */
    u4              pc;                 /**< Program counter */
    u2              exception;          /**< Exceção */
    object_list     *objects;           /**< Lista de objetos */
    char            *exception_name;    /**< Nome da exceção */
} JVM;

// Global variables
extern JVM *jvm;
extern instruction *instructions;
extern bool SHOW_OPCODES;

/**
 * @brief   Inicializa a estrutura da JVM
 */
void initialize();

/**
 * @brief   Executa a JVM
 */
void run_jvm();

void run_method(method_info *m, char *class_info, int caller);

void interpret_code(u1 *code, u4 length, method_info *m);

frame *create_frame(char *class_info, u2 max_locals);

void free_local_variable(local_variable *v, u2 vetor_length);

bool instruction_branch (char *nomeInstrucao);

u2 check_handler_method(method_info *);

u1* update_pc_current_method (u1 *, u4);

#endif
