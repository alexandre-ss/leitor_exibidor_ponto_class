#ifndef LISTA_OPERANDOS_H
#define LISTA_OPERANDOS_H

#include "ClassFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/**
 * @struct operand_list
 * @brief Estrutura de dados da lista de operandos.
 */
typedef struct operand_list {
	i4 operand;               	/**< Caso o operando seja um valor, é armazenado nesta variável */
	void *ref;    		          /**< Caso o operando seja uma referência, é armazenado nesta variável */
	u1 operand_type; 				    /**< Tipo do operando armazenado na estrutura. A lista de opções está definida na enumeração \c tipos_operandos */
	struct operand_list *next;  /**< Ponteiro de acesso ao próximo nó da lista */
	struct operand_list *prev; 	/**< Ponteiro de acesso ao nó anterior da lista */
} operand_list;

/**
 * @brief		Inicia a lista de operandos.
 * @return      Lista de operandos com valor NULL
 */
operand_list *create_operand_list();

/**
 * @brief					Insere um operando no início da lista.
 * @param 	list 			Ponteiro para a lista de operandos
 * @param 	operand 		Operando a ser inserido na lista
 * @param 	ref 			Operando referenciado a ser inserido na lista
 * @param 	operand_type 	Parâmetro para definir o tipo de operando a ser inserido
 * @return  				Ponteiro para a lista de operandos com a nova inserção
 */
operand_list *insert_operand(operand_list *, i4, void *, u1);

/**
 * @brief					Insere um operando no final da lista de operandos.
 * @param 	list 			Ponteiro para a lista de operandos
 * @param 	operand 		Operando a ser inserido na lista
 * @param 	ref 			Operando referenciado a ser inserido na lista
 * @param 	operand_type 	Parâmetro para definir o tipo de operando a ser inserido
 * @return					Ponteiro para a lista de operandos com a inserção no final da mesma
 */
// operand_list *insert_end_operand(operand_list *, i4, void *, u1);

/**
 * @brief 			Remove o operando que está no inicio da lista.
 * @param 	list 	Ponteiro para a lista de operandos
 * @return			Lista de operandos com o primeiro elemento removido
 */
operand_list *remove_operand(operand_list *);

/**
 * @brief 			Remove o operando que está no final da lista.
 * @param 	list 	Ponteiro para a lista de operandos
 * @return			Lista de operandos com o último elemento removido
 */
operand_list *remove_end_operand(operand_list *);

/**
 * @brief 		Imprime na tela a lista de operandos.
 * @param list 	Ponteiro para a lista de operandos a ser impressa
 */
void print_operand_list(operand_list *);

/**
 * @brief		Libera a memória utilizada pela lista de operandos.
 * @param list 	Ponteiro para a lista de operandos a ser liberada
 */
void free_operand_list(operand_list *);

#endif
