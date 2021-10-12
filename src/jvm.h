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

/**
 * @struct  object_list
 * @brief   Lista de objetos
 */
typedef struct object_list {
    ClassFile *obj;
    u1 size_data;
    u4 *data;
    struct object_list *prox;
    struct object_list *ant;
} object_list;

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
JVM *jvm = NULL;
instruction *instructions;

/**
 * @brief   Inicializa a estrutura da JVM
 * @return  Estrutura da JVM alocada
 */
JVM *initialize();

/**
 * @brief   Executa a JVM
 */
void run_jvm();

void run_method(method_info *m, char *class_info, int caller);

void interpret_code(u1 *code, u4 length, method_info *m);

frame *create_frame(char *class_info, u2 max_locals);

void free_local_variable(local_variable *v, u2 vetor_length);

bool instruction_branch (char *nomeInstrucao);

// ! DANDO ERRO POIS NÃO ESTÁ DEFINIDO NO INTERPRETADOR
void (*func_ptr[202])(frame*, u1, u1) = {
	nop_impl,
    aconst_null_impl,
    iconst_m1_impl,
    iconst_0_impl,
    iconst_1_impl,
    iconst_2_impl,
    iconst_3_impl,
    iconst_4_impl,
    iconst_5_impl,
    lconst_0_impl,
    lconst_1_impl,
    fconst_0_impl,
    fconst_1_impl,
    fconst_2_impl,
    dconst_0_impl,
    dconst_1_impl,
    bipush_impl,
    sipush_impl,
    ldc_impl,
    ldc_w_impl,
    ldc2_w_impl,
    iload_impl,
    lload_impl,
    fload_impl,
    dload_impl,
    aload_impl,
    iload_0_impl,
    iload_1_impl,
    iload_2_impl,
    iload_3_impl,
    lload_0_impl,
    lload_1_impl,
    lload_2_impl,
    lload_3_impl,
    fload_0_impl,
    fload_1_impl,
    fload_2_impl,
    fload_3_impl,
    dload_0_impl,
    dload_1_impl,
    dload_2_impl,
    dload_3_impl,
    aload_0_impl,
    aload_1_impl,
    aload_2_impl,
    aload_3_impl,
    iaload_impl,
    laload_impl,
    faload_impl,
    daload_impl,
    aaload_impl,
    baload_impl,
    caload_impl,
    saload_impl,
    istore_impl,
    lstore_impl,
    fstore_impl,
    dstore_impl,
    astore_impl,
    istore_0_impl,
    istore_1_impl,
    istore_2_impl,
    istore_3_impl,
    lstore_0_impl,
    lstore_1_impl,
    lstore_2_impl,
    lstore_3_impl,
    fstore_0_impl,
    fstore_1_impl,
    fstore_2_impl,
    fstore_3_impl,
    dstore_0_impl,
    dstore_1_impl,
    dstore_2_impl,
    dstore_3_impl,
    astore_0_impl,
    astore_1_impl,
    astore_2_impl,
    astore_3_impl,
    iastore_impl,
    lastore_impl,
    fastore_impl,
    dastore_impl,
    aastore_impl,
    bastore_impl,
    castore_impl,
    sastore_impl,
    pop_fantasma,
    pop2_fantasma,
    dup_impl,
    dup_x1_impl,
    dup_x2_impl,
    dup2_impl,
    dup2_x1_impl,
    dup2_x2_impl,
    swap_impl,
    iadd_impl,
    ladd_impl,
    fadd_impl,
    dadd_impl,
    isub_impl,
    lsub_impl,
    fsub_impl,
    dsub_impl,
    imul_impl,
    lmul_impl,
    fmul_impl,
    dmul_impl,
    idiv_impl,
    ldiv_impl,
    fdiv_impl,
    ddiv_impl,
    irem_impl,
    lrem_impl,
    frem_impl,
    drem_impl,
    ineg_impl,
    lneg_impl,
    fneg_impl,
    dneg_impl,
    ishl_impl,
    lshl_impl,
    ishr_impl,
    lshr_impl,
    iushr_impl,
    lushr_impl,
    iand_impl,
    land_impl,
    ior_impl,
    lor_impl,
    ixor_impl,
    lxor_impl,
    iinc_fantasma,
    i2l_impl,
    i2f_impl,
    i2d_impl,
    l2i_impl,
    l2f_impl,
    l2d_impl,
    f2i_impl,
    f2l_impl,
    f2d_impl,
    d2i_impl,
    d2l_impl,
    d2f_impl,
    i2b_impl,
    i2c_impl,
    i2s_impl,
    lcmp_impl,
    fcmpl_impl,
    fcmpg_impl,
    dcmpl_impl,
    dcmpg_impl,
    ifeq_impl,
    ifne_impl,
    iflt_impl,
    ifge_impl,
    ifgt_impl,
    ifle_impl,
    if_icmpeq_impl,
    if_icmpne_impl,
    if_icmplt_impl,
    if_icmpge_impl,
    if_icmpgt_impl,
    if_icmple_impl,
    acmpeq_impl,
    acmpne_impl,
    inst_goto_impl,
    jsr_impl,
    ret_impl,
    tableswitch_impl,
    lookupswitch_impl,
    ireturn_impl,
    lreturn_impl,
    freturn_impl,
    dreturn_impl,
    areturn_impl,
    inst_return_impl,
    getstatic_impl,
    putstatic_impl,
    getfield_impl,
    putfield_impl,
    invokevirtual_impl,
    invokespecial_impl,
    invokestatic_impl,
    invokeinterface_fantasma,
    invokedynamic_fantasma,
    inst_new_impl,
    newarray_impl,
    anewarray_impl,
    arraylength_impl,
    athrow_impl,
    checkcast_impl,
    instanceof_impl,
    monitorenter_impl,
    monitorexit_impl,
    wide_impl,
    multianewarray_fantasma,
    ifnull_impl,
    ifnonnull_impl,
    goto_w_impl,
    jsr_w_impl
};

#endif
