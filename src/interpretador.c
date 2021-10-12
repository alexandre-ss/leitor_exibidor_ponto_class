#include "interpretador.h"
#include "jvm.h"

ClassFile *resolve_class(char *class_name)
{
	loaded_class *lc = BuscarElemento_classes(jvm->classes, class_name);
	ClassFile *classe = NULL;

	if (lc != NULL)
	{
		return lc->class_file;
	}
	else
	{
		char *nomearquivo = (char *)malloc((strlen(class_name) + 7) * sizeof(char));
		strcpy(nomearquivo, class_name);
		strcat(nomearquivo, ".class");

		classe = lerArquivo(nomearquivo);
		jvm->classes = InserirFim_classes(jvm->classes, classe);
	}

	return (classe);
}

bool resolve_method(cp_info *cp, u2 cp_index, u1 interface)
{
	cp_info *method_ref = cp - 1 + cp_index;
	char *class_name = NULL;

	if (!interface)
	{
		class_name = decode_name_index_and_type(cp, method_ref->cp_union.CONSTANT_Methodref_info.class_index, NAME_INDEX);
	}
	else
	{
		class_name = decode_name_index_and_type(cp, method_ref->cp_union.CONSTANT_InterfaceMethodref_info.class_index, NAME_INDEX);
	}
	return resolve_class(class_name) != NULL ? true : false;
}

char *get_name_method(cp_info *cp, u2 cp_index, u1 interface)
{
	cp_info *methodref = cp - 1 + cp_index;
	char *descriptor = NULL;

	if (!interface)
	{
		descriptor = decode_name_index_and_type(cp, methodref->cp_union.CONSTANT_Methodref_info.name_and_type_index, NAME_AND_TYPE);
	}
	else
	{
		descriptor = decode_name_index_and_type(cp, methodref->cp_union.CONSTANT_InterfaceMethodref_info.name_and_type_index, NAME_AND_TYPE);
	}

	char *pch = strtok(descriptor, ":");

	return pch;
}

char *get_descriptor_method(cp_info *cp, u2 cp_index, u1 interface)
{
	cp_info *methodref = cp - 1 + cp_index;
	char *descriptor = NULL;

	if (!interface)
	{
		descriptor = decode_name_index_and_type(cp, methodref->cp_union.CONSTANT_Methodref_info.name_and_type_index, NAME_AND_TYPE);
	}
	else
	{
		descriptor = decode_name_index_and_type(cp, methodref->cp_union.CONSTANT_InterfaceMethodref_info.name_and_type_index, NAME_AND_TYPE);
	}

	char *pch = strtok(descriptor, ":");
	pch = strtok(NULL, ":");

	return pch;
}

int descriptor_field_validate(char *descriptor)
{
	if (*descriptor == 'L')
		return 5;

	if (strcmp(descriptor, "I") == 0)
	{
		return 0;
	}
	else if (strcmp(descriptor, "F") == 0)
	{
		return 1;
	}
	else if (strcmp(descriptor, "B") == 0)
	{
		return 2;
	}
	else if (strcmp(descriptor, "C") == 0)
	{
		return 3;
	}
	else if (strcmp(descriptor, "S") == 0)
	{
		return 4;
	}
	else if (strcmp(descriptor, "L") == 0)
	{
		return 5;
	}
	else if (strcmp(descriptor, "Z") == 0)
	{
		return 6;
	}
	else if (strcmp(descriptor, "D") == 0)
	{
		return 7;
	}
	else if (strcmp(descriptor, "J") == 0)
	{
		return 8;
	}
	return NULL;
}

char *get_class_method(cp_info *cp, u2 cp_index)
{
	cp_info *methodref = cp - 1 + cp_index;
	char *class_name = NULL;

	class_name = decode_name_index_and_type(cp, methodref->cp_union.CONSTANT_Methodref_info.class_index, NAME_INDEX);
	return class_name;
}

