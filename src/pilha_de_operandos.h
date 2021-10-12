#ifndef PILHA_DE_OPERANDOS_H
#define PILHA_DE_OPERANDOS_H

#include "lista_de_operandos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef TIPOS_U
#define TIPOS_U
typedef uint8_t u1; 	/**< Tipo \c unsigned \c int de 8 bits */
typedef uint16_t u2; 	/**< Tipo \c unsigned \c int de 16 bits */
typedef uint32_t u4; 	/**< Tipo \c unsigned \c int de 32 bits */
typedef uint64_t u8; 	/**< Tipo \c unsigned \c int de 64 bits */
#endif

/**
 * @struct operand_stack
 * @brief Estrutura de dados da pilha de operandos.
 * @see operand_list
 */
typedef struct operand_stack {
	struct operand_list *top; /**< Ponteiro para o topo da pilha de operandos */
} operand_stack;

#include "pilha_de_operandos.h"

operand_stack* create_operand_stack();
operand_stack* push_operand(operand_stack *p, u4 operand, void *reference, u1 operand_type);
operand_stack* pop_operand(operand_stack *p);
bool check_empty_stack(operand_stack *p);
bool print_empty(operand_stack *p);
operand_stack* top_operand(operand_stack *p);
void print_operand_stack(operand_stack *p);

#endif
