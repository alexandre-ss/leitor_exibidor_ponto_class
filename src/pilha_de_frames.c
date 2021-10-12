#include "pilha_de_frames.h"

stack_frame* init_stack_frame() {
  stack_frame *sf = malloc(sizeof(stack_frame));
  sf->top = -1;
  sf->max_size = TAMANHO_PILHA;
  sf->frames = (frame**) malloc(TAMANHO_PILHA * sizeof(frame*));

  return sf;
}

void push_frame(stack_frame *sf, frame *frame) {
  if (sf->top == sf->max_size - 1) {
    printf("Pilha de Frames cheia!");
    return;
  }

  sf->frames[++sf->top] = frame;
}

void pop_frame(stack_frame *sf) {
  --sf->top;
}

frame* get_top_element(stack_frame *sf) {
  return sf->frames[sf->top - 1];
}

bool is_empty(stack_frame *sf) {
  return sf->top == -1;
}

void print_stack_frame(stack_frame *sf) {
  printf("----- Pilha de Frames\n\n");
	for(u2 i = sf->top - 1; i >= 0; i--) {
    printf("----- Frame [%d]\n", i+1);
		printf("Endereco retorno: %04x\n", sf->frames[i]->return_address);
	}
}

void free_stack_frame(stack_frame *sf) {
  free(sf->frames);
  free(sf);
}
