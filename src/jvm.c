#include "jvm.h"
#include "pilha_de_operandos.h"

JVM *initialize()
{
	JVM *jvm;
	instruction *instructions;

	jvm = (JVM *)malloc(sizeof(JVM));
	jvm->classes = create_class_list();
	jvm->frames = create_stack_frame();
	jvm->pc = 0;
	jvm->exception = 0;
	jvm->objects = NULL;
	jvm->exception_name = (char *)malloc(100 * sizeof(char));

	instructions = build_instructions();

	return jvm;
}

void run_jvm()
{
	method_info *methods = jvm->classes->class_file->methods;

	char *executable_class = decode_name_index_and_type(jvm->classes->class_file->constant_pool, jvm->classes->class_file->this_class, NAME_INDEX);

	for (method_info *method_aux = methods; method_aux < methods + jvm->classes->class_file->methods_count; method_aux++)
	{
		char *string_method = decode_utf8_string(jvm->classes->class_file->constant_pool - 1 + method_aux->name_index);
		char *string_descriptor = decode_utf8_string(jvm->classes->class_file->constant_pool - 1 + method_aux->descriptor_index);

		// Se o método for a main
		if (strcmp(string_method, "<clinit>") == 0 && strcmp(string_descriptor, "()V") == 0 && method_aux->access_flags == 0x0008)
		{
			run_method(method_aux, executable_class, 1);
		}
	}

	int i = 0;
	for (method_info *aux = methods; i < methods + jvm->classes->class_file->methods_count; i++)
	{
		char *string_method = decode_utf8_string(jvm->classes->class_file->constant_pool - 1 + aux->name_index);
		char *string_descriptor = decode_utf8_string(jvm->classes->class_file->constant_pool - 1 + aux->descriptor_index);

		// Se o método for a main
		if (strcmp(string_method, "main") == 0 && strcmp(string_descriptor, "([Ljava/lang/String;)V") == 0 && aux->access_flags == 0x0009)
		{
			run_method(aux, executable_class, 1);
		}
	}
}

void run_method(method_info *m, char *class_info, int caller)
{
	attribute_info *aux;

	int i;
	for (i = 0; i < m->attributes_count; i++)
	{
		aux = (*(m->attributes + i));

		// Revisar esse trecho. O que é atual e o que é corrente? atual virou current_class e corrente virou class info

		loaded_class *current_class = search_class_element(jvm->classes, class_info);
		char *name_index = decode_utf8_string(current_class->class_file->constant_pool - 1 + aux->attribute_name_index);
		// Se o atributo for code
		if (strcmp(name_index, "Code") == 0)
		{
			code_attribute *c = (code_attribute *)aux->info;
			if (caller == 1)
			{
				frame *f = create_frame(class_info, c->max_locals);
				push_frame(jvm->frames, *f);
			}

			interpret_code(c->code, c->code_length, m);
		}
	}
}

