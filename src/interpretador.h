#ifndef INTERPRETADOR_H
#define INTERPRETADOR_H

#include "instrucoes.h"
#include "ClassFile.h"
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <stdint.h>

#define normalize_index(x,y) (x << 8) | y

/**
 * @brief Função para resolver uma classe, analisar e alocar caso não alocada
 * 
 * @param class_name Nome da classe a ser resolvida
 * @return Ponteiro para a classe resolvida
*/
ClassFile* resolve_class(char *class_name);

/**
 * @brief Fresolve_classesolver método, analisar parâmetros, nome e descriptor
 *
 * @param cp Ponteiro para constant pool
 * @param cp_index Índice a ser acessado na constante pool
 * @param interface Flag para sinalizar método de interface
 * @return Valor booleano representando o sucesso da execução da resolução
 */
bool resolve_method(cp_info *cp, u2 cp_index, u1 interface);

/**
 * @brief Função para obter o nome de um método na constant pool
 *
 * @param cp Ponteiro para constant pool
 * @param cp_index Índice a ser acessado na constante pool
 * @param interface Flag para sinalizar método de interface
 * @return Ponteiro de char representando a string contendo o nome do método
 */
char* get_name_method(cp_info *cp, u2 cp_index, u1 interface);

/**
 * @brief Função para obter o descriptor de um método
 *
 * @param cp Ponteiro para constant pool
 * @param cp_index Índice a ser acessado na constante pool
 * @param interface Flag para sinalizar método de interface
 * @return Ponteiro de char representando a string contendo o descriptor do método
 */
char* get_descriptor_method(cp_info *cp, u2 cp_index, u1 interface);

/**
 * @brief Função para obter o tipo do descriptor do método
 *
 * @param descriptor Ponteiro de char representando a string contendo o descriptor do método
 * @return Valor inteiro representando o tipo do descriptor do método
 */
int descriptor_field_validate (char * descriptor);

/**
 * @brief Função para obter a classe executante de um método
 *
 * @param cp P_fntei_vo para constant pool
 * @param cp_index Índice a ser acessado na constante pool
 *
 * @return Ponteiro de char representando a string contendo o nome da classe de um método
 */
char* get_class_method(cp_info *cp, u2 cp_index);

/**
 * @brief Função para transferir os valores da pilha de operandos de um Frame para o vetor de variáveis locais do próximo Frame
 *
 * @param prev        Frame anterior (previamente executado)
 * @param new_frame   Frame novo sendo alocado para execução de método
 * @param param_count Contador de parâmetros a serem passados para o array de variáveis locais do novo Frame
 * @return Retorna um ponteiro para Frame, com o array de variáveis locais atualizado
 */
frame* transfer_array_stack(frame *prev, frame *new_frame, int *param_count);

/**
 * @brief Nada é executado
 *
 * @param par0 -
 * @param par1 -
 * @param par2 -
 */
void nop_impl(frame *par0, u1 par1, u1 par2);

/**
 * @brief Faz o push de um valor nulo para a pilha de operandos
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void aconst_null_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push de um valor presente na constant pool
 * @param f Ponteiro para o Frame
 * @param indexbyte1 Índice da constant pool
 * @param par2 -
 */
void ldc_impl(frame *f,u1 indexbyte1,u1 par2);

/**
 * @brief [brief description]
 * @details [long description]
 *
 * @param f [description]
 * @param par1 [description]
 * @param par2 [description]
 */
void iconst_m1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 0 \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void iconst_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 1 \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void iconst_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 2 \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void iconst_2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 3 \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void iconst_3_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 4 \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void iconst_4_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 5 \c int
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void iconst_5_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 0 \c long
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lconst_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 1 \c long
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void lconst_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 0 \c float
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void fconst_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 1 \c float
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void fconst_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 2 \c float
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void fconst_2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 0 \c double
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void dconst_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 1 \c double
 *
 * @param f Ponteiro para o Frame
 * @param par1 -
 * @param par2 -
 */
void dconst_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push de um byte na pilha de operandos
 *
 * @param f Ponteiro para o Frame
 * @param byte Byte a ser empilhado
 * @param par1 -
 */
void bipush_impl(frame *f, u1 byte, u1 par1);

/**
 * @brief Faz o push de um short na pilha de operandos
 *
 * @param f Ponteiro para o Frame
 * @param byte Primeiro byte do short
 * @param par1 Segundo byte do short
 */
void sipush_impl(frame *f,u1 byte1, u1 byte2);

#endif