frame *transfer_array_stack(frame *prev, frame *new_frame, int *param_count)
{
	operand_stack *aux = create_operand_stack();

	int count = 0;
	while (prev->p->top != NULL && count < (*param_count))
	{
		operand_stack *p = pop_operand(prev->p);

		// Ordem reversa
		aux = push_operand(aux, p->top->operand, (void *)p->top->ref, p->top->operand_type);
		count++;
	}

	for (int i = 0; i < count; i++)
	{
		operand_stack *p = pop_operand(aux);

		new_frame->v[i].variable = malloc(sizeof(u4));
		if (p->top->operand_type <= 8)
		{
			*(new_frame->v[i].variable) = (u4)p->top->operand;
		}
		else
		{
			*(new_frame->v[i].variable) = (intptr_t)p->top->ref;
		}

		new_frame->v[i].type = (u1)p->top->operand_type;
	}

	*param_count = count;

	return new_frame;
}

frame *transfer_array_stack_count(frame *f, frame *new_frame, int qtd)
{
	operand_stack *aux = create_operand_stack();

	int count = 0;
	while (count < qtd)
	{
		operand_stack *p = pop_operand(f->p);

		// Ordem reversa
		aux = push_operand(aux, p->top->operand, (void *)p->top->ref, p->top->operand_type);
		count++;
	}

	new_frame->v = malloc(count * sizeof(local_variable));

	for (int i = 0; i < count; i++)
	{
		operand_stack *p = pop_operand(aux);

		new_frame->v[i].variable = malloc(sizeof(u4));
		if (p->top->operand_type <= 8)
		{
			*(new_frame->v[i].variable) = (u4)p->top->operand;
		}
		else
		{
			*(new_frame->v[i].variable) = (intptr_t)p->top->ref;
		}

		new_frame->v[i].type = (u1)p->top->operand_type;
	}

	return new_frame;
}

bool is_super(u2 flag)
{
	int super = false;
	while (flag)
	{
		if (flag >= ACC_SYNTHETIC)
			flag -= ACC_SYNTHETIC;
		if (flag >= ACC_ABSTRACT)
			flag -= ACC_ABSTRACT;
		if (flag >= ACC_INTERFACE)
			flag -= ACC_INTERFACE;
		if (flag >= ACC_TRANSIENT)
			flag -= ACC_TRANSIENT;
		if (flag >= ACC_VOLATILE)
			flag -= ACC_VOLATILE;
		if (flag >= ACC_SUPER)
		{
			flag -= ACC_SUPER;
			super = true;
			break;
		}
		if (flag >= ACC_FINAL)
			flag -= ACC_FINAL;
		if (flag >= ACC_STATIC)
			flag -= ACC_STATIC;
		if (flag >= ACC_PROTECTED)
			flag -= ACC_PROTECTED;
		if (flag >= ACC_PRIVATE)
			flag -= ACC_PRIVATE;
		if (flag >= ACC_PUBLIC)
			flag -= ACC_PUBLIC;
	}
	return (super);
}

void nop_impl(frame *par0, u1 par1, u1 par2)
{
	return;
}

void aconst_null_impl(frame *f, u1 par1, u1 par2)
{
	push_operand(f->p, 0, NULL, REFERENCE_OP); // 0 do tipo referência quer dizer referência apontando para NULL
}

void iconst_m1_impl(frame *f, u1 par1, u1 par2)
{
	i4 integer_sign = (i4)-1;
	push_operand(f->p, integer_sign, NULL, INTEGER_OP);
}

void iconst_0_impl(frame *f, u1 par1, u1 par2)
{
	i4 integer_sign = (i4)0;
	push_operand(f->p, integer_sign, NULL, INTEGER_OP);
}

void iconst_1_impl(frame *f, u1 par1, u1 par2)
{
	i4 integer_sign = (i4)1;
	push_operand(f->p, integer_sign, NULL, INTEGER_OP);
}

void iconst_2_impl(frame *f, u1 par1, u1 par2)
{
	i4 integer_sign = (i4)2;
	push_operand(f->p, integer_sign, NULL, INTEGER_OP);
}

void iconst_3_impl(frame *f, u1 par1, u1 par2)
{
	i4 integer_sign = (i4)3;
	push_operand(f->p, integer_sign, NULL, INTEGER_OP);
}

void iconst_4_impl(frame *f, u1 par1, u1 par2)
{
	i4 integer_sign = (i4)4;
	push_operand(f->p, integer_sign, NULL, INTEGER_OP);
}

void iconst_5_impl(frame *f, u1 par1, u1 par2)
{
	i4 integer_sign = (i4)5;
	push_operand(f->p, (u4)integer_sign, NULL, INTEGER_OP);
}

