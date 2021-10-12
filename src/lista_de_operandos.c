#include "lista_de_operandos.h"

operand_list *create_operand_list(){
    return NULL;
}

operand_list *insert_operand(operand_list *list, i4 operand, void *ref, u1 operand_type){
    operand_list *new = malloc (sizeof(operand_list));

    if(operand_type <= 8){
        new->operand = operand;
        new->ref = NULL;
    }
    else{
        new->operand = -INT_MAX;
        new->ref = ref;
    }

    new->operand_type = operand_type;
    new->next = list;
    new->prev = NULL;

    if(list != NULL){
        list->prev = new;
    }
    return new;
}

// operand_list *insert_end_operand(operand_list *list, i4 operand, void *ref, u1 operand_type){
// 	operand_list *prev = NULL, *pointer = list;

// 	while(pointer != NULL){
// 		prev = pointer;
// 		pointer = pointer->next;
// 	}
// }

operand_list *remove_operand(operand_list *list){
	operand_list *pointer = list;

	list = pointer->next;
	if(list != NULL){
		list->prev = NULL;
	}

	free(pointer);
	return (list);
}

// operand_list* remove_end_operand(operand_list *list){
// 	operand_list *pointer = list, *prev = NULL;

// 	while (pointer!= NULL){
// 		prev = pointer;
// 		pointer = pointer->next;
// 	}

// 	prev->prev->next = NULL;

// 	free(prev);

// 	return list;
// }

void print_operand_list(operand_list *list){
	operand_list *pointer;

	for(pointer = list; pointer != NULL; pointer = pointer->next){
		printf("Tipo: %d\t",pointer->operand_type);
		if(pointer->operand_type <= 8){
			printf("Operando: 0x%08x\n\n",pointer->operand);
		}
		else{
			switch(pointer->operand_type){
				case RETURN_ADDRESS_OP:
					printf("Operando: %s\n\n",(char*)pointer->ref);
				break;
				case REFERENCE_ARRAY_BOOLEAN_OP:
					printf("[Z@%p\n",(u4*) pointer->ref);
				break;
				case REFERENCE_ARRAY_CHAR_OP:
					printf("Operando: %s\n\n",(char*) pointer->ref);
				break;
				case REFERENCE_ARRAY_FLOAT_OP:
					printf("[F@%p\n",(u4*) pointer->ref);
				break;
				case REFERENCE_ARRAY_DOUBLE_OP:
					printf("[D@%p\n",(u4*) pointer->ref);
				break;
				case REFERENCE_ARRAY_BYTE_OP:
					printf("[B@%p\n",(u4*) pointer->ref);
				break;
				case REFERENCE_ARRAY_SHORT_OP:
					printf("[S@%p\n",(u4*) pointer->ref);
				break;
				case REFERENCE_ARRAY_INT_OP:
					printf("[I@%p\n",(u4*) pointer->ref);
				break;
				case REFERENCE_ARRAY_LONG_OP:
					printf("[J@%p\n",(u4*) pointer->ref);
				break;
				case REFERENCE_STRING_OP:
					printf("String: %s\n", (char *) pointer->ref);
				break;
				case REFERENCE_OP:
					printf("Operando: 0x%p\n\n", (u4*) pointer->ref);
				break;
			}
		}
	}
}

void free_operand_list(operand_list *list){
	operand_list *pointer = list;
	while(pointer != NULL){
		operand_list *aux = pointer->next;
		free(pointer);
		pointer = aux;
	}
}