void interpret_code(u1 *code, u4 length, method_info *m)
{
	u1 opcode;
	int current_pc;
	u2 handler_pc = 0;

	for (u1 *j = code; j < code + length;)
	{
		opcode = *j;
		current_pc = jvm->pc;
		instruction i = instructions[opcode];
		u1 arg_count = i.arg_count;
		j++;
		if (arg_count > 0)
		{

			u1 *argumentos = malloc(arg_count * sizeof(u1));
			// Criar vetor de ponteiro de funções
			// Deixar todas as funções com a mesma assinatura
			for (u1 arg = 0; arg < arg_count; arg++)
			{
				argumentos[arg] = *j;
				j++;
			}

			switch (arg_count)
			{
			case 1:
				(*func_ptr[i.opcode])(get_top_element(jvm->frames), argumentos[0], 0);
				if (jvm->exception == 1)
				{
					handler_pc = verificaHandlerMetodo(m);
					// Se encontrou o handler
					if (handler_pc != USHRT_MAX)
					{
						jvm->exception = 0;
						free(jvm->exception_name);
						jvm->exception_name = malloc(100 * sizeof(char));
						jvm->pc = handler_pc;
					}
					jvm->pc = handler_pc;
					j = atualizarPCMetodoAtual(code, length);
					jvm->exception = 0;
				}
				jvm->pc += i.pc_instrucao;
				break;

			case 2:
				(*func_ptr[i.opcode])(get_top_element(jvm->frames), argumentos[0], argumentos[1]);
				// Verificar se flag de exceção foi setada
				if (jvm->exception == 1)
				{
					handler_pc = verificaHandlerMetodo(m);
					if (handler_pc != USHRT_MAX)
					{
						jvm->exception = 0;
						free(jvm->exception_name);
						jvm->exception_name = malloc(100 * sizeof(char));
						jvm->pc = handler_pc;
					}
					jvm->pc = handler_pc;
					j = atualizarPCMetodoAtual(code, length);
					jvm->exception = 0;
				}
				if (instrucaoBranch(i.inst_name))
				{
					if (current_pc != jvm->pc)
					{
						j = atualizarPCMetodoAtual(code, length);
					}
					else
					{
						if (strcmp(i.inst_name, "invokestatic") != 0 ||
								strcmp(i.inst_name, "invokevirtual") != 0 ||
								strcmp(i.inst_name, "invokespecial") != 0)
							jvm->pc += i.pc_instrucao;
					}
				}
				else
				{
					jvm->pc += i.pc_instrucao;
				}
				break;

			case 3:
				if (strcmp(i.inst_name, "multianewarray") == 0)
				{
					multianewarray_impl(get_top_element(jvm->frames), argumentos[0], argumentos[1], argumentos[2]);
					if (jvm->exception == 1)
					{
						handler_pc = verificaHandlerMetodo(m);
						if (handler_pc != USHRT_MAX)
						{
							jvm->exception = 0;
							free(jvm->exception_name);
							jvm->exception_name = malloc(100 * sizeof(char));
							jvm->pc = handler_pc;
						}
						jvm->pc = handler_pc;
						j = atualizarPCMetodoAtual(code, length);
					}
				}
				break;

			case 4:
				if (strcmp(i.inst_name, "invokeinterface") == 0)
				{
					invokeinterface_impl(get_top_element(jvm->frames), argumentos[0], argumentos[1], argumentos[2]);
					// Verificar se flag de exceção foi setada
					if (jvm->exception == 1)
					{
						handler_pc = verificaHandlerMetodo(m);
						if (handler_pc != USHRT_MAX)
						{
							jvm->exception = 0;
							free(jvm->exception_name);
							jvm->exception_name = malloc(100 * sizeof(char));
							jvm->pc = handler_pc;
						}
						jvm->pc = handler_pc;
						j = atualizarPCMetodoAtual(code, length);
						jvm->exception = 0;
					}
				}
				break;
			}
		}
		else if (arg_count == 0)
		{
			// Verificar se é a instrução wide
			if (strcmp(i.inst_name, "wide") == 0)
			{
				// Obter o opcode da instrução que deve ser modificada
				u1 novo_opcode = *j;
				i = instructions[novo_opcode];
				j++;
				// Verificar se é diferente de iinc
				if (novo_opcode != iinc)
				{
					// Pegar os parâmetros
					u1 *argumentos = malloc(arg_count * sizeof(u1));
					// Obter um argumento a mais, porque é o índice que será modificado
					for (u1 arg = 0; arg < i.arg_count + 1; arg++)
					{
						argumentos[arg] = *j;
						j++;
					}

					(*func_ptr[i.opcode])(get_top_element(jvm->frames), argumentos[0], argumentos[1]);

					jvm->pc += i.arg_count + 1;
				}
				else
				{
					u1 *argumentos = malloc(i.arg_count * 2 * sizeof(u1));

					for (u1 arg = 0; arg < i.arg_count * 2; arg++)
					{
						argumentos[arg] = *j;
						j++;
					}

					iinc_wide_fantasma(get_top_element(jvm->frames), argumentos[0], argumentos[1], argumentos[2], argumentos[3]);

					jvm->pc += i.arg_count * 2 - 1;
				}
			}
			else
			{

				(*func_ptr[i.opcode])(get_top_element(jvm->frames), 0, 0);
			}

			if (jvm->exception == 1)
			{
				handler_pc = verificaHandlerMetodo(m);
				if (handler_pc != USHRT_MAX)
				{
					free(jvm->exception_name);
					jvm->exception_name = malloc(100 * sizeof(char));
					jvm->pc = handler_pc;
				}
				jvm->pc = handler_pc;
				j = atualizarPCMetodoAtual(code, length);
				jvm->exception = 0;
			}

			jvm->pc += i.pc_instrucao;
		}
	}
}

frame *create_frame(char *class_info, u2 max_locals)
{
	frame *f = (frame *)malloc(sizeof(struct frame));
	f->return_address = jvm->pc;
	f->p = create_operand_stack();
	f->v = malloc(max_locals * sizeof(local_variable));

	for (int i = 0; i < max_locals; i++)
	{
		f->v[i].variable = malloc(sizeof(u4));
	}

	f->cp = find_class_constant_pool(jvm->classes, class_info);
	f->vetor_length = max_locals;
	f->current_class = malloc(100 * sizeof(char));
	strcpy(f->current_class, class_info);

	return (f);
}