void lconst_0_impl(frame *f, u1 par1, u1 par2)
{
	i4 high_bytes = (i4)0;
	push_operand(f->p, high_bytes, NULL, LONG_OP);

	i4 low_bytes = (i4)0;
	push_operand(f->p, low_bytes, NULL, LONG_OP);
}

void lconst_1_impl(frame *f, u1 par1, u1 par2)
{
	i4 high_bytes = (i4)0;
	push_operand(f->p, high_bytes, NULL, LONG_OP);

	i4 low_bytes = (i4)1;
	push_operand(f->p, low_bytes, NULL, LONG_OP);
}

void fconst_0_impl(frame *f, u1 par1, u1 par2)
{
	i4 float_bytes = (i4)0;
	push_operand(f->p, float_bytes, NULL, FLOAT_OP);
}

void fconst_1_impl(frame *f, u1 par1, u1 par2)
{
	i4 float_bytes = 0x3f800000;
	push_operand(f->p, float_bytes, NULL, FLOAT_OP);
}

void fconst_2_impl(frame *f, u1 par1, u1 par2)
{
	i4 float_bytes = 0x40000000;
	push_operand(f->p, float_bytes, NULL, FLOAT_OP);
}

void dconst_0_impl(frame *f, u1 par1, u1 par2)
{
	i4 high_bytes = (i4)0;
	push_operand(f->p, high_bytes, NULL, DOUBLE_OP);

	i4 low_bytes = (i4)0;
	push_operand(f->p, low_bytes, NULL, DOUBLE_OP);
}

void dconst_1_impl(frame *f, u1 par1, u1 par2)
{
	i4 high_bytes = 0x3FF00000;
	push_operand(f->p, high_bytes, NULL, DOUBLE_OP);

	i4 low_bytes = (i4)0;
	push_operand(f->p, low_bytes, NULL, DOUBLE_OP);
}

void bipush_impl(frame *f, u1 byte, u1 par1)
{
	i1 aux = (i1)byte;
	i4 byte_int = (i4)aux;
	push_operand(f->p, byte_int, NULL, BYTE_OP);
}

void sipush_impl(frame *f, u1 byte1, u1 byte2)
{
	i2 byte_short = (i2)(byte1 << 8) | (i2)byte2;
	i4 byte_int = (i4)byte_short;
	push_operand(f->p, byte_int, NULL, SHORT_OP);
}

void ldc_impl(frame *f, u1 indexbyte1, u1 par2)
{
	cp_info *item = f->cp - 1 + indexbyte1;
	void *value = NULL;
	u4 num = 0;
	void *class = NULL;

	switch (item->tag)
	{
	case CONSTANT_String:
		value = (char *)decode_utf8_string(f->cp - 1 + item->cp_union.CONSTANT_String_info.string_index);
		f->p = push_operand(f->p, -INT_MAX, value, REFERENCE_OP);
		break;

	case CONSTANT_Float:
		num = item->cp_union.CONSTANT_Float_info.bytes;
		f->p = push_operand(f->p, num, NULL, FLOAT_OP);
		break;

	case CONSTANT_Integer:
		num = item->cp_union.CONSTANT_Integer_info.bytes;
		f->p = push_operand(f->p, num, NULL, INTEGER_OP);
		break;

	case CONSTANT_Class:
		value = (char *)decode_utf8_string(f->cp - 1 + item->cp_union.CONSTANT_Class_info.name_index);
		class = resolve_class(value);
		f->p = push_operand(f->p, -INT_MAX, class, REFERENCE_OP);
		break;

	default:
		value = (char *)decode_name_index_and_type(f->cp, item->cp_union.CONSTANT_Methodref_info.class_index, NAME_INDEX);
		value = (char *)decode_name_index_and_type(f->cp, item->cp_union.CONSTANT_Methodref_info.name_and_type_index, NAME_AND_TYPE);
		break;
	}
}

