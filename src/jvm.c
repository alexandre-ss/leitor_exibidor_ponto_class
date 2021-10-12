#include "jvm.h"
#include "pilha_de_operandos.h"

JVM *jvm = NULL;
instruction *instructions;
void (*func_ptr[202])(frame *, u1, u1) = {nop_impl, aconst_null_impl, iconst_m1_impl, iconst_0_impl, iconst_1_impl, iconst_2_impl, iconst_3_impl, iconst_4_impl, iconst_5_impl, lconst_0_impl, lconst_1_impl, fconst_0_impl, fconst_1_impl, fconst_2_impl, dconst_0_impl, dconst_1_impl, bipush_impl, sipush_impl, ldc_impl, ldc_w_impl, ldc2_w_impl, iload_impl, lload_impl, fload_impl, dload_impl, aload_impl, iload_0_impl, iload_1_impl, iload_2_impl, iload_3_impl, lload_0_impl, lload_1_impl, lload_2_impl, lload_3_impl, fload_0_impl, fload_1_impl, fload_2_impl, fload_3_impl, dload_0_impl, dload_1_impl, dload_2_impl, dload_3_impl, aload_0_impl, aload_1_impl, aload_2_impl, aload_3_impl, iaload_impl, laload_impl, faload_impl, daload_impl, aaload_impl, baload_impl, caload_impl, saload_impl, istore_impl, lstore_impl, fstore_impl, dstore_impl, astore_impl, istore_0_impl, istore_1_impl, istore_2_impl, istore_3_impl, lstore_0_impl, lstore_1_impl, lstore_2_impl, lstore_3_impl, fstore_0_impl, fstore_1_impl, fstore_2_impl, fstore_3_impl, dstore_0_impl, dstore_1_impl, dstore_2_impl, dstore_3_impl, astore_0_impl, astore_1_impl, astore_2_impl, astore_3_impl, iastore_impl, lastore_impl, fastore_impl, dastore_impl, aastore_impl, bastore_impl, castore_impl, sastore_impl, pop_ghost, pop2_ghost, dup_impl, dup_x1_impl, dup_x2_impl, dup2_impl, dup2_x1_impl, dup2_x2_impl, swap_impl, iadd_impl, ladd_impl, fadd_impl, dadd_impl, isub_impl, lsub_impl, fsub_impl, dsub_impl, imul_impl, lmul_impl, fmul_impl, dmul_impl, idiv_impl, ldiv_impl, fdiv_impl, ddiv_impl, irem_impl, lrem_impl, frem_impl, drem_impl, ineg_impl, lneg_impl, fneg_impl, dneg_impl, ishl_impl, lshl_impl, ishr_impl, lshr_impl, iushr_impl, lushr_impl, iand_impl, land_impl, ior_impl, lor_impl, ixor_impl, lxor_impl, iinc_ghost, i2l_impl, i2f_impl, i2d_impl, l2i_impl, l2f_impl, l2d_impl, f2i_impl, f2l_impl, f2d_impl, d2i_impl, d2l_impl, d2f_impl, i2b_impl, i2c_impl, i2s_impl, lcmp_impl, fcmpl_impl, fcmpg_impl, dcmpl_impl, dcmpg_impl, ifeq_impl, ifne_impl, iflt_impl, ifge_impl, ifgt_impl, ifle_impl, if_icmpeq_impl, if_icmpne_impl, if_icmplt_impl, if_icmpge_impl, if_icmpgt_impl, if_icmple_impl, acmpeq_impl, acmpne_impl, inst_goto_impl, jsr_impl, ret_impl, tableswitch_impl, lookupswitch_impl, ireturn_impl, lreturn_impl, freturn_impl, dreturn_impl, areturn_impl, inst_return_impl, getstatic_impl, putstatic_impl, getfield_impl, putfield_impl, invokevirtual_impl, invokespecial_impl, invokestatic_impl, invokeinterface_ghost, invokedynamic_ghost, inst_new_impl, newarray_impl, anewarray_impl, arraylength_impl, athrow_impl, checkcast_impl, instanceof_impl, monitorenter_impl, monitorexit_impl, wide_impl, multianewarray_ghost, ifnull_impl, ifnonnull_impl, goto_w_impl, jsr_w_impl};

