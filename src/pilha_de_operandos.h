#ifndef PILHA_DE_OPERANDOS_H
#define PILHA_DE_OPERANDOS_H

#include "ClassFile.h"
#include "lista_de_operandos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief 	Inicializa pilha de operandos.
 * @return	Ponteiro para pilha de operandos
 */
operand_stack* create_operand_stack();

/**
 * @brief 							Insere operando à pilha de operandos.
 * @param	p							Ponteiro para pilha de operandos
 * @param operand				Operando de valor direto a ser inserido na pilha
 * @param	reference			Operando de referência a ser inserido na lista
 * @param	operand_type	Tipo do operando a ser inserido
 * @return	Ponteiro para pilha de operandos
 */
operand_stack* push_operand(operand_stack *p, u4 operand, void *reference, u1 operand_type);

/**
 * @brief 		Remove operando da pilha de operandos.
 * @param		p	Ponteiro para pilha de operandos
 * @return		Ponteiro para pilha de operandos
 */
operand_stack* pop_operand(operand_stack *p);

/**
 * @brief 		Verifica se pilha de operandos está vazia.
 * @param		p	Ponteiro para pilha de operandos
 * @return		True, se pilha estiver vazia; False, caso contrário.
 */
bool check_empty_stack(operand_stack *p);

/**
 * @brief 		Verifica se campo no topo da pilha de operandos é uma quebra de linha
 * @param		p	Ponteiro para pilha de operandos
 * @return		True, se for quebra de linha; False, caso contrário.
 */
bool print_empty(operand_stack *p);

/**
 * @brief 		Recupera topo da pilha.
 * @param		p	Ponteiro para pilha de operandos
 * @return		Ponteiro para topo da pilha de operandos.
 */
operand_stack* top_operand(operand_stack *p);

/**
 * @brief 		Exibe valores da pilha de operandos.
 * @param		p	Ponteiro para pilha de operandos
 */
void print_operand_stack(operand_stack *p);

#endif