void ldc_w_impl(frame *f, u1 indexbyte1, u1 indexbyte2)
{
	u2 index_cp = normalize_index(indexbyte1, indexbyte2);
	cp_info *item = f->cp - 1 + index_cp;
	void *value = NULL;
	u4 num = 0;
	void *class = NULL;

	switch (item->tag)
	{
	case CONSTANT_String:
		value = (char *)decode_utf8_string(f->cp - 1 + item->cp_union.CONSTANT_String_info.string_index);
		f->p = push_operand(f->p, -INT_MAX, value, REFERENCE_OP);
		break;
	case CONSTANT_Float:
		num = item->cp_union.CONSTANT_Float_info.bytes;
		f->p = push_operand(f->p, num, NULL, FLOAT_OP);
		break;
	case CONSTANT_Integer:
		num = item->cp_union.CONSTANT_Integer_info.bytes;
		f->p = push_operand(f->p, num, NULL, INTEGER_OP);
		break;
	case CONSTANT_Class:
		value = (char *)decode_utf8_string(f->cp - 1 + item->cp_union.CONSTANT_Class_info.name_index);
		class = resolve_class(value);
		f->p = push_operand(f->p, -INT_MAX, class, REFERENCE_OP);
		break;
	default:
		value = (char *)decode_name_index_and_type(f->cp, item->cp_union.CONSTANT_Methodref_info.class_index, NAME_INDEX);
		value = (char *)decode_name_index_and_type(f->cp, item->cp_union.CONSTANT_Methodref_info.name_and_type_index, NAME_AND_TYPE);
		break;
	}
}

void ldc2_w_impl(frame *f, u1 branchbyte1, u1 branchbyte2)
{
	int8_t v1 = (int8_t)branchbyte1;
	int8_t v2 = (int8_t)branchbyte2;
	int16_t branch_offset = (v1 << 8) | v2;
	cp_info *double_value = f->cp - 1 + branch_offset;

	u4 high, low;

	if (double_value->tag == CONSTANT_Double)
	{
		high = double_value->cp_union.CONSTANT_Double_info.high_bytes;
		low = double_value->cp_union.CONSTANT_Double_info.low_bytes;
	}
	else
	{
		high = double_value->cp_union.CONSTANT_Long_info.high_bytes;
		low = double_value->cp_union.CONSTANT_Long_info.low_bytes;
	}

	if (double_value->tag == CONSTANT_Double)
	{
		push_operand(f->p, high, NULL, DOUBLE_OP); //high
		push_operand(f->p, low, NULL, DOUBLE_OP);	 //low
	}
	else
	{
		push_operand(f->p, high, NULL, LONG_OP); //high
		push_operand(f->p, low, NULL, LONG_OP);	 //low
	}
}

void iload_impl(frame *f, u1 index, u1 par1)
{
	push_operand(f->p, (i4) * (f->v[index].variable), NULL, INTEGER_OP);
}

void lload_impl(frame *f, u1 index, u1 par1)
{
	push_operand(f->p, (i4) * (f->v[index].variable), NULL, LONG_OP);
	Push_operand(f->p, (i4) * (f->v[index + 1].variable), NULL, LONG_OP);
}

void fload_impl(frame *f, u1 index, u1 par1)
{
	Push_operand(f->p, (i4) * (f->v[index].variable), NULL, FLOAT_OP);
}

void dload_impl(frame *f, u1 index, u1 par2)
{
	push_operand(f->p, (i4) * (f->v[index].variable), NULL, DOUBLE_OP);
	push_operand(f->p, (i4) * (f->v[index + 1].variable), NULL, DOUBLE_OP);
}

void aload_impl(frame *f, u1 index, u1 par1)
{
	push_operand(f->p, -INT_MAX, (void *)(intptr_t) * (f->v[index].variable), f->v[index].type);
}

void iload_0_impl(frame *f, u1 par1, u1 par2)
{
	Push_operand(f->p, (i4) * (f->v[0].variable), NULL, INTEGER_OP);
}

void iload_1_impl(frame *f, u1 par1, u1 par2)
{
	push_operand(f->p, (i4) * (f->v[1].variable), NULL, INTEGER_OP);
}

void iload_2_impl(frame *f, u1 par1, u1 par2)
{
	push_operand(f->p, (i4) * (f->v[2].variable), NULL, INTEGER_OP);
}

void iload_3_impl(frame *f, u1 par1, u1 par2)
{
	push_operand(f->p, (i4) * (f->v[3].variable), NULL, INTEGER_OP);
}

void lload_0_impl(frame *f, u1 par, u1 par2)
{
	push_operand(f->p, (i4) * (f->v[0].variable), NULL, LONG_OP);
	push_operand(f->p, (i4) * (f->v[1].variable), NULL, LONG_OP);
}