JVM *initialize()
{
	JVM *j;

	j = (JVM *)malloc(sizeof(JVM));
	j->classes = create_class_list();
	j->frames = init_stack_frame();
	j->pc = 0;
	j->exception = 0;
	j->objects = NULL;
	j->exception_name = (char *)malloc(100 * sizeof(char));

	instructions = build_instructions();

	return j;
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

	u2 i = 0;
	for (method_info *aux = methods; i < jvm->classes->class_file->methods_count; aux++, i++)
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

		loaded_class *current_class = find_class_element(jvm->classes, class_info);
		char *name_index = decode_utf8_string(current_class->class_file->constant_pool - 1 + aux->attribute_name_index);
		// Se o atributo for code
		if (strcmp(name_index, "Code") == 0)
		{
			code_attribute *c = (code_attribute *)aux->info;
			if (caller == 1)
			{
				frame *f = create_frame(class_info, c->max_locals);
				push_frame(jvm->frames, f);
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
					handler_pc = check_handler_method(m);
					// Se encontrou o handler
					if (handler_pc != USHRT_MAX)
					{
						jvm->exception = 0;
						free(jvm->exception_name);
						jvm->exception_name = malloc(100 * sizeof(char));
						jvm->pc = handler_pc;
					}
					jvm->pc = handler_pc;
					j = update_pc_current_method(code, length);
					jvm->exception = 0;
				}
				jvm->pc += i.pc_instruction;
				break;

			case 2:
				(*func_ptr[i.opcode])(get_top_element(jvm->frames), argumentos[0], argumentos[1]);
				// Verificar se flag de exceção foi setada
				if (jvm->exception == 1)
				{
					handler_pc = check_handler_method(m);
					if (handler_pc != USHRT_MAX)
					{
						jvm->exception = 0;
						free(jvm->exception_name);
						jvm->exception_name = malloc(100 * sizeof(char));
						jvm->pc = handler_pc;
					}
					jvm->pc = handler_pc;
					j = update_pc_current_method(code, length);
					jvm->exception = 0;
				}
				if (instruction_branch(i.inst_name))
				{
					if (current_pc != jvm->pc)
					{
						j = update_pc_current_method(code, length);
					}
					else
					{
						if (strcmp(i.inst_name, "invokestatic") != 0 ||
								strcmp(i.inst_name, "invokevirtual") != 0 ||
								strcmp(i.inst_name, "invokespecial") != 0)
							jvm->pc += i.pc_instruction;
					}
				}
				else
				{
					jvm->pc += i.pc_instruction;
				}
				break;

			case 3:
				if (strcmp(i.inst_name, "multianewarray") == 0)
				{
					multianewarray_impl(get_top_element(jvm->frames), argumentos[0], argumentos[1], argumentos[2]);
					if (jvm->exception == 1)
					{
						handler_pc = check_handler_method(m);
						if (handler_pc != USHRT_MAX)
						{
							jvm->exception = 0;
							free(jvm->exception_name);
							jvm->exception_name = malloc(100 * sizeof(char));
							jvm->pc = handler_pc;
						}
						jvm->pc = handler_pc;
						j = update_pc_current_method(code, length);
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
						handler_pc = check_handler_method(m);
						if (handler_pc != USHRT_MAX)
						{
							jvm->exception = 0;
							free(jvm->exception_name);
							jvm->exception_name = malloc(100 * sizeof(char));
							jvm->pc = handler_pc;
						}
						jvm->pc = handler_pc;
						j = update_pc_current_method(code, length);
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

					iinc_wide_ghost(get_top_element(jvm->frames), argumentos[0], argumentos[1], argumentos[2], argumentos[3]);

					jvm->pc += i.arg_count * 2 - 1;
				}
			}
			else
			{

				(*func_ptr[i.opcode])(get_top_element(jvm->frames), 0, 0);
			}

			if (jvm->exception == 1)
			{
				handler_pc = check_handler_method(m);
				if (handler_pc != USHRT_MAX)
				{
					free(jvm->exception_name);
					jvm->exception_name = malloc(100 * sizeof(char));
					jvm->pc = handler_pc;
				}
				jvm->pc = handler_pc;
				j = update_pc_current_method(code, length);
				jvm->exception = 0;
			}

			jvm->pc += i.pc_instruction;
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
	loaded_class *current_class = find_class_element(jvm->classes, get_top_element(jvm->frames)->current_class);
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
					char *exception_name = decode_name_index_and_type(current_class->class_file->constant_pool, aux_table->catch_type, NAME_INDEX);
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
		while (get_top_element(jvm->frames)->p->top != NULL)
		{
			pop_operand(get_top_element(jvm->frames)->p);
		}

		// Desalocar vetor
		free_local_variable(get_top_element(jvm->frames)->v, get_top_element(jvm->frames)->vetor_length);

		// Desempilhar o frame
		pop_frame(jvm->frames);
	}

	// Retornar USHRT_MAX para indicar que o handler não foi encontrado
	return (USHRT_MAX);
}

u1 *update_pc_current_method(u1 *code, u4 length)
{
	int count = 0;
	int arg_count = 0;
	u1 opcode;
	for (u1 *aux = code; aux < code + length; aux++)
	{
		opcode = *aux;
		instruction i = instructions[opcode];
		count += i.pc_instruction;
		arg_count = i.arg_count;
		if (arg_count > 0)
		{
			for (u1 arg = 0; arg < arg_count; arg++)
			{
				aux++;
			}
		}
		if (count == jvm->pc)
		{
			aux++;
			return aux;
		}
	}
	return 0;
}

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
