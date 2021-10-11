#include "pilha_de_frames.h"

stack_frame* init_stack_frame() {
  stack_frame *sf = malloc(sizeof(stack_frame));
  sf->top = -1;
  sf->max_size = TAMANHO_PILHA;
  sf->frames = (frame*) malloc(TAMANHO_PILHA * sizeof(frame));

  return sf;
}

void push_frame(stack_frame *sf, frame frame) {
  if (sf->top == sf->max_size - 1) {
    printf("Pilha de Frames cheia!");
    return;
  }

  sf->frames[++sf->top] = frame;
}

void pop_frame(stack_frame *sf) {
  --sf->top;
}

bool is_empty(stack_frame *sf) {
  sf->top == -1;
}

// TODO
// void print_stack_frame(stack_frame *sf) {
//   lista_operandos *p;

// 	for(p=lis;p!=NULL;p=p->prox){
// 		printf("Tipo: %d\t",p->tipo_operando);
// 		if(p->tipo_operando<=8){
// 			printf("Operando: 0x%08x\n\n",p->operando);
// 		}
// 		else{
// 			switch(p->tipo_operando){
// 				case RETURN_ADDRESS_OP:
// 					printf("Operando: %s\n\n",(char*) p->referencia);
// 				break;
// 				case REFERENCE_ARRAY_BOOLEAN_OP:
// 					printf("[Z@%p\n",(u4*) p->referencia);
// 				break;
// 				case REFERENCE_ARRAY_CHAR_OP:
// 					printf("Operando: %s\n\n",(char*) p->referencia);
// 				break;
// 				case REFERENCE_ARRAY_FLOAT_OP:
// 					printf("[F@%p\n",(u4*) p->referencia);
// 				break;
// 				case REFERENCE_ARRAY_DOUBLE_OP:
// 					printf("[D@%p\n",(u4*) p->referencia);
// 				break;
// 				case REFERENCE_ARRAY_BYTE_OP:
// 					printf("[B@%p\n",(u4*) p->referencia);
// 				break;
// 				case REFERENCE_ARRAY_SHORT_OP:
// 					printf("[S@%p\n",(u4*) p->referencia);
// 				break;
// 				case REFERENCE_ARRAY_INT_OP:
// 					printf("[I@%p\n",(u4*) p->referencia);
// 				break;
// 				case REFERENCE_ARRAY_LONG_OP:
// 					printf("[J@%p\n",(u4*) p->referencia);
// 				break;
// 				case REFERENCE_STRING_OP:
// 					printf("String: %s\n", (char *) p->referencia);
// 				break;
// 				case REFERENCE_OP:
// 					printf("Operando: 0x%p\n\n", (u4*) p->referencia);
// 				break;
// 			}
// 		}
// 	}
// }