void lload_1_impl(frame *f, u1 par, u1 par2)
{
	push_operand(f->p, (i4) * (f->v[1].variable), NULL, LONG_OP);
	push_operand(f->p, (i4) * (f->v[2].variable), NULL, LONG_OP);
}

void lload_2_impl(frame *f, u1 par, u1 par2)
{
	push_operand(f->p, (i4) * (f->v[2].variable), NULL, LONG_OP);
	push_operand(f->p, (i4) * (f->v[3].variable), NULL, LONG_OP);
}

void lload_3_impl(frame *f, u1 par, u1 par2)
{
	push_operand(f->p, (i4) * (f->v[3].variable), NULL, LONG_OP);
	push_operand(f->p, (i4) * (f->v[4].variable), NULL, LONG_OP);
}

void fload_3_impl(frame *f, u1 par1, u1 par2)
{
	push_operand(f->p, (i4) * (f->v[3].variable), NULL, FLOAT_OP);
}

void dload_0_impl(frame *f, u1 par1, u1 par2)
{
	push_operand(f->p, (i4) * (f->v[0].variable), NULL, DOUBLE_OP); //high
	push_operand(f->p, (i4) * (f->v[1].variable), NULL, DOUBLE_OP); //low
}

void dload_1_impl(frame *f, u1 par1, u1 par2)
{
	push_operand(f->p, (i4) * (f->v[1].variable), NULL, DOUBLE_OP);
	push_operand(f->p, (i4) * (f->v[2].variable), NULL, DOUBLE_OP);
}

void dload_2_impl(frame *f, u1 par1, u1 par2)
{
	push_operand(f->p, (i4) * (f->v[2].variable), NULL, DOUBLE_OP);
	push_operand(f->p, (i4) * (f->v[3].variable), NULL, DOUBLE_OP);
}

void dload_3_impl(frame *f, u1 par1, u1 par2)
{
	push_operand(f->p, (i4) * (f->v[3].variable), NULL, DOUBLE_OP);
	push_operand(f->p, (i4) * (f->v[4].variable), NULL, DOUBLE_OP);
}

void aload_0_impl(frame *f, u1 par1, u1 par2)
{
	push_operand(f->p, -INT_MAX, (void *)(intptr_t) * (f->v[0].variable), f->v[0].type);
}

void aload_1_impl(frame *f, u1 par1, u1 par2)
{
	push_operand(f->p, -INT_MAX, (void *)(intptr_t) * (f->v[1].variable), f->v[1].type);
}

void aload_2_impl(frame *f, u1 par1, u1 par2)
{
	push_operand(f->p, -INT_MAX, (void *)(intptr_t) * (f->v[2].variable), f->v[2].type);
}

void aload_3_impl(frame *f, u1 par1, u1 par2)
{
	push_operand(f->p, -INT_MAX, (void *)(intptr_t) * (f->v[3].variable), f->v[3].type);
}

void iaload_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *index = pop_operand(f->p);
	operand_stack *reference = pop_operand(f->p);

	u4 *address = (reference->top->reference) + (index->top->operand * sizeof(i4));

	push_operand(f->p, *address, NULL, INTEGER_OP);
}





void laload_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *index = pop_operand(f->p);
	operand_stack *reference = pop_operand(f->p);

	u4 *address = (reference->top->reference) + (index->top->operand * 2 * sizeof(i4));
	//Verificar ordem
	push_operand(f->p, *address, NULL, LONG_OP);
	address++;
	push_operand(f->p, *(address), NULL, LONG_OP);
}

void faload_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *index = pop_operand(f->p);
	operand_stack *reference = pop_operand(f->p);

	u4 *address = (reference->top->reference) + (index->top->operand * sizeof(i4));

	push_operand(f->p, *address, NULL, FLOAT_OP);
}

void daload_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *index = pop_operand(f->p);
	operand_stack *reference = pop_operand(f->p);

	u4 *address = (reference->top->reference) + (index->top->operand * 2 * sizeof(u4));

	//Verificar ordem
	push_operand(f->p, *address, NULL, DOUBLE_OP);
	address++;
	push_operand(f->p, *(address), NULL, DOUBLE_OP);
}

void aaload_impl(frame *f, u1 par1, u1 par2)
{
}

