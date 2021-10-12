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
 * @brief Função para transferir os valores da pilha de operandos de um frame para o vetor de variáveis locais do próximo frame
 *
 * @param prev        frame anterior (previamente executado)
 * @param new_frame   frame novo sendo alocado para execução de método
 * @param param_count Contador de parâmetros a serem passados para o array de variáveis locais do novo frame
 * @return Retorna um ponteiro para frame, com o array de variáveis locais atualizado
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
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void aconst_null_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push de um valor presente na constant pool
 * @param f Ponteiro para o frame
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
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void iconst_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 1 \c int
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void iconst_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 2 \c int
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void iconst_2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 3 \c int
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void iconst_3_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 4 \c int
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void iconst_4_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 5 \c int
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void iconst_5_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 0 \c long
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void lconst_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 1 \c long
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void lconst_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 0 \c float
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void fconst_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 1 \c float
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void fconst_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 2 \c float
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void fconst_2_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 0 \c double
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void dconst_0_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push para a pilha de operando da constante 1 \c double
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void dconst_1_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Faz o push de um byte na pilha de operandos
 *
 * @param f Ponteiro para o frame
 * @param byte Byte a ser empilhado
 * @param par1 -
 */
void bipush_impl(frame *f, u1 byte, u1 par1);

/**
 * @brief Faz o push de um short na pilha de operandos
 *
 * @param f Ponteiro para o frame
 * @param byte Primeiro byte do short
 * @param par1 Segundo byte do short
 */
void sipush_impl(frame *f,u1 byte1, u1 byte2);

/**
 * @brief Faz o push de um valor presente na constant pool
 * @param f Ponteiro para o frame
 * @param indexbyte1 Índice da constant pool (primeira parte)
 * @param indexbyte2 Índice da constant pool (segunda parte)
 */
void ldc_w_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Faz o push de um valor long ou double presente na constant pool
 * @param f Ponteiro para o frame
 * @param branchbyte1 Índice da constant pool (primeira parte)
 * @param branchbyte2 Índice da constant pool (segunda parte)
 */
void ldc2_w_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Carrega um valor \c int do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param index Índice no vetor de variáveis locais
 * @param par1 -
 */
void iload_impl(frame *f, u1 index, u1 par1);

/**
 * @brief Carrega um valor \c long do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param index Índice no vetor de variáveis locais
 * @param par1 -
 */
void lload_impl(frame *f, u1 index, u1 par1);

/**
 * @brief Carrega um valor \c float do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param index Índice no vetor de variáveis locais
 * @param par1 -
 */
void fload_impl(frame *f, u1 index, u1 par1);

/**
 * @brief Carrega um valor \c double do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param index Índice no vetor de variáveis locais
 * @param par1 -
 */
void dload_impl(frame *par0, u1 par1,u1 par2);

/**
 * @brief Carrega uma referência do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param index Índice no vetor de variáveis locais
 * @param par1 -
 */
void aload_impl(frame *f, u1 index, u1 par1);

/**
 * @brief Carrega um valor \c int da posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void iload_0_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c int da posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void iload_1_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c int da posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void iload_2_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c int da posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void iload_3_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c long da posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void lload_0_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c long da posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void lload_1_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c long da posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void lload_2_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c long da posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void lload_3_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c float da posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void fload_0_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c float da posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void fload_1_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c float da posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void fload_2_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c float da posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void fload_3_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c double da posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void dload_0_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c double da posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void dload_1_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c double da posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void dload_2_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um valor \c double da posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void dload_3_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega uma referência da posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void aload_0_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega uma referência da posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void aload_1_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega uma referência da posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void aload_2_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega uma referência da posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par2 -
 * @param par1 -
 */
void aload_3_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um \c int de um array
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void iaload_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um \c long de um array
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void laload_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um \c float de um array
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void faload_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um \c double de um array
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void daload_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega uma referência de um array
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void aaload_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um \c byte de um array
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void baload_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um \c char de um array
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void caload_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Carrega um \c short de um array
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void saload_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c int no vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param index Índice do vetor de variáveis locais
 */
