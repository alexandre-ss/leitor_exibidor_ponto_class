#include "pilha_de_operandos.h"

operand_stack* create_operand_stack(){
	operand_stack *p = malloc(sizeof(struct operand_stack));
	p->top = NULL;
	return(p);
}

operand_stack* push_operand(operand_stack *p, u4 operand, void *reference, u1 operand_type){
	p->top = InserirInicio_operandos(p->top, operand, reference, operand_type);
	return (p);
}

operand_stack* pop_operand(operand_stack *p){
	operand_stack *aux = create_operand_stack();
	aux = push_operand(aux, p->top->operand, p->top->ref, p->top->operand_type);
	p->top = RemoverInicio_operandos(p->top);
	return (aux);
}

bool check_empty_stack(operand_stack *p) {
	return p->top == NULL;
}

bool print_empty(operand_stack *p) {
	int i = 0;
	u1 op_type_aux;
	for (operand_list *lo = p->top; lo != NULL; lo = lo->next) {
		i++;
		op_type_aux = lo->operand_type;
	}

	return (i == 1 && op_type_aux == 10);
}

operand_stack* top_operand(operand_stack *p){
	return(p);
}

void print_operand_stack(operand_stack *p){
	print_operand_list(p->top);
}