void baload_impl(frame *f, u1 par1, u1 par2)
{

	operand_stack *index = pop_operand(f->p);
	operand_stack *reference = pop_operand(f->p);

	i1 *address = (reference->top->reference) + (index->top->operand * sizeof(i1));
	push_operand(f->p, *address, NULL, BYTE_OP);
}

void caload_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *index = pop_operand(f->p);
	operand_stack *reference = pop_operand(f->p);

	u2 *address = (reference->top->reference) + (index->top->operand * sizeof(u2));
	push_operand(f->p, *address, NULL, CHAR_OP);
}

void saload_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *index = pop_operand(f->p);
	operand_stack *reference = pop_operand(f->p);

	i2 *address = (reference->top->reference) + (index->top->operand * sizeof(i2));

	push_operand(f->p, *address, NULL, SHORT_OP);
}

void istore_impl(frame *f, u1 index, u1 par1)
{
	operand_stack *valor = pop_operand(f->p);
	*(f->v[index].variavel) = (i4)valor->top->operand;
}

void lstore_impl(frame *f, u1 index, u1 par1)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[index].variavel) = (u4)high_bytes->top->operand;
	*(f->v[index + 1].variavel) = (u4)low_bytes->top->operand;
}

void fstore_impl(frame *f, u1 index, u1 par1)
{
	operand_stack *valor = pop_operand(f->p);
	*(f->v[index].variavel) = (u4)valor->top->operand;
}

void dstore_impl(frame *f, u1 index, u1 par1)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[index].variavel) = (u4)high_bytes->top->operand;
	*(f->v[index + 1].variavel) = (u4)low_bytes->top->operand;
}

void astore_impl(frame *f, u1 index, u1 par1)
{
	operand_stack *valor = pop_operand(f->p);
	*(f->v[index].variavel) = (intptr_t)valor->top->reference;
	f->v[index].tipo_variavel = valor->top->tipo_operand;
}

void istore_0_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *valor = pop_operand(f->p);
	*(f->v[0].variavel) = (i4)valor->top->operand;
}

void istore_1_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *valor = pop_operand(f->p);
	*(f->v[1].variavel) = (u4)valor->top->operand;
}

void istore_2_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *valor = pop_operand(f->p);
	*(f->v[2].variavel) = (u4)valor->top->operand;
}

void istore_3_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *valor = pop_operand(f->p);
	*(f->v[3].variavel) = (u4)valor->top->operand;
}

void lstore_0_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[0].variavel) = (u4)high_bytes->top->operand;
	*(f->v[1].variavel) = (u4)low_bytes->top->operand;
}

void lstore_1_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[1].variavel) = (u4)high_bytes->top->operand;
	*(f->v[2].variavel) = (u4)low_bytes->top->operand;
}

void lstore_2_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[2].variavel) = (u4)high_bytes->top->operand;
	*(f->v[3].variavel) = (u4)low_bytes->top->operand;
}

void lstore_3_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[3].variavel) = (u4)high_bytes->top->operand;
	*(f->v[4].variavel) = (u4)low_bytes->top->operand;
}

void fstore_0_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *valor = pop_operand(f->p);
	*(f->v[0].variavel) = (u4)valor->top->operand;
}

void fstore_1_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *valor = pop_operand(f->p);
	*(f->v[1].variavel) = (u4)valor->top->operand;
}

void fstore_2_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *valor = pop_operand(f->p);
	*(f->v[2].variavel) = (u4)valor->top->operand;
}

void fstore_3_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *valor = pop_operand(f->p);
	*(f->v[3].variavel) = (u4)valor->top->operand;
}

void dstore_0_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[0].variavel) = (u4)high_bytes->top->operand;
	*(f->v[1].variavel) = (u4)low_bytes->top->operand;
}

void dstore_1_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[1].variavel) = (u4)high_bytes->top->operand;
	*(f->v[2].variavel) = (u4)low_bytes->top->operand;
}

void dstore_2_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[2].variavel) = (u4)high_bytes->top->operand;
	*(f->v[3].variavel) = (u4)low_bytes->top->operand;
}

void dstore_3_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[3].variavel) = (u4)high_bytes->top->operand;
	*(f->v[4].variavel) = (u4)low_bytes->top->operand;
}

void astore_0_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *valor = pop_operand(f->p);
	*(f->v[0].variavel) = (intptr_t)valor->top->reference;
	f->v[0].tipo_variavel = valor->top->tipo_operand;
}

