#ifndef PILHA_DE_FRAMES_H
#define PILHA_DE_FRAMES_H

#include "ClassFile.h"

#define TAMANHO_PILHA 1000

/**
 * @struct 	stack_frame
 * @brief 	Estrutura de dados da pilha de frames
 */
typedef struct stack_frame {
	i2 top;                     /**< Topo da pilha de frames */
  u2 max_size;                /**< Tamanho máximo da pilha de frames */
  frame *frames;              /**< Frames da pilha */
} stack_frame;

/**
 * @brief 	Inicializa Pilha de Frames
 * @return 	Ponteiro para Pilha de Frames
 */
stack_frame* init_stack_frame();

/**
 * @brief			Adiciona frame ao topo da pilha
 * @param	sf	Ponteiro para a pilha
 * @param	f		Frame a ser adicionado
 */
void push_frame(stack_frame *sf, frame f);

/**
 * @brief			Remove frame do topo da pilha
 * @param	sf	Ponteiro para a pilha
 */
void pop_frame(stack_frame *sf);

/**
 * @brief			Recupera frame do topo da pilha
 * @param	sf	Ponteiro para a pilha
 * @return    Frame do topo da pilha
 */
frame get_top_element(stack_frame *sf);

/**
 * @brief			Verifica se pilha está vazia
 * @param	sf	Ponteiro para a pilha
 * @return		True se pilha está vazia; False, caso contrário.
 */
bool is_empty(stack_frame *sf);

/**
 * @brief			Exibe frames da pilha
 * @param	sf	Ponteiro para a pilha
 */
void print_stack_frame(stack_frame *sf);

/**
 * @brief			Libera espaço alocado para a pilha e seus frames
 * @param	sf	Ponteiro para a pilha
 */
void free_stack_frame(stack_frame *sf);

#endif