void free_local_variable(local_variable *v, u2 vetor_length)
{
	int cont = 0;

	while (cont < vetor_length)
	{
		free(v[cont].variable);
		cont++;
	}

	free(v);
}

bool instruction_branch(char *instruction_name)
{
	if (strcmp(instruction_name, "goto") == 0 ||
			strcmp(instruction_name, "if_icmpgt") == 0 ||
			strcmp(instruction_name, "ifne") == 0 ||
			strcmp(instruction_name, "ifeq") == 0 ||
			strcmp(instruction_name, "iflt") == 0 ||
			strcmp(instruction_name, "ifge") == 0 ||
			strcmp(instruction_name, "ifle") == 0 ||
			strcmp(instruction_name, "ifgt") == 0 ||
			strcmp(instruction_name, "if_icmpeq") == 0 ||
			strcmp(instruction_name, "if_icmpne") == 0 ||
			strcmp(instruction_name, "if_icmplt") == 0 ||
			strcmp(instruction_name, "if_icmpge") == 0 ||
			strcmp(instruction_name, "if_icmple") == 0 ||
			strcmp(instruction_name, "if_acmpeq") == 0 ||
			strcmp(instruction_name, "if_acmpne") == 0 ||
			strcmp(instruction_name, "jsr") == 0 ||
			strcmp(instruction_name, "ifnonnull") == 0 ||
			strcmp(instruction_name, "ifnull") == 0)
		return true;
	else
		return false;
}

u2 check_handler_method(method_info *m)
{
	attribute_info *aux;

	// Procurar no método corrente se existe um handler para a exceção que foi lançada
	loaded_class *current_class = find_class_element(jvm->classes, get_top_element(jvm->frames).current_class);
	method_info *aux_method;

	while (get_top_element(jvm->frames) != NULL)
	{
		for (aux_method = m; aux_method < m + m->attributes_count; aux_method++)
		{
			aux = (*aux_method->attributes);
			char *name_index = decode_utf8_string(current_class->class_file->constant_pool - 1 + aux->attribute_name_index);
			if (strcmp(name_index, "Code") == 0)
			{
				code_attribute *c = (code_attribute *)aux->info;
				for (exception_table *aux_table = c->exception_table; aux_table < c->exception_table + c->exception_table_length; aux_table++)
				{
					char *exception_name = decodificaNIeNT(current_class->class_file->constant_pool, aux_table->catch_type, NAME_INDEX);
					if (strcmp(exception_name, jvm->exception_name) == 0)
					{
						// Retornar o valor do Handler se achar a excecao que foi lancada
						return (aux_table->handread_pc);
					}
				}
			}
		}

		// Se não encontrar o handler, desempilhar o frame corrente da pilha de frames e retornar -INT_MAX para indicar que o handler não foi encontrado
		// Desalocar operandos
		// while (jvm->frames->topo->f->p->topo != NULL)
		// {
		// 	Pop_operandos(jvm->frames->topo->f->p);
		// }

		// Desalocar vetor
		// freeVetorLocais(jvm->frames->topo->f->v, jvm->frames->topo->f->vetor_length);

		// Desempilhar o frame
		// Pop_frames(jvm->frames);
	}

	// Retornar USHRT_MAX para indicar que o handler não foi encontrado
	return (USHRT_MAX);
}


// u1* atualizarPCMetodoAtual (u1 *code, u4 length)
// {
// 	int contador = 0;
// 	int numArgs = 0;
// 	u1 opcode;
// 	for (u1 *aux = code; aux < code+length; aux++) {
// 			opcode = *aux;
// 			Instrucao i = instrucoes[opcode];
// 			contador += i.pc_instrucao;
// 			numArgs = i.numarg;
// 			if(numArgs > 0){
// 				for(u1 arg = 0; arg < numArgs; arg++) {
// 					aux++;
// 				}
// 			}
// 			if (contador == jvm->pc) {
// 				aux++;
// 				return aux;
// 			}
// 	}
// 	return 0;
// }

// void ImprimeCode (u1 *code, u4 length)
// {
// 	int numArgs = 0;
// 	for (u1 *saida = code; saida < code+length; saida++) {
// 		u1 opcodeaux = *saida;
// 		Instrucao iaux = instrucoes[opcodeaux];
// 		numArgs = iaux.numarg;

// 		if(numArgs>0){
// 			for(u1 arg = 0; arg < numArgs; arg++) {
// 				saida++;
// 			}
// 		}

// 	}

// }