void istore_impl(frame *f, u1 index, u1 par1);
/**
 * @brief Armazena um \c long no vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param index Índice do vetor de variáveis locais
 */
void lstore_impl(frame *f, u1 index, u1 par1);
/**
 * @brief Armazena um \c float no vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param index Índice do vetor de variáveis locais
 */
void fstore_impl(frame *f, u1 index, u1 par1);
/**
 * @brief Armazena um \c double no vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param index Índice do vetor de variáveis locais
 */
void dstore_impl(frame *f, u1 index, u1 par1);
/**
 * @brief Armazena uma referência no vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param index Índice do vetor de variáveis locais
 */
void astore_impl(frame *f,u1 index, u1 par1);
/**
 * @brief Armazena um \c int na posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void istore_0_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c int na posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void istore_1_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c int na posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void istore_2_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c int na posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void istore_3_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c long na posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void lstore_0_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c long na posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void lstore_1_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c long na posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void lstore_2_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c long na posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void lstore_3_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c float na posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void fstore_0_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c float na posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void fstore_1_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c float na posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void fstore_2_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c float na posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void fstore_3_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c double na posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void dstore_0_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c double na posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void dstore_1_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c double na posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void dstore_2_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um \c double na posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void dstore_3_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena uma referência na posição 0 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void astore_0_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena uma referência na posição 1 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void astore_1_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena uma referência na posição 2 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void astore_2_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena uma referência na posição 3 do vetor de variáveis locais
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void astore_3_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um valor \c int em um array
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void iastore_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um valor \c long em um array
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void lastore_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um valor \c float em um array
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void fastore_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um valor \c double em um array
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void dastore_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena uma referência em um array
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void aastore_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um valor \c byte em um array
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void bastore_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um valor \c char em um array
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void castore_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Armazena um valor \c short em um array
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void sastore_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Remove o valor de topo da pilha de operandos
 *
 * @param f Ponteiro para o frame
 */
operand_stack* pop_impl(frame *f);
/**
 * @brief Remove os dois valores de topo da pilha de operandos
 *
 * @param f Ponteiro para o frame
 */
operand_stack** pop2_impl(frame *f);
/**
 * @brief PlaceHolder para ocupar posicao no vetor de posicoes da instrucao pop
 * @details [long description]
 *
 * @param par0 frame.
 * @param par1 -
 * @param par2 -
 */
void pop_ghost(frame *par0, u1 par1, u1 par2);
/**
 * @brief PlaceHolder para ocupar posicao no vetor de posicoes da instrucao pop2
 * @details [long description]
 *
 * @param par0 frame.
 * @param par1 -
 * @param par2 -
 */
void pop2_ghost(frame *par0, u1 par1, u1 par2);
/**
 * @brief Duplica o valor no topo da pilha de operandos.
 * @details [long description]
 *
 * @param f frame.
 * @param par1 -
 * @param par2 -
 */
void dup_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Duplica o valor no topo da pilha de operandos e insere dois valores a baixo.
 * @details [long description]
 *
 * @param f frame.
 * @param par1 -
 * @param par2 -
 */
void dup_x1_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Duplica o valor no topo da pilha de operandos e insere dois ou três valores a baixo.
 * @details [long description]
 *
 * @param f frame.
 * @param par1 -
 * @param par2 -
 */
void dup_x2_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Duplica o primeiro ou o segundo valor no topo da pilha de operandos e insere logo abaixo.
 * @details [long description]
 *
 * @param f frame.
 * @param par1 -
 * @param par2 -
 */
void dup2_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Duplica o primeiro ou o segundo valor no topo da pilha de operandos e insere dois valores a baixo.
 * @details [long description]
 *
 * @param f frame.
 * @param par1 -
 * @param par2 -
 */
