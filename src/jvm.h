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
 * @struct  JVM
 * @brief   Programa que carrega e executa os aplicativos Java, convertendo os bytecodes em código executável de máquina. A JVM é responsável pelo gerenciamento dos aplicativos, à medida que são executados.
 */
typedef struct jvm {
    loaded_class    *classes;           /**< Classes carregadas */
    stack_frame     *frames;            /**< Pilha de Frames da JVM */
    u4              pc;                 /**< Program counter */
    u2              exception;          /**< Exceção */
    object_list     *objects;           /**< Lista de objetos */
    char            *exception_name;    /**< Nome da exceção */
} JVM;

// Variáveis Globais
extern JVM *jvm;
extern instruction *instructions;

/**
 * @brief   Inicializa a estrutura da JVM, realizando sua alocação e setando valores iniciais
 * @return  Estrutura da JVM alocada
 */
JVM *initialize();

/**
 * @brief Executa a JVM, iniciando a interpretação do arquivo .class
 */
void run_jvm();

/**
 * @brief               Executa método da classe
 * @param m             Ponteiro para method_info
 * @param class_info    Nome da classe
 * @param caller        diferenciacao se chamador foi um arquivo da JVM ou instrução invoke
 */
void run_method(method_info *m, char *class_info, int caller);

/**
 * @brief           Interpreta as funções Code de um método
 * @param code      Ponteiro para code que deve ser interpretado
 * @param length    Tamanho do código
 * @param m         Método do code
 */
void interpret_code(u1 *code, u4 length, method_info *m);

/**
 * @brief               Cria e inicializa frame
 * @param class_info    Nome da classe
 * @param max_locals    Tamanho do vetor de variáveis
 * @return Ponteiro para frame criado
 */
frame *create_frame(char *class_info, u2 max_locals);

/**
 * @brief               Libera memória alocada para variáveis locais
 * @param v             Vetor de variáveis
 * @param vector_length Tamanho do vetor de variáveis
 */
void free_local_variable(local_variable *v, u2 vetor_length);

/**
 * @brief               Avalia se a instrução passada como parâmetro é uma instrução do tipo branch
 * @param nomeInstrucao Nome da instrucao
 * @return True, se a instrução é do tipo branch
 */
bool instruction_branch (char *nomeInstrucao);

/**
 * @brief   Encontra em um método o PC do handler da exceção lançada na JVM
 * @param m Ponteiro para método
 * @return PC do método
 */
u2 check_handler_method(method_info *m);

/**
 * @brief   Atualizar o PC, de acordo com a instrução branch previamente executada, verificando sua execução
 * @param code      Ponteiro para code
 * @param length    Tamanho do code
 * @return PC atualizado
 */
u1* update_pc_current_method (u1 *code, u4 length);

#endif
