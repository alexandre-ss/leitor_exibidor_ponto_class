#include "jvm.h"

JVM* initialize() {
	JVM *jvm;
	instruction *instructions;

	jvm = (JVM*) malloc(sizeof(JVM));
	jvm->classes = create_class_list();
	jvm->frames = create_stack_frame();
	jvm->pc = 0;
	jvm->exception = 0;
	jvm->objects = NULL;
	jvm->exception_name = (char *) malloc(100 * sizeof(char));

	instructions = build_instructions();

	return jvm;
}

void run_jvm() {
    method_info *methods = jvm->classes->ClassFile->methods;

    char *executable_class = decode_name_index_and_type(jvm->classes->ClassFile->constant_pool, jvm->classes->ClassFile->this_class, NAME_INDEX);

    for (method_info *method_aux = methods; method_aux < methods + jvm->classes->ClassFile->methods_count; method_aux++) {
        char *string_method = decode_utf8_string(jvm->classes->ClassFile->constant_pool - 1 + method_aux->name_index);
        char *string_descriptor = decode_utf8_string(jvm->classes->ClassFile->constant_pool - 1 + method_aux->descriptor_index);

        // Se o método for a main
        if(strcmp(string_method, "<clinit>") == 0 && strcmp(string_descriptor, "()V") == 0 && method_aux->access_flags == 0x0008){
            run_method(method_aux, executable_class, 1);
        }
    }

    int i = 0;
    for (method_info *aux = methods; i < methods + jvm->classes->ClassFile->methods_count; i++) {
        char *string_method = decode_utf8_string(jvm->classes->ClassFile->constant_pool - 1 + aux->name_index);
        char *string_descriptor = decode_utf8_string(jvm->classes->ClassFile->constant_pool - 1 + aux->descriptor_index);

        // Se o método for a main
        if(strcmp(string_method, "main") == 0 && strcmp(string_descriptor, "([Ljava/lang/String;)V") == 0 && aux->access_flags == 0x0009){
			run_method(aux, executable_class, 1);
		}
    }
}

void run_method(method_info *m, char* class_info, int caller) {
    attribute_info *aux;
    
    int i;
    for (i = 0; i < m->attributes_count; i++) {
		aux = (*(m->attributes + i));




        // Revisar esse trecho. O que é atual e o que é corrente? atual virou current_class e corrente virou class info


		loaded_class *current_class = search_class_element(jvm->classes, class_info);
		char *name_index = decode_utf8_string(current_class->ClassFile->constant_pool - 1 + aux->attribute_name_index);
		// Se atributo for code
		if(strcmp(name_index,"Code") == 0) {
			code_attribute *c = (code_attribute *) aux->info;
			if(caller == 1){
				frame *f = create_frame(class_info, c->max_locals);
				jvm->frames = Push_frames(jvm->frames,f);
			}

			interpret_code(c->code,c->code_length,m);
		}
	}
}


// REVISARR
frame* create_frame(char *class_info, u2 max_locals)
{
	frame *f = (frame *) malloc(sizeof(struct frame));
	f->return_address = jvm->pc;
	f->p = CriarPilha_operandos();
	f->v = malloc(max_locals*sizeof(Vetor_locais));
	for (int i = 0; i < max_locals; i++) {
		f->v[i].variavel = malloc(sizeof(u4));
	}
	f->cp = find_class_constant_pool(jvm->classes, class_info);
	f->vetor_length = max_locals;
	f->classeCorrente = malloc(100 * sizeof(char));
	strcpy(f->classeCorrente,classeCorrente);

	return(f);
}