void dup2_x1_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Duplica o primeiro ou o segundo valor no topo da pilha de operandos e insere dois ou três valores a baixo.
 * @details [long description]
 *
 * @param f frame.
 * @param par1 -
 * @param par2 -
 */
void dup2_x2_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Inverte os dois operandos do topo da pilha
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void swap_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma soma de \c int
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void iadd_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma soma de \c long
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void ladd_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma soma de \c float
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void fadd_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma soma de \c double
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void dadd_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma subtração de \c int
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void isub_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma subtração de \c long
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void lsub_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma subtração de \c float
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void fsub_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma subtração de \c double
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void dsub_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma multiplicação de \c int
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void imul_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma multiplicação de \c long
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void lmul_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma multiplicação de \c float
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void fmul_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma multiplicação de \c double
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void dmul_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma divisão de \c int
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void idiv_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma divisão de \c long
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void ldiv_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma divisão de \c float
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void fdiv_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa uma divisão de \c double
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void ddiv_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Calcula o resto \c int da divisão entre dois operandos
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void irem_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Calcula o resto \c long da divisão entre dois operandos
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void lrem_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Calcula o resto \c float da divisão entre dois operandos
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void frem_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Calcula o resto \c double da divisão entre dois operandos
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void drem_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Inverte o sinal de um \c int
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void ineg_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Inverte o sinal de um \c long
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void lneg_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Inverte o sinal de um \c float
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void fneg_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Inverte o sinal de um \c double
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void dneg_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa um shift left em um \c int
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void ishl_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa um shift left em um \c long
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void lshl_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa um shift right em um \c int
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void ishr_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa um shift right em um \c long
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void lshr_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa um shift right lógico em um \c int
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void iushr_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa um shift right lógico em um \c long
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void lushr_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa a operação "AND" de dois \c int
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void iand_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa a operação "AND" de dois \c long
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void land_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa a operação "OR" de dois \c int
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void ior_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa a operação "OR" de dois \c long
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void lor_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa a operação "XOR" de dois \c int
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void ixor_impl(frame *f, u1 par1, u1 par2);
/**
 * @brief Executa a operação "XOR" de dois \c long
 *
 * @param f Ponteiro para o frame
 * @param par1 -
 * @param par2 -
 */
void lxor_impl(frame *f, u1 par1, u1 par2);

void iinc_ghost(frame *par0, u1 par1, u1 par2);

void iinc_impl(frame *f, u1 index, i1 constante);

void iinc_wide_ghost(frame *f, u1 indexbyte1, u1 indexbyte2, u1 constbyte1, u1 constbyte2);

void iinc_wide(frame *f, u2 indexbyte, i2 constbyte);

void i2l_impl(frame *f, u1 par1, u1 par2);

void i2f_impl(frame *f, u1 par1, u1 par2);

void i2d_impl(frame *f, u1 par1, u1 par2);

void l2i_impl(frame *f, u1 par1, u1 par2);

void l2f_impl(frame *f, u1 par1, u1 par2);

void l2d_impl(frame *f, u1 par1, u1 par2);

void f2i_impl(frame *f, u1 par1, u1 par2);

void f2l_impl(frame *f, u1 par1, u1 par2);

void f2d_impl(frame *f, u1 par1, u1 par2);

void d2i_impl(frame *f, u1 par1, u1 par2);

void d2l_impl(frame *f, u1 par1, u1 par2);

void d2f_impl(frame *f, u1 par1, u1 par2);

void i2b_impl(frame *f, u1 par1, u1 par2);

void i2c_impl(frame *f, u1 par1, u1 par2);

void i2s_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valores long oriundos da pilha de operandos
 *
 * @param f frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void lcmp_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valores float oriundos da pilha de operandos
 *
 * @param f frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fcmpl_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valores float oriundos da pilha de operandos
 *
 * @param f frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void fcmpg_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valores double oriundos da pilha de operandos
 *
 * @param f frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dcmpl_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valores double oriundos da pilha de operandos
 *
 * @param f frame do método corrente que está executando a instrução atual
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void dcmpg_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que compara valor da pilha de operandos com zero, verificando se são iguais
 *
 * @param f frame do método corrente que está executando a instrução atual
 * @param branchbyte1 Byte high para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 * @param branchbyte2 Byte low para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 */