void astore_1_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *valor = pop_operand(f->p);

	*(f->v[1].variavel) = (intptr_t)valor->top->reference;
	f->v[1].tipo_variavel = valor->top->tipo_operand;
}

void astore_2_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *valor = pop_operand(f->p);
	*(f->v[2].variavel) = (intptr_t)valor->top->reference;
	f->v[2].tipo_variavel = valor->top->tipo_operand;
}

void astore_3_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *valor = pop_operand(f->p);
	*(f->v[3].variavel) = (intptr_t)valor->top->reference;
	f->v[3].tipo_variavel = valor->top->tipo_operand;
}

void iastore_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *valor = pop_operand(f->p);
	operand_stack *index = pop_operand(f->p);
	operand_stack *array = pop_operand(f->p);

	i4 *address_array = ((array->top->reference) + (index->top->operand * sizeof(i4)));
	*address_array = valor->top->operand;
}

void lastore_impl(frame *f, u1 par1, u1 par2)
{

	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);
	operand_stack *index = pop_operand(f->p);
	operand_stack *array = pop_operand(f->p);

	i4 *address_array = ((array->top->reference) + (index->top->operand * 2 * sizeof(i4)));

	*address_array = high_bytes->top->operand;

	address_array++;

	*address_array = low_bytes->top->operand;
}

void fastore_impl(frame *f, u1 par1, u1 par2)
{

	operand_stack *valor = pop_operand(f->p);
	operand_stack *index = pop_operand(f->p);
	operand_stack *array = pop_operand(f->p);

	i4 *address_array = ((array->top->reference) + (index->top->operand * sizeof(i4)));
	*address_array = valor->top->operand;
}

void dastore_impl(frame *f, u1 par1, u1 par2)
{

	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);
	operand_stack *index = pop_operand(f->p);
	operand_stack *array = pop_operand(f->p);

	u4 *address_array = ((array->top->reference) + (index->top->operand * 2 * sizeof(u4)));

	*address_array = high_bytes->top->operand;

	address_array++;

	*address_array = low_bytes->top->operand;
}

void bastore_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *valor = pop_operand(f->p);
	operand_stack *index = pop_operand(f->p);
	operand_stack *array = pop_operand(f->p);

	i1 *address_array = ((array->top->reference) + (index->top->operand * sizeof(i1)));
	*address_array = valor->top->operand;
}

void castore_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *valor = pop_operand(f->p);
	operand_stack *index = pop_operand(f->p);
	operand_stack *array = pop_operand(f->p);

	u2 *address_array = ((array->top->reference) + (index->top->operand * sizeof(u2)));
	*address_array = valor->top->operand;
}

void sastore_impl(frame *f, u1 par1, u1 par2)
{
	operand_stack *valor = pop_operand(f->p);
	operand_stack *index = pop_operand(f->p);
	operand_stack *array = pop_operand(f->p);

	i2 *address_array = ((array->top->reference) + (index->top->operand * sizeof(i2)));
	*address_array = valor->top->operand;
}

void aastore_impl(frame *f, u1 par1, u1 par2)
{
}

operand_stack *pop_impl(frame *f)
{
	operand_stack *valor = pop_operand(f->p);

	return (valor);
}

operand_stack **pop2_impl(frame *f)
{
	pop_operand(f->p);
	pop_operand(f->p);
	return NULL;
}

void pop_fantasma(frame *f, u1 par1, u1 par2)
{
	pop_impl(f);
}

void pop2_fantasma(frame *par0, u1 par1, u1 par2)
{
}

void dup_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor = Topo_operandos(f->p);
	f->p = Push_operandos(f->p,valor->topo->operando,valor->topo->referencia,valor->topo->tipo_operando);
}

void dup_x1_impl(Frame *f, u1 par1, u1 par2){
	Pilha_operandos *valor1 = Pop_operandos(f->p);
	Pilha_operandos *valor2 = Pop_operandos(f->p);

	f->p = Push_operandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
	f->p = Push_operandos(f->p,valor2->topo->operando,valor2->topo->referencia,valor2->topo->tipo_operando);
	f->p = Push_operandos(f->p,valor1->topo->operando,valor1->topo->referencia,valor1->topo->tipo_operando);
}

