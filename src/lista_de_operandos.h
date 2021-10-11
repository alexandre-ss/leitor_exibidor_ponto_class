#ifndef LISTA_OPERANDOS_h
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

#endif