void ifeq_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Instrução que compara valor da pilha de operandos com zero, verificando se são diferentes
 *
 * @param f frame do método corrente que está executando a instrução atual
 * @param branchbyte1 Byte high para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 * @param branchbyte2 Byte low para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 */
void ifne_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Instrução que compara valor da pilha de operandos com zero, verificando se o valor da pilha de operandos é menor que zero
 *
 * @param f frame do método corrente que está executando a instrução atual
 * @param branchbyte1 Byte high para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 * @param branchbyte2 Byte low para formar o branch em caso de sucesso na comparação do valor da pilha de operandos com zero
 */
void iflt_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for maior ou igual ao valor zero
 *
 * @param f frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void ifge_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for maior do que o valor zero
 *
 * @param f frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void ifgt_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for menor do que o valor zero
 *
 * @param f frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void ifle_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for igual ao próximo inteiro na pilha
 *
 * @param f frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmpeq_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for diferente do próximo inteiro na pilha
 *
 * @param f frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmpne_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for maior do que o próximo inteiro na pilha
 *
 * @param f frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmplt_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for menor do que o próximo inteiro na pilha
 *
 * @param f frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmpge_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for menor do que o próximo inteiro na pilha
 *
 * @param f frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmpgt_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se o inteiro no topo da pilha for maior do que o próximo inteiro na pilha
 *
 * @param f frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void if_icmple_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se as referências na pilha forem iguais
 *
 * @param f frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void acmpeq_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Branch será efetuado se as referências na pilha não forem iguais
 *
 * @param f frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o índice do branch
 * @param branchbyte2 Byte que será concatenado para montar o índice do branch
 */
void acmpne_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief A instrução sempre efetuará um branch
 *
 * @param f frame corrente
 * @param branchbyte1 Parâmetro que será concatenado para montar o índice do branch
 * @param branchbyte2 Parâmetro que será concatenado para montar o índice do branch
 */
