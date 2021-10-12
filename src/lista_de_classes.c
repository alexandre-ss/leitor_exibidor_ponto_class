#include "lista_de_classes.h"

loaded_class *create_class_list() {
    return NULL;
}

loaded_class *insert_class_element(loaded_class *list, ClassFile *class_file){
    loaded_class *new;
    loaded_class *prev = NULL;
    loaded_class *pointer = list;

    while(pointer != NULL){
        prev = pointer;
        pointer = pointer->next;
    }

    new = (loaded_class *) malloc (sizeof(loaded_class));

    new->class_file = class_file;
    new->next = NULL;
    new->prev = prev;

    if( prev != NULL){
        prev->next = new;
        return list;
    }
    else{
        return new;
    }
}

loaded_class *remove_last_class_element(loaded_class *list){
    loaded_class *pointer = list, *prev = NULL;
    while (pointer != NULL)
    {
        prev = pointer;
        pointer = pointer->next;
    }

    if(prev != NULL){
        if(prev->prev != NULL){
            prev->prev->next = NULL;
        }
        else{
            free(prev);
            return NULL;
        }
    }
    free(prev);
    return list;
}

loaded_class *remove_class_element(loaded_class *list, char *word){
    loaded_class *pointer = find_class_element(list, word);

    if(pointer == NULL) return list;

    if(list == pointer){
        list = pointer->next;
    }
    else{
        pointer->prev->next = pointer->next;
    }

    if(pointer->next != NULL){
        pointer->next->prev = pointer->prev;
    }

    free(pointer);
    return list;
}

loaded_class *find_class_element(loaded_class *list, char *word){
    loaded_class *pointer;

    for(pointer = list; pointer != NULL; pointer = pointer->next){
        if(strcmp(decode_name_index_and_type(pointer->class_file->constant_pool, pointer->class_file->this_class, NAME_INDEX), word) == 0) return pointer;
    }

    return NULL;
}

cp_info *find_class_constant_pool(loaded_class *list, char *word){
    loaded_class *current = find_class_element(list, word);
    return current->class_file->constant_pool;
}

method_info *find_class_method(loaded_class *list, char *name, char *method_name){
    loaded_class *current = find_class_element(list, name);
    method_info *method_aux = current->class_file->methods;

    for(method_info *aux = method_aux; aux < method_aux+current->class_file->methods_count; aux++){
        char *method_name_aux = decode_utf8_string(current->class_file->constant_pool-1+aux->name_index);
        if(strcmp(method_name, method_name_aux) == 0) return aux;
    }
    return NULL;
}

field_info *find_class_field(loaded_class *list, char *class_name, char *field_name){
    loaded_class *current_class = find_class_element(list,class_name);
    field_info *field_aux = current_class->class_file->fields;

    for(field_info *aux = field_aux; aux < field_aux+current_class->class_file->fields_count; aux++){
		char *field_name_aux = decode_utf8_string(current_class->class_file->constant_pool-1+aux->name_index);
		if(strcmp(field_name,field_name_aux) == 0) return aux;
    }
    return NULL;
}

void print_class_list(loaded_class *list){
    loaded_class *pointer;
    for(pointer = list; pointer != NULL; pointer = pointer->next){
		printf("%s",decode_name_index_and_type(pointer->class_file->constant_pool,pointer->class_file->this_class,NAME_INDEX));
		printf("\n");
    }
}

void free_class_list(loaded_class *list){
    loaded_class *pointer = list;
    while(pointer != NULL){
        loaded_class *aux = pointer->next;
        free(pointer);
        pointer = aux;
    }
}