#ifndef PILHA_DE_FRAMES_H
#define PILHA_DE_FRAMES_H

#include "ClassFile.h"

#define TAMANHO_PILHA 1000

/**
 * Estrutura de dados da pilha de frames
 */
typedef struct stack_frame {
	i2 top;                     /**< Topo da pilha de frames */
  u2 max_size;                /**< Tamanho máximo da pilha de frames */
  frame *frames;              /**< Frames da pilha */
} stack_frame;

typedef enum operand_types {
	BOOLEAN_OP = 1,
	BYTE_OP,
	CHAR_OP,
	SHORT_OP,
	INTEGER_OP,
	FLOAT_OP,
	LONG_OP,
	DOUBLE_OP,
	RETURN_ADDRESS_OP,
	REFERENCE_OP,
	REFERENCE_ARRAY_BOOLEAN_OP,
	REFERENCE_ARRAY_CHAR_OP,
	REFERENCE_ARRAY_FLOAT_OP,
	REFERENCE_ARRAY_DOUBLE_OP,
	REFERENCE_ARRAY_BYTE_OP,
	REFERENCE_ARRAY_SHORT_OP,
	REFERENCE_ARRAY_INT_OP,
	REFERENCE_ARRAY_LONG_OP,
	REFERENCE_STRING_OP,
} operand_types;

/**
 * Inicializa Pilha de Frames
 * @return Ponteiro para Pilha de Frames
 */
stack_frame* init_stack_frame();
void push_frame(stack_frame *sf, frame f);
void pop_frame(stack_frame *sf);
bool is_empty(stack_frame *sf);
void print_stack_frame(stack_frame *sf);  


#endif