void inst_goto_impl(frame *f,u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Instrução para efetuar um branch para um offset, dentro do código do mesmo método sendo executado
 *
 * @param f frame corrente
 * @param branchbyte1 Parâmetro que será concatenado para montar o índice do branch
 * @param branchbyte2 Parâmetro que será concatenado para montar o índice do branch
 */
void jsr_impl(frame *f,u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Instrução para retornar de uma subrotina
 *
 * @param f frame corrente
 * @param index Índice no vetor de variáveis locais do frame corrente
 * @param par1 Esse parâmetro não será utilizado na prática
 */
void ret_impl(frame *f, u1 index, u1 par1);

/**
 * @brief Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 *
 * @param par0 Esse parâmetro não será utilizado na prática
 * @param par1 Esse parâmetro não será utilizado na prática
 * @param par2 Esse parâmetro não será utilizado na prática
 */
void tableswitch_ghost(frame *par0, u1 par1, u1 par2);

//void tableswitchundefined3_impl(frame *par0, u1 par1, u1 par2);

/**
 * @brief Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 *
 * @param par0 Esse parâmetro não será utilizado na prática
 * @param par1 Esse parâmetro não será utilizado na prática
 * @param par2 Esse parâmetro não será utilizado na prática
 */
void lookupswitch_ghost(frame *par0, u1 par1, u1 par2);

//void lookupswitchundefined3_impl(frame *par0, u1 par1, u1 par2);

/**
 * @brief Instrução para acessar uma jump table por index e efetuar um jump
 *
 * @param f frame corrente
 */
// void tableswitch_impl(frame *f);
void tableswitch_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para acessar uma chave em um jump table e efetuar um jump
 *
 * @param f frame corrente
 */
// void lookupswitch_impl(frame *f);
void lookupswitch_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para retornar um inteiro de um método
 *
 * @param f frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void ireturn_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para retornar um long de um método
 *
 * @param f frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void lreturn_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para retornar um float de um método
 *
 * @param f frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void freturn_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para retornar um double de um método
 *
 * @param f frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void dreturn_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para retornar uma referência de um método
 *
 * @param f frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void areturn_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para retornar void de um método
 *
 * @param f frame corrente
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void inst_return_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para obter um field estático de uma classe
 *
 * @param f frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void getstatic_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução para setar um field estático em uma classe
 *
 * @param f frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void putstatic_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Verifica se flag STATIC está presente em accessFlags
 *
 * @param accessFlags Flags de acesso
 * @return True, se flag STATIC está presente em Access Flags
 */
bool static_flag_find(char *accessFlags);

/**
 * @brief Instrução para obter o field de um objeto
 *
 * @param f frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void getfield_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução para obter o tipo do operando a ser alocado na pilha de operandos
 *
 * @param descriptor_ret Ponteiro de char contendo a string que representa o descriptor do método
 * @return Inteiro com o tipo do operando a ser alocado na pilha de operandos
 */
int get_out_operand_type(char *descriptor_ret);


/**
 * @brief Instrução para obter o modo da pilha do valor de retorno de um método
 *
 * @param descriptor Ponteiro de char com uma string para o descriptor do método
 * @return Inteiro com o modo da pilha do valor de retorno de um método na pilha de operandos
 */
int get_return_type(char *descriptor);

/**
 * @brief Instrução para setar um field em um objeto
 *
 * @param f frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void putfield_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução para buscar um objeto na lista de objetos
 *
 * @param p Ponteiro de classe com a referência do objeto
 * @return Retorna um ponteiro para a lista de objetos
 */
object_list *search_object_by_reference(ClassFile *p);

/**
 * @brief Função para invocar um método de instância, geralmente utilizada para impressão em tela
 *
 * @param f frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void invokevirtual_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução para invocar um método de instância
 *
 * @param f frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void invokespecial_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução para invocar um método estático (método de classe)
 *
 * @param f frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void invokestatic_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 *
 * @param f Esse parâmetro não será utilizado na prática
 * @param par1 Esse parâmetro não será utilizado na prática
 * @param par2 Esse parâmetro não será utilizado na prática
 */
void invokeinterface_ghost(frame *par0, u1 par1, u1 par2);

/**
 * @brief Instrução para invocar um método de interface
 *
 * @param f frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 * @param count Um byte que não pode ser zero
 */
void invokeinterface_impl(frame *f, u1 indexbyte1, u1 indexbyte2, u1 count);

/**
 * @brief Instrução para invocar um método dinâmico. Essa instrução só existe para criar uma posição "falsa" no vetor de ponteiros de função
 * @details [long description]
 *
 * @param par0 Parâmetro não utilizado
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void invokedynamic_ghost(frame *par0, u1 par1, u1 par2);

/**
 * @brief Instrução para criar um novo objeto
 *
 * @param f frame corrente
 * @param indexbyte1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param indexbyte2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void inst_new_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Função para instanciar um objeto novo
 *
 * @param class_name Ponteiro para char contendo a string do nome da classe
 * @return Ponteiro para o objeto alocado
 */
ClassFile *instantiate_class (char *class_name);

/**
 * @brief Função para inserir um objeto novo na lista de objetos
 *
 * @param lis Ponteiro para o começo da lista de objetos
 * @param class Referência para a classe (objeto) a ser alocada na lista
 * @param not_static_paramether Quantidade de parâmetros que não são static
 * @return Ponteiro para a lista de objetos atualizado com a inserção do novo objeto
 */
object_list* insert_object(object_list *lis, ClassFile *class, int not_static_paramether);
/**
 * @brief Função para criar um novo array de um dado tipo
 *
 * @param f frame corrente
 * @param atype Tipo do array que deve ser criado
 * @param par1 Parâmetro não utilizado
 */
void newarray_impl(frame *f ,u1 atype, u1 par1);

/**
 * @brief Criar um novo array do tipo referência
 * @details [long description]
 *
 * @param f frame corrente
 * @param par1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param par2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void anewarray_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para obter o tamanho de um array
 *
 * @param f frame corrente
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void arraylength_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para lançar uma exceção ou erro
 *
 * @param f frame corrente
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void athrow_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para verificar se um objeto é de um determinado tipo
 *
 * @param f frame corrente
 * @param par1 Byte que será concatenado para acessar uma entrada na constant pool
 * @param par2 Byte que será concatenado para acessar uma entrada na constant pool
 */
void checkcast_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para determinar se um objeto é de um eterminado tipo
 *
 * @param f frame corrente
 * @param par1 Byte que será concatenado para acessar uma entrada na constant pool
 *
 * @param par2 Byte que será concatenado para acessar uma entrada na constant pool
 *
 */
void instanceof_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para entrar no monitor de um objeto
 *
 * @param f frame corrente
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void monitorenter_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para sair de um monitor de um objeto
 *
 * @param f frame corrente
 * @param par1 Parâmetro não utilizado
 * @param par2 Parâmetro não utilizado
 */
void monitorexit_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para estender a próxima instrução para ter o dobro de bytes de índice.
 *
 * @param f Parâmetro não utilizado
 * @param indexbyte1 Parâmetro não utilizado
 * @param indexbyte2 Parâmetro não utilizado
 */
void wide_impl(frame *f, u1 indexbyte1, u1 indexbyte2);

/**
 * @brief Instrução que irá criar um array multidimensional
 *
 * @param f frame corrente
 * @param indexbyte1 Byte que será concatenado para montar um índice para a constant pool da classe corrente
 * @param indexbyte2 Byte que será concatenado para montar um índice para a constant pool da classe corrente
 * @param dimensions Número de dimensões do array
 */
void multianewarray_impl(frame *f, u1 indexbyte1, u1 indexbyte2, u1 dimensions);

/**
 * @brief Instrução utilizada como placeholder, para ocupar a posição no array de instruções, para não prejudicar o mapeamento direto
 *
 * @param f Esse parâmetro não será utilizado na prática
 * @param par1 Esse parâmetro não será utilizado na prática
 * @param par2 Esse parâmetro não será utilizado na prática
 */
void multianewarray_ghost(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução que irá efetuar um branch caso a referência no topo da pilha seja nula
 *
 * @param f frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o offset do branch
 * @param branchbyte2 Byte que será concatenado para montar o offset do branch
 */
void ifnull_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief Instrução que irá efetuar um branch caso a referência no topo da pilha não seja nula
 *
 * @param f frame corrente
 * @param branchbyte1 Byte que será concatenado para montar o offset do branch
 * @param branchbyte2 Byte que será concatenado para montar o offset do branch
 */
void ifnonnull_impl(frame *f, u1 branchbyte1, u1 branchbyte2);

/**
 * @brief A instrução sempre efetuará um branch, para um offset wide
 *
 * @param f frame corrente
 * @param par1 Parâmetro que será concatenado para montar o índice do branch
 * @param par2 Parâmetro que será concatenado para montar o índice do branch
 */
void goto_w_impl(frame *f, u1 par1, u1 par2);

/**
 * @brief Instrução para efetuar um branch para um offset wide, de 32 bits, dentro do código do mesmo método sendo executado
 *
 * @param f frame corrente
 * @param par1 Parâmetro que será concatenado para montar o índice do branch
 * @param par2 Parâmetro que será concatenado para montar o índice do branch
 */
void jsr_w_impl(frame *f, u1 par1, u1 par2);

extern void (*func_ptr[202])(frame *,u1,u1);

double decode_double_value(u4 high, u4 low);

long decode_long_value(u4 high, u4 low);

float decode_float_value(u4 value);

int get_not_static_params(ClassFile *);

int get_param_count(char *);

int get_field_position(ClassFile *, char *);

#endif
