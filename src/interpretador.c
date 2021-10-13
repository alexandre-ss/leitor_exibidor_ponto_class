#include "interpretador.h"
#include "jvm.h"

ClassFile *resolve_class(char *class_name)
{
	loaded_class *lc = find_class_element(jvm->classes, class_name);
	ClassFile *class = NULL;

	if (lc != NULL)
	{
		return lc->class_file;
	}
	else
	{
		char *file_name = (char *)malloc((strlen(class_name) + 7) * sizeof(char));
		strcpy(file_name, class_name);
		strcat(file_name, ".class");

		class = read_file(file_name);
		jvm->classes = insert_class_element(jvm->classes, class);
	}

	return (class);
}

bool resolve_method(cp_info *cp, u2 cp_index, u1 interface)
{
	cp_info *method_ref = cp - 1 + cp_index;
	char *class_name = NULL;
	class_name = !interface ? decode_name_index_and_type(cp, method_ref->cp_union.CONSTANT_Methodref_info.class_index, NAME_INDEX) : decode_name_index_and_type(cp, method_ref->cp_union.CONSTANT_InterfaceMethodref_info.class_index, NAME_INDEX);

	return resolve_class(class_name) != NULL ? true : false;
}

char *get_name_method(cp_info *cp, u2 cp_index, u1 interface)
{
	cp_info *methodref = cp - 1 + cp_index;
	char *descriptor = NULL;

	descriptor = !interface ? decode_name_index_and_type(cp, methodref->cp_union.CONSTANT_Methodref_info.name_and_type_index, NAME_AND_TYPE) : decode_name_index_and_type(cp, methodref->cp_union.CONSTANT_InterfaceMethodref_info.name_and_type_index, NAME_AND_TYPE);

	char *pch = strtok(descriptor, ":");
	return pch;
}

char *get_descriptor_method(cp_info *cp, u2 cp_index, u1 interface)
{
	cp_info *methodref = cp - 1 + cp_index;
	char *descriptor = NULL;

	descriptor = !interface ? decode_name_index_and_type(cp, methodref->cp_union.CONSTANT_Methodref_info.name_and_type_index, NAME_AND_TYPE) : decode_name_index_and_type(cp, methodref->cp_union.CONSTANT_InterfaceMethodref_info.name_and_type_index, NAME_AND_TYPE);

	char *pch = strtok(descriptor, ":");
	pch = strtok(NULL, ":");

	return pch;
}

int descriptor_field_validate(char *descriptor)
{
	if (*descriptor == 'L')
		return 5;

	if (strcmp(descriptor, "I") == 0)
		return 0;
	else if (strcmp(descriptor, "F") == 0)
		return 1;
	else if (strcmp(descriptor, "B") == 0)
		return 2;
	else if (strcmp(descriptor, "C") == 0)
		return 3;
	else if (strcmp(descriptor, "S") == 0)
		return 4;
	else if (strcmp(descriptor, "L") == 0)
		return 5;
	else if (strcmp(descriptor, "Z") == 0)
		return 6;
	else if (strcmp(descriptor, "D") == 0)
		return 7;
	else if (strcmp(descriptor, "J") == 0)
		return 8;
	return 0;
}

char *get_class_method(cp_info *cp, u2 cp_index)
{
	cp_info *methodref = cp - 1 + cp_index;
	char *class_name = NULL;

	class_name = decode_name_index_and_type(cp, methodref->cp_union.CONSTANT_Methodref_info.class_index, NAME_INDEX);
	return class_name;
}

frame *transfer_array_stack(frame *prev, frame *new_frame, int *params_count)
{
	operand_stack *aux = create_operand_stack();

	int count = 0;
	while (prev->p->top != NULL && count < (*params_count))
	{
		operand_stack *p = pop_operand(prev->p);

		aux = push_operand(aux, p->top->operand, (void *)p->top->ref, p->top->operand_type);
		count++;
	}

	for (int i = 0; i < count; i++)
	{
		operand_stack *p = pop_operand(aux);

		new_frame->v[i].variable = malloc(sizeof(u4));

		*(new_frame->v[i].variable) = (p->top->operand_type <= 8) ? (u4)p->top->operand : (intptr_t)p->top->ref;

		new_frame->v[i].type = (u1)p->top->operand_type;
	}

	*params_count = count;

	return new_frame;
}

frame *transfer_array_stack_count(frame *f, frame *new_frame, int qtd)
{
	operand_stack *aux = create_operand_stack();

	int count = 0;
	while (count < qtd)
	{
		operand_stack *p = pop_operand(f->p);

		aux = push_operand(aux, p->top->operand, (void *)p->top->ref, p->top->operand_type);
		count++;
	}

	new_frame->v = malloc(count * sizeof(local_variable));

	for (int i = 0; i < count; i++)
	{
		operand_stack *p = pop_operand(aux);

		new_frame->v[i].variable = malloc(sizeof(u4));

		*(new_frame->v[i].variable) = (p->top->operand_type <= 8) ? (u4)p->top->operand : (intptr_t)p->top->ref;

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
	return super;
}

void nop_impl(frame *pair0, u1 pair1, u1 pair2)
{
	return;
}

void aconst_null_impl(frame *f, u1 pair1, u1 pair2)
{
	push_operand(f->p, 0, NULL, REFERENCE_OP);
}

void iconst_m1_impl(frame *f, u1 pair1, u1 pair2)
{
	i4 integer_sign = (i4)-1;
	push_operand(f->p, integer_sign, NULL, INTEGER_OP);
}

void iconst_0_impl(frame *f, u1 pair1, u1 pair2)
{
	i4 integer_sign = (i4)0;
	push_operand(f->p, integer_sign, NULL, INTEGER_OP);
}

void iconst_1_impl(frame *f, u1 pair1, u1 pair2)
{
	i4 integer_sign = (i4)1;
	push_operand(f->p, integer_sign, NULL, INTEGER_OP);
}

void iconst_2_impl(frame *f, u1 pair1, u1 pair2)
{
	i4 integer_sign = (i4)2;
	push_operand(f->p, integer_sign, NULL, INTEGER_OP);
}

void iconst_3_impl(frame *f, u1 pair1, u1 pair2)
{
	i4 integer_sign = (i4)3;
	push_operand(f->p, integer_sign, NULL, INTEGER_OP);
}

void iconst_4_impl(frame *f, u1 pair1, u1 pair2)
{
	i4 integer_sign = (i4)4;
	push_operand(f->p, integer_sign, NULL, INTEGER_OP);
}

void iconst_5_impl(frame *f, u1 pair1, u1 pair2)
{
	i4 integer_sign = (i4)5;
	push_operand(f->p, (u4)integer_sign, NULL, INTEGER_OP);
}

void lconst_0_impl(frame *f, u1 pair1, u1 pair2)
{
	i4 high_bytes = (i4)0;
	push_operand(f->p, high_bytes, NULL, LONG_OP);

	i4 low_bytes = (i4)0;
	push_operand(f->p, low_bytes, NULL, LONG_OP);
}

void lconst_1_impl(frame *f, u1 pair1, u1 pair2)
{
	i4 high_bytes = (i4)0;
	push_operand(f->p, high_bytes, NULL, LONG_OP);

	i4 low_bytes = (i4)1;
	push_operand(f->p, low_bytes, NULL, LONG_OP);
}

void fconst_0_impl(frame *f, u1 pair1, u1 pair2)
{
	i4 float_bytes = (i4)0;
	push_operand(f->p, float_bytes, NULL, FLOAT_OP);
}

void fconst_1_impl(frame *f, u1 pair1, u1 pair2)
{
	i4 float_bytes = 0x3f800000;
	push_operand(f->p, float_bytes, NULL, FLOAT_OP);
}

void fconst_2_impl(frame *f, u1 pair1, u1 pair2)
{
	i4 float_bytes = 0x40000000;
	push_operand(f->p, float_bytes, NULL, FLOAT_OP);
}

void dconst_0_impl(frame *f, u1 pair1, u1 pair2)
{
	i4 high_bytes = (i4)0;
	push_operand(f->p, high_bytes, NULL, DOUBLE_OP);

	i4 low_bytes = (i4)0;
	push_operand(f->p, low_bytes, NULL, DOUBLE_OP);
}

void dconst_1_impl(frame *f, u1 pair1, u1 pair2)
{
	i4 high_bytes = 0x3FF00000;
	push_operand(f->p, high_bytes, NULL, DOUBLE_OP);

	i4 low_bytes = (i4)0;
	push_operand(f->p, low_bytes, NULL, DOUBLE_OP);
}

void bipush_impl(frame *f, u1 byte, u1 pair1)
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

void ldc_impl(frame *f, u1 index_byte1, u1 pair2)
{
	cp_info *item = f->cp - 1 + index_byte1;
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

void ldc_w_impl(frame *f, u1 index_byte1, u1 index_byte2)
{
	u2 index_cp = normalize_index(index_byte1, index_byte2);
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

void ldc2_w_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{
	int8_t v1 = (int8_t)branch_byte1;
	int8_t v2 = (int8_t)branch_byte2;

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

void iload_impl(frame *f, u1 index, u1 pair1)
{
	push_operand(f->p, (i4) * (f->v[index].variable), NULL, INTEGER_OP);
}

void lload_impl(frame *f, u1 index, u1 pair1)
{
	push_operand(f->p, (i4) * (f->v[index].variable), NULL, LONG_OP);
	push_operand(f->p, (i4) * (f->v[index + 1].variable), NULL, LONG_OP);
}

void fload_impl(frame *f, u1 index, u1 pair1)
{
	push_operand(f->p, (i4) * (f->v[index].variable), NULL, FLOAT_OP);
}

void dload_impl(frame *f, u1 index, u1 pair2)
{
	push_operand(f->p, (i4) * (f->v[index].variable), NULL, DOUBLE_OP);
	push_operand(f->p, (i4) * (f->v[index + 1].variable), NULL, DOUBLE_OP);
}

void aload_impl(frame *f, u1 index, u1 pair1)
{
	push_operand(f->p, -INT_MAX, (void *)(intptr_t) * (f->v[index].variable), f->v[index].type);
}

void iload_0_impl(frame *f, u1 pair1, u1 pair2)
{
	push_operand(f->p, (i4) * (f->v[0].variable), NULL, INTEGER_OP);
}

void iload_1_impl(frame *f, u1 pair1, u1 pair2)
{
	push_operand(f->p, (i4) * (f->v[1].variable), NULL, INTEGER_OP);
}

void iload_2_impl(frame *f, u1 pair1, u1 pair2)
{
	push_operand(f->p, (i4) * (f->v[2].variable), NULL, INTEGER_OP);
}

void iload_3_impl(frame *f, u1 pair1, u1 pair2)
{
	push_operand(f->p, (i4) * (f->v[3].variable), NULL, INTEGER_OP);
}

void lload_0_impl(frame *f, u1 pair, u1 pair2)
{
	push_operand(f->p, (i4) * (f->v[0].variable), NULL, LONG_OP);
	push_operand(f->p, (i4) * (f->v[1].variable), NULL, LONG_OP);
}

void lload_1_impl(frame *f, u1 pair, u1 pair2)
{
	push_operand(f->p, (i4) * (f->v[1].variable), NULL, LONG_OP);
	push_operand(f->p, (i4) * (f->v[2].variable), NULL, LONG_OP);
}

void lload_2_impl(frame *f, u1 pair, u1 pair2)
{
	push_operand(f->p, (i4) * (f->v[2].variable), NULL, LONG_OP);
	push_operand(f->p, (i4) * (f->v[3].variable), NULL, LONG_OP);
}

void lload_3_impl(frame *f, u1 pair, u1 pair2)
{
	push_operand(f->p, (i4) * (f->v[3].variable), NULL, LONG_OP);
	push_operand(f->p, (i4) * (f->v[4].variable), NULL, LONG_OP);
}

void fload_0_impl(frame *f, u1 par1, u1 par2)
{
	push_operand(f->p, (i4) * (f->v[0].variable), NULL, FLOAT_OP);
}

void fload_1_impl(frame *f, u1 par1, u1 par2)
{
	push_operand(f->p, (i4) * (f->v[1].variable), NULL, FLOAT_OP);
}

void fload_2_impl(frame *f, u1 par1, u1 par2)
{
	push_operand(f->p, (i4) * (f->v[2].variable), NULL, FLOAT_OP);
}

void fload_3_impl(frame *f, u1 pair1, u1 pair2)
{
	push_operand(f->p, (i4) * (f->v[3].variable), NULL, FLOAT_OP);
}

void dload_0_impl(frame *f, u1 pair1, u1 pair2)
{
	push_operand(f->p, (i4) * (f->v[0].variable), NULL, DOUBLE_OP); //high
	push_operand(f->p, (i4) * (f->v[1].variable), NULL, DOUBLE_OP); //low
}

void dload_1_impl(frame *f, u1 pair1, u1 pair2)
{
	push_operand(f->p, (i4) * (f->v[1].variable), NULL, DOUBLE_OP);
	push_operand(f->p, (i4) * (f->v[2].variable), NULL, DOUBLE_OP);
}

void dload_2_impl(frame *f, u1 pair1, u1 pair2)
{
	push_operand(f->p, (i4) * (f->v[2].variable), NULL, DOUBLE_OP);
	push_operand(f->p, (i4) * (f->v[3].variable), NULL, DOUBLE_OP);
}

void dload_3_impl(frame *f, u1 pair1, u1 pair2)
{
	push_operand(f->p, (i4) * (f->v[3].variable), NULL, DOUBLE_OP);
	push_operand(f->p, (i4) * (f->v[4].variable), NULL, DOUBLE_OP);
}

void aload_0_impl(frame *f, u1 pair1, u1 pair2)
{
	push_operand(f->p, -INT_MAX, (void *)(intptr_t) * (f->v[0].variable), f->v[0].type);
}

void aload_1_impl(frame *f, u1 pair1, u1 pair2)
{
	push_operand(f->p, -INT_MAX, (void *)(intptr_t) * (f->v[1].variable), f->v[1].type);
}

void aload_2_impl(frame *f, u1 pair1, u1 pair2)
{
	push_operand(f->p, -INT_MAX, (void *)(intptr_t) * (f->v[2].variable), f->v[2].type);
}

void aload_3_impl(frame *f, u1 pair1, u1 pair2)
{
	push_operand(f->p, -INT_MAX, (void *)(intptr_t) * (f->v[3].variable), f->v[3].type);
}

void iaload_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *index = pop_operand(f->p);
	operand_stack *ref = pop_operand(f->p);

	u4 *address = (ref->top->ref) + (index->top->operand * sizeof(i4));

	push_operand(f->p, *address, NULL, INTEGER_OP);
}

void laload_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *index = pop_operand(f->p);
	operand_stack *ref = pop_operand(f->p);

	u4 *address = (ref->top->ref) + (index->top->operand * 2 * sizeof(i4));
	//Verificar ordem
	push_operand(f->p, *address, NULL, LONG_OP);
	address++;
	push_operand(f->p, *(address), NULL, LONG_OP);
}

void faload_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *index = pop_operand(f->p);
	operand_stack *ref = pop_operand(f->p);

	u4 *address = (ref->top->ref) + (index->top->operand * sizeof(i4));

	push_operand(f->p, *address, NULL, FLOAT_OP);
}

void daload_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *index = pop_operand(f->p);
	operand_stack *ref = pop_operand(f->p);

	u4 *address = (ref->top->ref) + (index->top->operand * 2 * sizeof(u4));

	//Verificar ordem
	push_operand(f->p, *address, NULL, DOUBLE_OP);
	address++;
	push_operand(f->p, *(address), NULL, DOUBLE_OP);
}

void aaload_impl(frame *f, u1 pair1, u1 pair2)
{
}

void baload_impl(frame *f, u1 pair1, u1 pair2)
{

	operand_stack *index = pop_operand(f->p);
	operand_stack *ref = pop_operand(f->p);

	i1 *address = (ref->top->ref) + (index->top->operand * sizeof(i1));
	push_operand(f->p, *address, NULL, BYTE_OP);
}

void caload_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *index = pop_operand(f->p);
	operand_stack *ref = pop_operand(f->p);

	u2 *address = (ref->top->ref) + (index->top->operand * sizeof(u2));
	push_operand(f->p, *address, NULL, CHAR_OP);
}

void saload_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *index = pop_operand(f->p);
	operand_stack *ref = pop_operand(f->p);

	i2 *address = (ref->top->ref) + (index->top->operand * sizeof(i2));

	push_operand(f->p, *address, NULL, SHORT_OP);
}

void istore_impl(frame *f, u1 index, u1 pair1)
{
	operand_stack *value = pop_operand(f->p);
	*(f->v[index].variable) = (i4)value->top->operand;
}

void lstore_impl(frame *f, u1 index, u1 pair1)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[index].variable) = (u4)high_bytes->top->operand;
	*(f->v[index + 1].variable) = (u4)low_bytes->top->operand;
}

void fstore_impl(frame *f, u1 index, u1 pair1)
{
	operand_stack *value = pop_operand(f->p);
	*(f->v[index].variable) = (u4)value->top->operand;
}

void dstore_impl(frame *f, u1 index, u1 pair1)
{	
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[index].variable) = (u4)high_bytes->top->operand;
	*(f->v[index + 1].variable) = (u4)low_bytes->top->operand;
}

void astore_impl(frame *f, u1 index, u1 pair1)
{
	operand_stack *value = pop_operand(f->p);
	*(f->v[index].variable) = (intptr_t)value->top->ref;
	f->v[index].type = value->top->operand_type;
}

void istore_0_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);
	*(f->v[0].variable) = (i4)value->top->operand;
}

void istore_1_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);
	*(f->v[1].variable) = (u4)value->top->operand;
}

void istore_2_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);
	*(f->v[2].variable) = (u4)value->top->operand;
}

void istore_3_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);
	*(f->v[3].variable) = (u4)value->top->operand;
}

void lstore_0_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[0].variable) = (u4)high_bytes->top->operand;
	*(f->v[1].variable) = (u4)low_bytes->top->operand;
}

void lstore_1_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[1].variable) = (u4)high_bytes->top->operand;
	*(f->v[2].variable) = (u4)low_bytes->top->operand;
}

void lstore_2_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[2].variable) = (u4)high_bytes->top->operand;
	*(f->v[3].variable) = (u4)low_bytes->top->operand;
}

void lstore_3_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[3].variable) = (u4)high_bytes->top->operand;
	*(f->v[4].variable) = (u4)low_bytes->top->operand;
}

void fstore_0_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);
	*(f->v[0].variable) = (u4)value->top->operand;
}

void fstore_1_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);
	*(f->v[1].variable) = (u4)value->top->operand;
}

void fstore_2_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);
	*(f->v[2].variable) = (u4)value->top->operand;
}

void fstore_3_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);
	*(f->v[3].variable) = (u4)value->top->operand;
}

void dstore_0_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[0].variable) = (u4)high_bytes->top->operand;
	*(f->v[1].variable) = (u4)low_bytes->top->operand;
}

void dstore_1_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[1].variable) = (u4)high_bytes->top->operand;
	*(f->v[2].variable) = (u4)low_bytes->top->operand;
}

void dstore_2_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[2].variable) = (u4)high_bytes->top->operand;
	*(f->v[3].variable) = (u4)low_bytes->top->operand;
}

void dstore_3_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	*(f->v[3].variable) = (u4)high_bytes->top->operand;
	*(f->v[4].variable) = (u4)low_bytes->top->operand;
}

void astore_0_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);
	*(f->v[0].variable) = (intptr_t)value->top->ref;
	f->v[0].type = value->top->operand_type;
}

void astore_1_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);

	*(f->v[1].variable) = (intptr_t)value->top->ref;
	f->v[1].type = value->top->operand_type;
}

void astore_2_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);
	*(f->v[2].variable) = (intptr_t)value->top->ref;
	f->v[2].type = value->top->operand_type;
}

void astore_3_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);
	*(f->v[3].variable) = (intptr_t)value->top->ref;
	f->v[3].type = value->top->operand_type;
}

void iastore_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);
	operand_stack *index = pop_operand(f->p);
	operand_stack *array = pop_operand(f->p);

	i4 *address_array = ((array->top->ref) + (index->top->operand * sizeof(i4)));
	*address_array = value->top->operand;
}

void lastore_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);
	operand_stack *index = pop_operand(f->p);
	operand_stack *array = pop_operand(f->p);

	i4 *address_array = ((array->top->ref) + (index->top->operand * 2 * sizeof(i4)));

	*address_array = high_bytes->top->operand;

	address_array++;

	*address_array = low_bytes->top->operand;
}

void fastore_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);
	operand_stack *index = pop_operand(f->p);
	operand_stack *array = pop_operand(f->p);

	i4 *address_array = ((array->top->ref) + (index->top->operand * sizeof(i4)));
	*address_array = value->top->operand;
}

void dastore_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);
	operand_stack *index = pop_operand(f->p);
	operand_stack *array = pop_operand(f->p);

	u4 *address_array = ((array->top->ref) + (index->top->operand * 2 * sizeof(u4)));

	*address_array = high_bytes->top->operand;

	address_array++;

	*address_array = low_bytes->top->operand;
}

void bastore_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);
	operand_stack *index = pop_operand(f->p);
	operand_stack *array = pop_operand(f->p);

	i1 *address_array = ((array->top->ref) + (index->top->operand * sizeof(i1)));
	*address_array = value->top->operand;
}

void castore_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);
	operand_stack *index = pop_operand(f->p);
	operand_stack *array = pop_operand(f->p);

	u2 *address_array = ((array->top->ref) + (index->top->operand * sizeof(u2)));
	*address_array = value->top->operand;
}

void sastore_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);
	operand_stack *index = pop_operand(f->p);
	operand_stack *array = pop_operand(f->p);

	i2 *address_array = ((array->top->ref) + (index->top->operand * sizeof(i2)));
	*address_array = value->top->operand;
}

void aastore_impl(frame *f, u1 pair1, u1 pair2)
{
}

operand_stack *pop_impl(frame *f)
{
	operand_stack *value = pop_operand(f->p);

	return (value);
}

operand_stack **pop2_impl(frame *f)
{
	pop_operand(f->p);
	pop_operand(f->p);
	return NULL;
}

void pop_ghost(frame *f, u1 pair1, u1 pair2)
{
	pop_impl(f);
}

void pop2_ghost(frame *pair0, u1 pair1, u1 pair2)
{
}

void dup_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = top_operand(f->p);
	f->p = push_operand(f->p, value->top->operand, value->top->ref, value->top->operand_type);
}

void dup_x1_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	f->p = push_operand(f->p, value1->top->operand, value1->top->ref, value1->top->operand_type);
	f->p = push_operand(f->p, value2->top->operand, value2->top->ref, value2->top->operand_type);
	f->p = push_operand(f->p, value1->top->operand, value1->top->ref, value1->top->operand_type);
}

void dup_x2_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);
	operand_stack *value3 = pop_operand(f->p);

	f->p = push_operand(f->p, value1->top->operand, value1->top->ref, value1->top->operand_type);
	f->p = push_operand(f->p, value3->top->operand, value3->top->ref, value3->top->operand_type);
	f->p = push_operand(f->p, value2->top->operand, value2->top->ref, value2->top->operand_type);
	f->p = push_operand(f->p, value1->top->operand, value1->top->ref, value1->top->operand_type);
}

void dup2_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	f->p = push_operand(f->p, value2->top->operand, value2->top->ref, value2->top->operand_type);
	f->p = push_operand(f->p, value1->top->operand, value1->top->ref, value1->top->operand_type);
	f->p = push_operand(f->p, value2->top->operand, value2->top->ref, value2->top->operand_type);
	f->p = push_operand(f->p, value1->top->operand, value1->top->ref, value1->top->operand_type);
}

void dup2_x1_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);
	operand_stack *value3 = pop_operand(f->p);

	f->p = push_operand(f->p, value3->top->operand, value3->top->ref, value3->top->operand_type);
	f->p = push_operand(f->p, value2->top->operand, value2->top->ref, value2->top->operand_type);
	f->p = push_operand(f->p, value1->top->operand, value1->top->ref, value1->top->operand_type);
}

void dup2_x2_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);
	operand_stack *value3 = pop_operand(f->p);
	operand_stack *value4 = pop_operand(f->p);

	f->p = push_operand(f->p, value4->top->operand, value4->top->ref, value4->top->operand_type);
	f->p = push_operand(f->p, value3->top->operand, value3->top->ref, value3->top->operand_type);
	f->p = push_operand(f->p, value2->top->operand, value2->top->ref, value2->top->operand_type);
	f->p = push_operand(f->p, value1->top->operand, value1->top->ref, value1->top->operand_type);
}

void swap_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	f->p = push_operand(f->p, value1->top->operand, value1->top->ref, value1->top->operand_type);
	f->p = push_operand(f->p, value2->top->operand, value2->top->ref, value2->top->operand_type);
}

void iadd_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	operand_stack *value3 = create_operand_stack();
	value3 = push_operand(value3, value1->top->operand + value2->top->operand, NULL, value1->top->operand_type);
	value3->top->operand_type = value1->top->operand_type;
	f->p = push_operand(f->p, value3->top->operand, NULL, value3->top->operand_type);
}

/////////////// REVISAR newMENTE /////////////////////////////////

void ladd_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes1 = pop_operand(f->p);
	operand_stack *high_bytes1 = pop_operand(f->p);

	i8 long1 = ((u8)high_bytes1->top->operand << 32) | low_bytes1->top->operand;

	operand_stack *low_bytes2 = pop_operand(f->p);
	operand_stack *high_bytes2 = pop_operand(f->p);

	i8 long2 = ((u8)high_bytes2->top->operand << 32) | low_bytes2->top->operand;

	i8 result = long1 + long2;

	f->p = push_operand(f->p, (u4)(result >> 32), NULL, LONG_OP);
	f->p = push_operand(f->p, (u4)result, NULL, LONG_OP);
}

void fadd_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	float op1 = decode_float_value(value1->top->operand);
	float op2 = decode_float_value(value2->top->operand);

	float f_sum = op1 + op2;
	u4 sum = (u4)(*(u4 *)&f_sum);
	u4 result = (signal(sum) << 31) | (exponent(sum) << 23) | mantissa(sum);
	f->p = push_operand(f->p, result, NULL, FLOAT_OP);
}
void dadd_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1_low = pop_operand(f->p);
	operand_stack *value1_high = pop_operand(f->p);
	operand_stack *value2_low = pop_operand(f->p);
	operand_stack *value2_high = pop_operand(f->p);

	double op1 = decode_double_value(value1_high->top->operand, value1_low->top->operand);
	double op2 = decode_double_value(value2_high->top->operand, value2_low->top->operand);

	double d_sum = op1 + op2;
	u8 sum = (u8)(*(u8 *)&d_sum);
	u8 result = (signal_d(sum) << 63) | (exponent_d(sum) << 52) | mantissa_d(sum);

	f->p = push_operand(f->p, (u4)(result >> 32), NULL, DOUBLE_OP);
	f->p = push_operand(f->p, (u4)result, NULL, DOUBLE_OP);
}

void isub_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	i4 result = value2->top->operand - value1->top->operand;
	f->p = push_operand(f->p, result, NULL, INTEGER_OP);
}

void lsub_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes1 = pop_operand(f->p);
	operand_stack *high_bytes1 = pop_operand(f->p);

	i8 long1 = ((u8)high_bytes1->top->operand << 32) | low_bytes1->top->operand;

	operand_stack *low_bytes2 = pop_operand(f->p);
	operand_stack *high_bytes2 = pop_operand(f->p);

	i8 long2 = ((u8)high_bytes2->top->operand << 32) | low_bytes2->top->operand;

	i8 result = long2 - long1;

	f->p = push_operand(f->p, (u4)(result >> 32), NULL, LONG_OP);
	f->p = push_operand(f->p, (u4)result, NULL, LONG_OP);
}

void fsub_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	float op1 = decode_float_value(value1->top->operand);
	float op2 = decode_float_value(value2->top->operand);
	float f_sub = op2 - op1;
	u4 sub = (u4)(*(u4 *)&f_sub);
	u4 result = (signal(sub) << 31) | (exponent(sub) << 23) | mantissa(sub);
	f->p = push_operand(f->p, result, NULL, FLOAT_OP);
}

void dsub_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1_low = pop_operand(f->p);
	operand_stack *value1_high = pop_operand(f->p);
	operand_stack *value2_low = pop_operand(f->p);
	operand_stack *value2_high = pop_operand(f->p);

	double op1 = decode_double_value(value1_high->top->operand, value1_low->top->operand);
	double op2 = decode_double_value(value2_high->top->operand, value2_low->top->operand);

	double d_sub = op2 - op1;
	u8 sub = (u8)(*(u8 *)&d_sub);
	u8 result = (signal_d(sub) << 63) | (exponent_d(sub) << 52) | mantissa_d(sub);

	f->p = push_operand(f->p, (u4)(result >> 32), NULL, DOUBLE_OP);
	f->p = push_operand(f->p, (u4)result, NULL, DOUBLE_OP);
}

void imul_impl(frame *f, u1 pair1, u1 pair2)
{

	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	i4 result = value1->top->operand * value2->top->operand;
	f->p = push_operand(f->p, result, NULL, INTEGER_OP);
}

void lmul_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes1 = pop_operand(f->p);
	operand_stack *high_bytes1 = pop_operand(f->p);

	i8 long1 = ((u8)high_bytes1->top->operand << 32) | low_bytes1->top->operand;

	operand_stack *low_bytes2 = pop_operand(f->p);
	operand_stack *high_bytes2 = pop_operand(f->p);

	i8 long2 = ((u8)high_bytes2->top->operand << 32) | low_bytes2->top->operand;

	i8 result = long1 * long2;

	f->p = push_operand(f->p, (u4)(result >> 32), NULL, LONG_OP);
	f->p = push_operand(f->p, (u4)result, NULL, LONG_OP);
}

void fmul_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	float op1 = decode_float_value(value1->top->operand);
	float op2 = decode_float_value(value2->top->operand);
	float f_res = op1 * op2;
	u4 res = (u4)(*(u4 *)&f_res);
	u4 result = (signal(res) << 31) | (exponent(res) << 23) | mantissa(res);
	f->p = push_operand(f->p, result, NULL, FLOAT_OP);
}

void dmul_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1_low = pop_operand(f->p);
	operand_stack *value1_high = pop_operand(f->p);
	operand_stack *value2_low = pop_operand(f->p);
	operand_stack *value2_high = pop_operand(f->p);

	double op1 = decode_double_value(value1_high->top->operand, value1_low->top->operand);
	double op2 = decode_double_value(value2_high->top->operand, value2_low->top->operand);

	double d_res = op1 * op2;
	u8 res = (u8)(*(u8 *)&d_res);
	u8 result = (signal_d(res) << 63) | (exponent_d(res) << 52) | mantissa_d(res);

	f->p = push_operand(f->p, (u4)(result >> 32), NULL, DOUBLE_OP);
	f->p = push_operand(f->p, (u4)result, NULL, DOUBLE_OP);
}

void idiv_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	i4 result;
	void *message_error_div = NULL;
	char *message_error = "java.lang.ArithmeticException: / by zero";

	if (value1->top->operand != 0)
	{
		// Se os tipos dos valores forem iguais, e se esse tipo for inteiro
		result = value2->top->operand / value1->top->operand;
		f->p = push_operand(f->p, result, message_error_div, INTEGER_OP);
	}
	else
	{
		jvm->exception = 1;
		//Caso ocorra uma exception, temos que result deve ser 1.
		result = value2->top->operand / value2->top->operand;
		strcpy(jvm->exception_name, "java/lang/ArithmeticException");

		message_error_div = message_error;
		f->p = push_operand(f->p, result, message_error_div, REFERENCE_OP);
	}
}

void ldiv_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes1 = pop_operand(f->p);
	operand_stack *high_bytes1 = pop_operand(f->p);

	i8 long1 = ((u8)high_bytes1->top->operand << 32) | low_bytes1->top->operand;

	operand_stack *low_bytes2 = pop_operand(f->p);
	operand_stack *high_bytes2 = pop_operand(f->p);

	i8 long2 = ((u8)high_bytes2->top->operand << 32) | low_bytes2->top->operand;

	i8 result = long2 / long1;

	f->p = push_operand(f->p, (u4)(result >> 32), NULL, LONG_OP);
	f->p = push_operand(f->p, (u4)result, NULL, LONG_OP);
}

void fdiv_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	float op1 = decode_float_value(value1->top->operand);
	float op2 = decode_float_value(value2->top->operand);
	float f_res = op2 / op1;
	u4 res = (u4)(*(u4 *)&f_res);
	u4 result = (signal(res) << 31) | (exponent(res) << 23) | mantissa(res);
	f->p = push_operand(f->p, result, NULL, FLOAT_OP);
}

void ddiv_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1_low = pop_operand(f->p);
	operand_stack *value1_high = pop_operand(f->p);
	operand_stack *value2_low = pop_operand(f->p);
	operand_stack *value2_high = pop_operand(f->p);

	double op1 = decode_double_value(value1_high->top->operand, value1_low->top->operand);
	double op2 = decode_double_value(value2_high->top->operand, value2_low->top->operand);

	double d_res = op2 / op1;
	u8 res = (u8)(*(u8 *)&d_res);
	u8 result = (signal_d(res) << 63) | (exponent_d(res) << 52) | mantissa_d(res);

	f->p = push_operand(f->p, (u4)(result >> 32), NULL, DOUBLE_OP);
	f->p = push_operand(f->p, (u4)result, NULL, DOUBLE_OP);
}

void irem_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	if (value1->top->operand == 0)
	{
		// Lançar Arithmetic Exception
	}

	i4 value_push = value2->top->operand - (value2->top->operand / value1->top->operand) * value1->top->operand;

	f->p = push_operand(f->p, value_push, NULL, INTEGER_OP);
}

void lrem_impl(frame *f, u1 pair1, u1 pair2)
{

	operand_stack *low_bytes1 = pop_operand(f->p);
	operand_stack *high_bytes1 = pop_operand(f->p);

	i8 long1 = ((u8)high_bytes1->top->operand << 32) | low_bytes1->top->operand;

	operand_stack *low_bytes2 = pop_operand(f->p);
	operand_stack *high_bytes2 = pop_operand(f->p);

	i8 long2 = ((u8)high_bytes2->top->operand << 32) | low_bytes2->top->operand;

	i8 result = long2 - (long2 / long1) * long1;

	f->p = push_operand(f->p, (u4)(result >> 32), NULL, LONG_OP);
	f->p = push_operand(f->p, (u4)result, NULL, LONG_OP);
}

void frem_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	float v1 = decode_float_value(value1->top->operand);
	float v2 = decode_float_value(value2->top->operand);

	float result_aux = fmodf(v2, v1);
	u4 result = (u4)(*(u4 *)&result_aux);

	f->p = push_operand(f->p, result, NULL, FLOAT_OP);
}

void drem_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1_low = pop_operand(f->p);
	operand_stack *value1_high = pop_operand(f->p);
	operand_stack *value2_low = pop_operand(f->p);
	operand_stack *value2_high = pop_operand(f->p);

	double v1 = decode_double_value(value1_high->top->operand, value1_low->top->operand);
	double v2 = decode_double_value(value2_high->top->operand, value2_low->top->operand);

	double result_aux = fmod(v2, v1);

	u8 result = (u8)(*(u8 *)&result_aux);

	f->p = push_operand(f->p, (u4)(result >> 32), NULL, DOUBLE_OP);
	f->p = push_operand(f->p, (u4)result, NULL, DOUBLE_OP);
}

void ineg_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);

	i4 result = -value1->top->operand;
	f->p = push_operand(f->p, result, NULL, INTEGER_OP);
}

void lneg_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes1 = pop_operand(f->p);
	operand_stack *high_bytes1 = pop_operand(f->p);

	i8 long1 = ((u8)high_bytes1->top->operand << 32) | low_bytes1->top->operand;

	i8 result = -long1;

	f->p = push_operand(f->p, (u4)(result >> 32), NULL, LONG_OP);
	f->p = push_operand(f->p, (u4)result, NULL, LONG_OP);
}

void fneg_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);

	u4 op1 = value1->top->operand;

	u4 result = op1 ^ 0X80000000;

	f->p = push_operand(f->p, result, NULL, FLOAT_OP);
}

void dneg_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1_low = pop_operand(f->p);
	operand_stack *value1_high = pop_operand(f->p);

	u4 high_bytes = value1_high->top->operand;
	u4 low_bytes = value1_low->top->operand;

	u4 result = high_bytes ^ 0X80000000;

	f->p = push_operand(f->p, result, NULL, DOUBLE_OP);
	f->p = push_operand(f->p, low_bytes, NULL, DOUBLE_OP);
}

void ishl_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	i4 s = (value1->top->operand << 27) >> 27;

	i4 result = value2->top->operand << s;

	f->p = push_operand(f->p, result, NULL, value1->top->operand_type);
}

void lshl_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value2 = pop_operand(f->p);

	i4 s = (value2->top->operand << 27) >> 27;

	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	i8 long1 = ((i8)high_bytes->top->operand << 32) | low_bytes->top->operand;

	i8 result = long1 << s;

	f->p = push_operand(f->p, (u4)(result >> 32), NULL, LONG_OP);
	f->p = push_operand(f->p, (u4)result, NULL, LONG_OP);
}

void ishr_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	i4 s = (i4)(value1->top->operand << 27) >> 27;

	i4 result = (i4)value2->top->operand >> s;

	f->p = push_operand(f->p, result, NULL, value1->top->operand_type);
}

void lshr_impl(frame *f, u1 pair1, u1 pair2)
{

	operand_stack *value2 = pop_operand(f->p);

	i4 s = (value2->top->operand << 27) >> 27;

	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	i8 long1 = ((i8)high_bytes->top->operand << 32) | low_bytes->top->operand;

	i8 result = long1 >> s;

	f->p = push_operand(f->p, (u4)(result >> 32), NULL, LONG_OP);
	f->p = push_operand(f->p, (u4)result, NULL, LONG_OP);
}

void iushr_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	u4 s = (u4)(value2->top->operand << 27) >> 27;

	u4 result = (u4)value1->top->operand >> s;

	f->p = push_operand(f->p, (i4)result, NULL, value1->top->operand_type);
}

void lushr_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value2 = pop_operand(f->p);

	u4 s = (value2->top->operand << 27) >> 27;

	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	u8 long1 = ((u8)high_bytes->top->operand << 32) | low_bytes->top->operand;

	u8 result = long1 >> s;

	f->p = push_operand(f->p, (u4)(result >> 32), NULL, LONG_OP);
	f->p = push_operand(f->p, (u4)result, NULL, LONG_OP);
}

void iand_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	i4 result = value2->top->operand & value1->top->operand;

	f->p = push_operand(f->p, result, NULL, value1->top->operand_type);
}

void land_impl(frame *f, u1 pair1, u1 pair2)
{

	operand_stack *low_bytes1 = pop_operand(f->p);
	operand_stack *high_bytes1 = pop_operand(f->p);

	u8 long1 = ((u8)high_bytes1->top->operand << 32) | low_bytes1->top->operand;

	operand_stack *low_bytes2 = pop_operand(f->p);
	operand_stack *high_bytes2 = pop_operand(f->p);

	u8 long2 = ((u8)high_bytes2->top->operand << 32) | low_bytes2->top->operand;

	u8 result = long2 & long1;

	f->p = push_operand(f->p, (u4)(result >> 32), NULL, LONG_OP);
	f->p = push_operand(f->p, (u4)result, NULL, LONG_OP);
}

void ior_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	i4 result = value1->top->operand | value2->top->operand;

	f->p = push_operand(f->p, result, NULL, value1->top->operand_type);
}

void lor_impl(frame *f, u1 pair1, u1 pair2)
{

	operand_stack *low_bytes1 = pop_operand(f->p);
	operand_stack *high_bytes1 = pop_operand(f->p);

	u8 long1 = ((u8)high_bytes1->top->operand << 32) | low_bytes1->top->operand;

	operand_stack *low_bytes2 = pop_operand(f->p);
	operand_stack *high_bytes2 = pop_operand(f->p);

	u8 long2 = ((u8)high_bytes2->top->operand << 32) | low_bytes2->top->operand;

	u8 result = long2 | long1;

	f->p = push_operand(f->p, (u4)(result >> 32), NULL, LONG_OP);
	f->p = push_operand(f->p, (u4)result, NULL, LONG_OP);
}

void ixor_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	i4 result = value1->top->operand ^ value2->top->operand;

	f->p = push_operand(f->p, result, NULL, INTEGER_OP);
}

void lxor_impl(frame *f, u1 pair1, u1 pair2)
{

	operand_stack *low_bytes1 = pop_operand(f->p);
	operand_stack *high_bytes1 = pop_operand(f->p);

	u8 long1 = ((u8)high_bytes1->top->operand << 32) | low_bytes1->top->operand;

	operand_stack *low_bytes2 = pop_operand(f->p);
	operand_stack *high_bytes2 = pop_operand(f->p);

	u8 long2 = ((u8)high_bytes2->top->operand << 32) | low_bytes2->top->operand;

	u8 result = long2 ^ long1;

	f->p = push_operand(f->p, (u4)(result >> 32), NULL, LONG_OP);
	f->p = push_operand(f->p, (u4)result, NULL, LONG_OP);
}

void iinc_impl(frame *f, u1 index, i1 constante)
{
	i4 constant_integer = (i4)constante;
	*(f->v[index].variable) += constant_integer;
}

void iinc_ghost(frame *pair0, u1 pair1, u1 pair2)
{
	i1 value = (i1)pair2;
	iinc_impl(pair0, pair1, value);
}

void iinc_wide_ghost(frame *f, u1 index_byte1, u1 index_byte2, u1 const_byte1, u1 const_byte2)
{
	u2 index_byte = normalize_index(index_byte1, index_byte2);
	i2 const_byte = (i2)((i1)(const_byte1 << 8) | (i1)const_byte2);

	iinc_wide(f, index_byte, const_byte);
}

void iinc_wide(frame *f, u2 index_byte, i2 const_byte)
{
	i4 constant_integer = (i4)const_byte;
	f->v[index_byte].variable += constant_integer;
}

void i2l_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	i4 value = (i4)value1->top->operand;

	i8 value_long = (i8)value;

	f->p = push_operand(f->p, (u4)(value_long >> 32), NULL, LONG_OP);
	f->p = push_operand(f->p, (u4)value_long, NULL, LONG_OP);
}

void i2f_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	float value = (float)value1->top->operand;
	u4 flo = (u4)(*(u4 *)&value);
	f->p = push_operand(f->p, flo, NULL, FLOAT_OP);
}

void i2d_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	double value = (double)value1->top->operand;

	u8 doub = (u8)(*(u8 *)&value);

	f->p = push_operand(f->p, (u4)(doub >> 32), NULL, DOUBLE_OP);
	f->p = push_operand(f->p, (u4)doub, NULL, DOUBLE_OP);
}

void l2i_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes1 = pop_operand(f->p);
	pop_operand(f->p);

	f->p = push_operand(f->p, low_bytes1->top->operand, NULL, INTEGER_OP);
}

void l2f_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes1 = pop_operand(f->p);
	operand_stack *high_bytes1 = pop_operand(f->p);

	i8 long1 = ((u8)high_bytes1->top->operand << 32) | low_bytes1->top->operand;
	float flo = (float)long1;
	u4 value = (u4)(*(u4 *)&flo);
	f->p = push_operand(f->p, value, NULL, FLOAT_OP);
}

void l2d_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes1 = pop_operand(f->p);
	operand_stack *high_bytes1 = pop_operand(f->p);

	i8 long1 = ((u8)high_bytes1->top->operand << 32) | low_bytes1->top->operand;
	double value = (double)long1;
	u8 doub = (u8)(*(u8 *)&value);

	f->p = push_operand(f->p, (u4)(doub >> 32), NULL, DOUBLE_OP);
	f->p = push_operand(f->p, (u4)doub, NULL, DOUBLE_OP);
}

void f2i_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	float value = decode_float_value(value1->top->operand);
	f->p = push_operand(f->p, (i4)value, NULL, INTEGER_OP);
}

void f2l_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	float value = decode_float_value(value1->top->operand);

	i8 value_long = (i8)value;

	f->p = push_operand(f->p, (u4)(value_long >> 32), NULL, LONG_OP);
	f->p = push_operand(f->p, (u4)value_long, NULL, LONG_OP);
}

void f2d_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	double value = (double)decode_float_value(value1->top->operand);

	u8 doub = (u8)(*(u8 *)&value);

	f->p = push_operand(f->p, (u4)(doub >> 32), NULL, DOUBLE_OP);
	f->p = push_operand(f->p, (u4)doub, NULL, DOUBLE_OP);
}

void d2i_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1_low = pop_operand(f->p);
	operand_stack *value1_high = pop_operand(f->p);

	u4 high_bytes = value1_high->top->operand;
	u4 low_bytes = value1_low->top->operand;

	double value = decode_double_value(high_bytes, low_bytes);
	f->p = push_operand(f->p, (i4)value, NULL, INTEGER_OP);
}

void d2l_impl(frame *f, u1 pair1, u1 pair2)
{

	operand_stack *value1_low = pop_operand(f->p);
	operand_stack *value1_high = pop_operand(f->p);

	u4 high_bytes = value1_high->top->operand;
	u4 low_bytes = value1_low->top->operand;

	double value = decode_double_value(high_bytes, low_bytes);

	i8 value_long = (i8)value;

	f->p = push_operand(f->p, (u4)(value_long >> 32), NULL, LONG_OP);
	f->p = push_operand(f->p, (u4)value_long, NULL, LONG_OP);
}

void d2f_impl(frame *f, u1 pair1, u1 pair2)
{

	operand_stack *value1_low = pop_operand(f->p);
	operand_stack *value1_high = pop_operand(f->p);

	u4 high_bytes = value1_high->top->operand;
	u4 low_bytes = value1_low->top->operand;

	float flo = (float)decode_double_value(high_bytes, low_bytes);
	u4 value = (u4)(*(u4 *)&flo);
	f->p = push_operand(f->p, value, NULL, FLOAT_OP);
}

void i2b_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);

	i1 truncate = (i1)value1->top->operand;

	i4 result = (i4)truncate;

	f->p = push_operand(f->p, result, NULL, BYTE_OP);
}

void i2c_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);

	i1 truncate = (i1)value1->top->operand;

	i4 result = (i4)truncate;

	f->p = push_operand(f->p, result, NULL, CHAR_OP);
}

void i2s_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);

	i2 truncate = (i2)value1->top->operand;

	i4 result = (i4)truncate;

	f->p = push_operand(f->p, result, NULL, SHORT_OP);
}

void lcmp_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes1 = pop_operand(f->p);
	operand_stack *high_bytes1 = pop_operand(f->p);

	i8 long1 = ((i8)high_bytes1->top->operand << 32) | low_bytes1->top->operand;

	operand_stack *low_bytes2 = pop_operand(f->p);
	operand_stack *high_bytes2 = pop_operand(f->p);

	i8 long2 = ((i8)high_bytes2->top->operand << 32) | low_bytes2->top->operand;
	i4 ret;
	if (long2 > long1)
		ret = 1;
	else if (long2 < long1)
		ret = -1;
	else
		ret = 0;

	f->p = push_operand(f->p, ret, NULL, INTEGER_OP);
}

void fcmpl_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	float op1 = decode_float_value(value1->top->operand);
	float op2 = decode_float_value(value2->top->operand);
	i4 ret;
	if (op2 > op1)
		ret = 1;
	else if (op2 < op1)
		ret = -1;
	else
		ret = 0;

	f->p = push_operand(f->p, ret, NULL, INTEGER_OP);
}

void fcmpg_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	float op1 = decode_float_value(value1->top->operand);
	float op2 = decode_float_value(value2->top->operand);
	i4 ret;
	if (op2 > op1)
		ret = 1;
	else if (op2 < op1)
		ret = -1;
	else
		ret = 0;

	f->p = push_operand(f->p, ret, NULL, INTEGER_OP);
}

void dcmpl_impl(frame *f, u1 pair1, u1 pair2)
{

	operand_stack *value1_low = pop_operand(f->p);
	operand_stack *value1_high = pop_operand(f->p);
	operand_stack *value2_low = pop_operand(f->p);
	operand_stack *value2_high = pop_operand(f->p);

	double op1 = decode_double_value(value1_high->top->operand, value1_low->top->operand);
	double op2 = decode_double_value(value2_high->top->operand, value2_low->top->operand);
	i4 ret;
	if (op2 > op1)
		ret = 1;
	else if (op2 < op1)
		ret = -1;
	else
		ret = 0;

	f->p = push_operand(f->p, ret, NULL, INTEGER_OP);
}

void dcmpg_impl(frame *f, u1 pair1, u1 pair2)
{

	operand_stack *value1_low = pop_operand(f->p);
	operand_stack *value1_high = pop_operand(f->p);
	operand_stack *value2_low = pop_operand(f->p);
	operand_stack *value2_high = pop_operand(f->p);

	double op1 = decode_double_value(value1_high->top->operand, value1_low->top->operand);
	double op2 = decode_double_value(value2_high->top->operand, value2_low->top->operand);
	i4 ret;
	if (op2 > op1)
		ret = 1;
	else if (op2 < op1)
		ret = -1;
	else
		ret = 0;

	f->p = push_operand(f->p, ret, NULL, INTEGER_OP);
}

void ifeq_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{
	operand_stack *value = pop_operand(f->p);

	if (value->top->operand == 0)
	{
		int8_t v1 = (int8_t)branch_byte1;
		int8_t v2 = (int8_t)branch_byte2;
		int16_t branch_offset = (v1 << 8) | v2;
		jvm->pc += branch_offset;
	}
}

void ifne_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{

	operand_stack *value = pop_operand(f->p);

	if (value->top->operand != 0)
	{
		int8_t v1 = (int8_t)branch_byte1;
		int8_t v2 = (int8_t)branch_byte2;
		int16_t branch_offset = (v1 << 8) | v2;
		jvm->pc += branch_offset;
	}
}

void iflt_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{

	operand_stack *value = pop_operand(f->p);

	if (value->top->operand < 0)
	{
		int8_t v1 = (int8_t)branch_byte1;
		int8_t v2 = (int8_t)branch_byte2;
		int16_t branch_offset = (v1 << 8) | v2;
		jvm->pc += branch_offset;
	}
}

void ifge_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{
	operand_stack *value = pop_operand(f->p);

	if (value->top->operand >= 0)
	{
		uint8_t v1 = (int8_t)branch_byte1;
		int8_t v2 = (int8_t)branch_byte2;
		int16_t branch_offset = (v1 << 8) | v2;
		jvm->pc += branch_offset;
	}
}

void ifgt_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{
	operand_stack *value = pop_operand(f->p);

	if (value->top->operand > 0)
	{
		int8_t v1 = (int8_t)branch_byte1;
		int8_t v2 = (int8_t)branch_byte2;
		int16_t branch_offset = (v1 << 8) | v2;
		jvm->pc += branch_offset;
	}
}

void ifle_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{
	operand_stack *value = pop_operand(f->p);

	if (value->top->operand <= 0)
	{
		int8_t v1 = (int8_t)branch_byte1;
		int8_t v2 = (int8_t)branch_byte2;
		int16_t branch_offset = (v1 << 8) | v2;
		jvm->pc += branch_offset;
	}
}

void if_icmpeq_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	if (value1->top->operand == value2->top->operand)
	{
		int8_t v1 = (int8_t)branch_byte1;
		int8_t v2 = (int8_t)branch_byte2;
		int16_t branch_offset = (v1 << 8) | v2;
		jvm->pc += branch_offset;
	}
}

void if_icmpne_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	if (value1->top->operand != value2->top->operand)
	{
		int8_t v1 = (int8_t)branch_byte1;
		int8_t v2 = (int8_t)branch_byte2;
		int16_t branch_offset = (v1 << 8) | v2;
		jvm->pc += branch_offset;
	}
}

void if_icmplt_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	if (value2->top->operand < value1->top->operand)
	{
		int8_t v1 = (int8_t)branch_byte1;
		int8_t v2 = (int8_t)branch_byte2;
		int16_t branch_offset = (v1 << 8) | v2;
		jvm->pc += branch_offset;
	}
}

void if_icmpge_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	if (value2->top->operand >= value1->top->operand)
	{
		int8_t v1 = (int8_t)branch_byte1;
		int8_t v2 = (int8_t)branch_byte2;
		int16_t branch_offset = (v1 << 8) | v2;
		jvm->pc += branch_offset;
	}
}

void if_icmpgt_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	if (value2->top->operand > value1->top->operand)
	{
		int8_t v1 = (int8_t)branch_byte1;
		int8_t v2 = (int8_t)branch_byte2;
		int16_t branch_offset = (v1 << 8) | v2;
		jvm->pc += branch_offset;
	}
}

void if_icmple_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	if (value2->top->operand <= value1->top->operand)
	{
		int8_t v1 = (int8_t)branch_byte1;
		int8_t v2 = (int8_t)branch_byte2;
		int16_t branch_offset = (v1 << 8) | v2;
		jvm->pc += branch_offset;
	}
}

void acmpeq_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	if (value1->top->operand == value2->top->operand)
	{
		int8_t v1 = (int8_t)branch_byte1;
		int8_t v2 = (int8_t)branch_byte2;
		int16_t branch_offset = (v1 << 8) | v2;
		jvm->pc += branch_offset;
	}
}

void acmpne_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{
	operand_stack *value1 = pop_operand(f->p);
	operand_stack *value2 = pop_operand(f->p);

	if (value1->top->operand != value2->top->operand)
	{
		int8_t v1 = (int8_t)branch_byte1;
		int8_t v2 = (int8_t)branch_byte2;
		int16_t branch_offset = (v1 << 8) | v2;
		jvm->pc += branch_offset;
	}
}

void inst_goto_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{
	int8_t bb1 = (int8_t)branch_byte1;
	int8_t bb2 = (int8_t)branch_byte2;
	int16_t branch_offset = (bb1 << 8) | bb2;

	jvm->pc += branch_offset;
}

void jsr_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{
	int8_t v1 = (int8_t)branch_byte1;
	int8_t v2 = (int8_t)branch_byte2;
	int16_t branch_offset = (v1 << 8) | v2;
	jvm->pc += branch_offset;
}

void ret_impl(frame *f, u1 index, u1 pair)
{
}

void tableswitch_impl(frame *f, u1 pair1, u1 pair2)
{
	printf("Tableswitch -> Ainda nao implementado nessa versao!\npair1: %d\npair2: %d\nOp: %s\n", pair1, pair2, (char *)pop_operand(f->p));
}

void lookupswitch_impl(frame *f, u1 pair1, u1 pair2)
{
	printf("Lookupswitch -> Ainda nao implementado nessa versao!\npair1: %d\npair2: %d\nOp1: %s\n", pair1, pair2, (char *)pop_operand(f->p));
}

void ireturn_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *value = pop_operand(f->p);

	get_top_element(jvm->frames)->p = push_operand(get_top_element(jvm->frames)->p, (i4)value->top->operand, NULL, value->top->operand_type);
	pop_frame(jvm->frames);
}

void lreturn_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	get_top_element(jvm->frames)->p = push_operand(get_top_element(jvm->frames)->p, (u4)high_bytes->top->operand, NULL, LONG_OP);
	get_top_element(jvm->frames)->p = push_operand(get_top_element(jvm->frames)->p, (u4)low_bytes->top->operand, NULL, LONG_OP);
	pop_frame(jvm->frames);
}

void freturn_impl(frame *f, u1 pair1, u1 pair2)
{
	// Analisar as condições do método que deve ser retornado
	operand_stack *value = pop_operand(f->p);

	// Empilhar na pilha de operands do frame do chamador
	get_top_element(jvm->frames)->p = push_operand(get_top_element(jvm->frames)->p, (u4)value->top->operand, NULL, FLOAT_OP);
	pop_frame(jvm->frames);
}

void dreturn_impl(frame *f, u1 pair1, u1 pair2)
{
	// Analisar as condições do método que deve ser retornado
	operand_stack *low_bytes = pop_operand(f->p);
	operand_stack *high_bytes = pop_operand(f->p);

	// Empilhar na pilha de operands do frame do chamador
	get_top_element(jvm->frames)->p = push_operand(get_top_element(jvm->frames)->p, (u4)high_bytes->top->operand, NULL, DOUBLE_OP);
	get_top_element(jvm->frames)->p = push_operand(get_top_element(jvm->frames)->p, (u4)low_bytes->top->operand, NULL, DOUBLE_OP);
	pop_frame(jvm->frames);
}

void areturn_impl(frame *f, u1 pair1, u1 pair2)
{
	pop_operand(f->p);
}

void inst_return_impl(frame *f, u1 pair1, u1 pair2)
{
	pop_frame(jvm->frames);
}

void getstatic_impl(frame *f, u1 index_byte1, u1 index_byte2)
{
	u2 index_cp = (index_byte1 << 8) | index_byte2;

	cp_info *aux = f->cp - 1 + index_cp;

	// Descobrir a class do field
	char *field_class = decode_name_index_and_type(f->cp, aux->cp_union.CONSTANT_Fieldref_info.class_index, NAME_INDEX);
	if (strcmp(field_class, "java/lang/System") == 0)
	{
		f->p = push_operand(f->p, -INT_MAX, "out", REFERENCE_OP);
	}
	else
	{
		loaded_class *new = find_class_element(jvm->classes, field_class);
		if (new == NULL)
		{
			if (resolve_class(field_class) == NULL)
			{
				printf("Falha ao abrir class com field estático, encerrando.\n");
				exit(1);
			}
		}
		else
		{
			cp_info *name_type_field = f->cp - 1 + aux->cp_union.CONSTANT_Fieldref_info.name_and_type_index;

			char *name_field = decode_name_index_and_type(f->cp, name_type_field->cp_union.CONSTANT_NameAndType_info.name_index, NAME_AND_TYPE_INFO_NAME_INDEX);

			field_info *field_out = find_class_field(jvm->classes, field_class, name_field);
			if (field_out != NULL)
			{
				char *accessF = decode_access_flags(field_out->access_flags);

				if (static_flag_find(accessF))
				{
					char *field_descriptor_aux = decode_name_index_and_type(f->cp, name_type_field->cp_union.CONSTANT_NameAndType_info.descriptor_index, NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX);
					if (descriptor_field_validate(field_descriptor_aux) < 7)
					{
						if (descriptor_field_validate(field_descriptor_aux) == 1)
						{
							i4 valuePushed = *field_out->static_data->low;
							f->p = push_operand(f->p, valuePushed, NULL, FLOAT_OP);
						}
						else if (descriptor_field_validate(field_descriptor_aux) == 5)
						{
							u4 *valuePushed = field_out->static_data->low;
							f->p = push_operand(f->p, -INT_MAX, (void *)valuePushed, REFERENCE_STRING_OP);
						}
						else
						{
							i4 valuePushed = *field_out->static_data->low;
							f->p = push_operand(f->p, valuePushed, NULL, INTEGER_OP);
						}
					}
					else if (descriptor_field_validate(field_descriptor_aux) == 7 || descriptor_field_validate(field_descriptor_aux) == 8)
					{
						i4 valuePushedLow = *field_out->static_data->low;
						i4 valuePushedHigh = *field_out->static_data->high;
						if (descriptor_field_validate(field_descriptor_aux) == 7)
						{
							f->p = push_operand(f->p, valuePushedHigh, NULL, DOUBLE_OP);
							f->p = push_operand(f->p, valuePushedLow, NULL, DOUBLE_OP);
						}
						else
						{
							f->p = push_operand(f->p, valuePushedHigh, NULL, LONG_OP);
							f->p = push_operand(f->p, valuePushedLow, NULL, LONG_OP);
						}
					}
				}
			}
		}
	}
}

void putstatic_impl(frame *f, u1 index_byte1, u1 index_byte2)
{
	u2 index_cp = (index_byte1 << 8) | index_byte2;

	cp_info *field = f->cp - 1 + index_cp;

	// Resolver o field
	char *class_name = decode_name_index_and_type(f->cp, field->cp_union.CONSTANT_Fieldref_info.class_index, NAME_INDEX);
	loaded_class *new = find_class_element(jvm->classes, class_name);
	if (new == NULL)
	{
		if (resolve_class(class_name) == NULL)
		{
			printf("Falha ao abrir class com field estático, encerrando.\n");
			exit(1);
		}
	}
	else
	{
		cp_info *name_type_field = f->cp - 1 + field->cp_union.CONSTANT_Fieldref_info.name_and_type_index;

		char *name_field = decode_name_index_and_type(f->cp, name_type_field->cp_union.CONSTANT_NameAndType_info.name_index, NAME_AND_TYPE_INFO_NAME_INDEX);

		field_info *field_out = find_class_field(jvm->classes, class_name, name_field);
		if (field_out != NULL)
		{
			char *accessF = decode_access_flags(field_out->access_flags);

			if (static_flag_find(accessF))
			{

				char *field_descriptor_aux = decode_name_index_and_type(f->cp, name_type_field->cp_union.CONSTANT_NameAndType_info.descriptor_index, NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX);
				if (descriptor_field_validate(field_descriptor_aux) < 7)
				{
					operand_stack *value = pop_operand(f->p);
					field_out->static_data = (static_data *)malloc(sizeof(static_data));
					field_out->static_data->low = (u4 *)malloc(sizeof(u4));
					field_out->static_data->high = NULL;
					if (value->top->operand_type == REFERENCE_OP)
					{
						field_out->static_data->low = (u4 *)value->top->ref;
					}
					else
					{
						*field_out->static_data->low = (u4)value->top->operand;
					}
				}
				else if (descriptor_field_validate(field_descriptor_aux) == 7 || descriptor_field_validate(field_descriptor_aux) == 8)
				{
					operand_stack *value_low = pop_operand(f->p);
					operand_stack *value_high = pop_operand(f->p);
					field_out->static_data = (static_data *)malloc(sizeof(static_data));
					field_out->static_data->low = (u4 *)malloc(sizeof(u4));
					field_out->static_data->high = (u4 *)malloc(sizeof(u4));
					*field_out->static_data->low = (u4)value_low->top->operand;
					*field_out->static_data->high = (u4)value_high->top->operand;
				}
			}
		}
	}
}

bool static_flag_find(char *accessFlags)
{
	char *acc = (char *)malloc(100 * sizeof(char));
	strcpy(acc, accessFlags);
	const char s[2] = " ";
	char *token;
	token = strtok(acc, s);
	while (token != NULL)
	{
		if (strcmp(token, "STATIC") == 0)
		{
			return true;
		}
		token = strtok(NULL, s);
	}
	return false;
}

void getfield_impl(frame *f, u1 index_byte1, u1 index_byte2)
{
	u2 index_cp = (index_byte1 << 8) | index_byte2;
	cp_info *aux = f->cp - 1 + index_cp;
	char *field_class = decode_name_index_and_type(f->cp, aux->cp_union.CONSTANT_Fieldref_info.class_index, NAME_INDEX);
	if (strcmp(field_class, "java/lang/System") == 0)
	{
		f->p = push_operand(f->p, -INT_MAX, "out", REFERENCE_OP);
	}
	else
	{
		cp_info *name_type_field = f->cp - 1 + aux->cp_union.CONSTANT_Fieldref_info.name_and_type_index;
		char *name_field = decode_name_index_and_type(f->cp, name_type_field->cp_union.CONSTANT_NameAndType_info.name_index, NAME_AND_TYPE_INFO_NAME_INDEX);
		char *descriptor_ret = decode_name_index_and_type(f->cp, name_type_field->cp_union.CONSTANT_NameAndType_info.descriptor_index, NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX);
		int ret_type = get_return_type(descriptor_ret);
		if (ret_type != -1)
		{
			operand_stack *object_ref = pop_operand(f->p);
			ClassFile *searched = (ClassFile *)object_ref->top->ref;
			object_list *obj = search_object_by_reference(searched);
			if (obj != NULL)
			{
				int field_position = get_field_position(obj->obj, name_field);
				if (field_position != -1)
				{
					if (ret_type == 1)
					{
						u4 *value_low = (u4 *)malloc(sizeof(u4));
						*value_low = obj->data[field_position];
						int out_operand_type = get_out_operand_type(descriptor_ret);
						f->p = push_operand(f->p, *value_low, NULL, out_operand_type);
					}
					else
					{
						u4 *value_high = (u4 *)malloc(sizeof(u4));
						*value_high = obj->data[field_position];
						u4 *value_low = (u4 *)malloc(sizeof(u4));
						*value_low = obj->data[field_position + 1];
						int out_operand_type = get_out_operand_type(descriptor_ret);
						f->p = push_operand(f->p, *value_high, NULL, out_operand_type);
						f->p = push_operand(f->p, *value_low, NULL, out_operand_type);
					}
				}
				else
				{
					exit(1);
				}
			}
		}
	}
}

int get_out_operand_type(char *descriptor_ret)
{
	switch (*descriptor_ret)
	{
	case 'I':
		return INTEGER_OP;
	case 'F':
		return FLOAT_OP;
	case 'B':
		return BYTE_OP;
	case 'C':
		return CHAR_OP;
	case 'S':
		return SHORT_OP;
	case 'Z':
		return BOOLEAN_OP;
	case 'D':
		return DOUBLE_OP;
	case 'J':
		return LONG_OP;
	default:
		break;
	}
	return -1;
}

int get_return_type(char *descriptor)
{
	switch (*descriptor)
	{
	case 'I':
	case 'F':
	case 'B':
	case 'C':
	case 'S':
	case 'Z':
		return 1;
	case 'D':
	case 'J':
		return 2;
	default:
		break;
	}
	return -1;
}

void putfield_impl(frame *f, u1 index_byte1, u1 index_byte2)
{
	u2 index_cp = (index_byte1 << 8) | index_byte2;
	cp_info *field = f->cp - 1 + index_cp;
	char *class_name = decode_name_index_and_type(f->cp, field->cp_union.CONSTANT_Fieldref_info.class_index, NAME_INDEX);
	loaded_class *new = find_class_element(jvm->classes, class_name);
	if (new == NULL)
	{
		if (resolve_class(class_name) == NULL)
		{
			printf("Falha ao abrir class com field estático, encerrando.\n");
			exit(1);
		}
	}
	else
	{
		cp_info *name_type_field = f->cp - 1 + field->cp_union.CONSTANT_Fieldref_info.name_and_type_index;
		char *name_field = decode_name_index_and_type(f->cp, name_type_field->cp_union.CONSTANT_NameAndType_info.name_index, NAME_AND_TYPE_INFO_NAME_INDEX);
		operand_stack *value_poped_low = pop_operand(f->p);
		operand_stack *value_poped_high = NULL;
		if (value_poped_low->top->operand_type == DOUBLE_OP || value_poped_low->top->operand_type == LONG_OP)
		{
			value_poped_high = pop_operand(f->p);
		}
		operand_stack *object_ref = pop_operand(f->p);
		ClassFile *searched = (ClassFile *)object_ref->top->ref;
		object_list *obj = search_object_by_reference(searched);
		if (obj != NULL)
		{
			int field_position = get_field_position(obj->obj, name_field);
			if (field_position != -1)
			{
				if (value_poped_high == NULL)
				{
					obj->data[field_position] = value_poped_low->top->operand;
				}
				else
				{
					obj->data[field_position] = value_poped_high->top->operand;
					obj->data[field_position + 1] = value_poped_low->top->operand;
				}
			}
			else
			{
				exit(1);
			}
		}
	}
}

int get_field_position(ClassFile *obj, char *name_field)
{
	ClassFile *aux = obj;
	int pos = 0;
	for (field_info *auxF = aux->fields; auxF < aux->fields + aux->fields_count; auxF++)
	{
		char *nameF = decode_utf8_string(aux->constant_pool - 1 + auxF->name_index);
		if (strcmp(nameF, name_field) == 0)
			return pos;
		else
			pos++;
	}
	return -1;
}

object_list *search_object_by_reference(ClassFile *p)
{
	object_list *object_aux;
	for (object_aux = jvm->objects; object_aux != NULL; object_aux = object_aux->prox)
	{
		if (object_aux->obj == p)
			return object_aux;
	}
	return NULL;
}

void invokevirtual_impl(frame *f, u1 index_byte1, u1 index_byte2)
{
	u4 *end;
	u2 index_cp = (index_byte1 << 8) | index_byte2;
	char *method_class = NULL;
	int *parametros_cont = malloc(sizeof(int));

	char *name_method = get_name_method(f->cp, index_cp, 0);
	char *descriptor_method = get_descriptor_method(f->cp, index_cp, 0);
	char *copy_descriptor = malloc(strlen(descriptor_method) * sizeof(char));
	strcpy(copy_descriptor, descriptor_method);

	if (strcmp(name_method, "println") == 0)
	{
		double out_value_double;
		float out_value_float;
		long out_value_long;

		if (!check_empty_stack(f->p))
		{
			if (!print_empty(f->p))
			{
				operand_stack *string = pop_operand(f->p);
				operand_stack *v2;
				if (string->top->operand_type == DOUBLE_OP || string->top->operand_type == LONG_OP)
				{
					v2 = pop_operand(f->p);
				}

				pop_operand(f->p);

				switch (string->top->operand_type)
				{

				case BOOLEAN_OP:
					printf("%d\n", (i4)string->top->operand);
					break;
				case BYTE_OP:
					printf("%d\n", (i4)string->top->operand);
					break;
				case CHAR_OP:
					printf("%c\n", (char)string->top->operand);
					break;
				case SHORT_OP:
					printf("%d\n", (i4)string->top->operand);
					break;
				case INTEGER_OP:
					printf("%d\n", (i4)string->top->operand);
					break;
				case FLOAT_OP:
					out_value_float = decode_float_value(string->top->operand);
					printf("%.6f\n", out_value_float);
					break;
				case LONG_OP:
					out_value_long = decode_long_value(v2->top->operand, string->top->operand);
					printf("%ld\n", out_value_long);
					break;
				case DOUBLE_OP:
					out_value_double = decode_double_value(v2->top->operand, string->top->operand);
					printf("%.6lf\n", out_value_double);
					break;
				case RETURN_ADDRESS_OP:
					printf("[@%p\n", (u4 *)string->top->ref);
					break;
				case REFERENCE_ARRAY_BOOLEAN_OP:
					printf("[Z@%p\n", (u4 *)string->top->ref);
					break;
				case REFERENCE_ARRAY_CHAR_OP:
					printf("%s\n", (char *)string->top->ref);
					break;
				case REFERENCE_ARRAY_FLOAT_OP:
					printf("[F@%p\n", (u4 *)string->top->ref);
					break;
				case REFERENCE_ARRAY_DOUBLE_OP:
					printf("[D@%p\n", (u4 *)string->top->ref);
					break;
				case REFERENCE_ARRAY_BYTE_OP:
					printf("[B@%p\n", (u4 *)string->top->ref);
					break;
				case REFERENCE_ARRAY_SHORT_OP:
					printf("[S@%p\n", (u4 *)string->top->ref);
					break;
				case REFERENCE_ARRAY_INT_OP:
					printf("[I@%p\n", (u4 *)string->top->ref);
					break;
				case REFERENCE_ARRAY_LONG_OP:
					printf("[J@%p\n", (u4 *)string->top->ref);
					break;
				case REFERENCE_OP:
					printf("%s\n", (char *)string->top->ref);
					break;
				case REFERENCE_STRING_OP:
					end = (u4 *)string->top->ref;
					printf("%s\n", (char *)(end));
					break;
				}
			}
			else
			{
				printf("\n");
				pop_operand(f->p);
			}
		}
	}
	else if (strcmp(name_method, "print") == 0)
	{
		double out_value_double;
		float out_value_float;
		long out_value_long;

		if (!check_empty_stack(f->p))
		{
			if (!print_empty(f->p))
			{
				operand_stack *string = pop_operand(f->p);
				operand_stack *v2;
				if (string->top->operand_type == DOUBLE_OP || string->top->operand_type == LONG_OP)
				{
					v2 = pop_operand(f->p);
				}

				pop_operand(f->p);

				switch (string->top->operand_type)
				{

				case BOOLEAN_OP:
					printf("%d", (i4)string->top->operand);
					break;
				case BYTE_OP:
					printf("%d", (i4)string->top->operand);
					break;
				case CHAR_OP:
					printf("%c", (char)string->top->operand);
					break;
				case SHORT_OP:
					printf("%d", (i4)string->top->operand);
					break;
				case INTEGER_OP:
					printf("%d", (i4)string->top->operand);
					break;
				case FLOAT_OP:
					out_value_float = decode_float_value(string->top->operand);
					printf("%.6f", out_value_float);
					break;
				case LONG_OP:
					out_value_long = decode_long_value(v2->top->operand, string->top->operand);
					printf("%ld", out_value_long);
					break;
				case DOUBLE_OP:
					out_value_double = decode_double_value(v2->top->operand, string->top->operand);
					printf("%.6lf", out_value_double);
					break;
				case RETURN_ADDRESS_OP:
					printf("[@%p", (u4 *)string->top->ref);
					break;
				case REFERENCE_ARRAY_BOOLEAN_OP:
					printf("[Z@%p", (u4 *)string->top->ref);
					break;
				case REFERENCE_ARRAY_CHAR_OP:
					printf("%s", (char *)string->top->ref);
					break;
				case REFERENCE_ARRAY_FLOAT_OP:
					printf("[F@%p", (u4 *)string->top->ref);
					break;
				case REFERENCE_ARRAY_DOUBLE_OP:
					printf("[D@%p", (u4 *)string->top->ref);
					break;
				case REFERENCE_ARRAY_BYTE_OP:
					printf("[B@%p", (u4 *)string->top->ref);
					break;
				case REFERENCE_ARRAY_SHORT_OP:
					printf("[S@%p", (u4 *)string->top->ref);
					break;
				case REFERENCE_ARRAY_INT_OP:
					printf("[I@%p", (u4 *)string->top->ref);
					break;
				case REFERENCE_ARRAY_LONG_OP:
					printf("[J@%p", (u4 *)string->top->ref);
					break;
				case REFERENCE_OP:
					printf("%s", (char *)string->top->ref);
					break;
				case REFERENCE_STRING_OP:
					end = (u4 *)string->top->ref;
					printf("%s", (char *)(end));
					break;
				}
			}
			else
			{
				pop_operand(f->p);
			}
		}
	}
	else
	{
		method_class = get_class_method(f->cp, index_cp);

		method_info *method_aux = find_class_method(jvm->classes, method_class, name_method);
		attribute_info *aux;
		int pos;
		*parametros_cont = 1 + get_param_count(descriptor_method);

		for (pos = 0; pos < method_aux->attributes_count; pos++)
		{
			aux = (*(method_aux->attributes + pos));
			loaded_class *current_class = find_class_element(jvm->classes, method_class);
			char *nameindex = decode_utf8_string(current_class->class_file->constant_pool - 1 + aux->attribute_name_index);
			if (strcmp(nameindex, "Code") == 0)
			{
				code_attribute *c = (code_attribute *)aux->info;
				frame *f_new = create_frame(method_class, c->max_locals);
				f_new = transfer_array_stack(f, f_new, parametros_cont);

				push_frame(jvm->frames, f_new);

				loaded_class *class = find_class_element(jvm->classes, method_class);

				// Achar o método na class que o contém
				method_info *methods = class->class_file->methods;
				for (method_info *aux = methods; aux < methods + class->class_file->methods_count; aux++)
				{
					// Verificar se o name e o descriptor do método que deve ser invocado são iguais ao que está sendo analisado no .class
					char *name_method_aux = decode_utf8_string(class->class_file->constant_pool - 1 + aux->name_index);
					char *descriptor_method_aux = decode_utf8_string(class->class_file->constant_pool - 1 + aux->descriptor_index);

					if (strcmp(name_method, name_method_aux) == 0 && strcmp(copy_descriptor, descriptor_method_aux) == 0)
					{
						// Executar o code do método invocado
						jvm->pc += 3;
						f->return_address = jvm->pc;
						jvm->pc = 0;
						run_method(aux, method_class, 2);
						jvm->pc = f->return_address;
					}
				}
			}
		}
	}
}

void invokespecial_impl(frame *f, u1 index_byte1, u1 index_byte2)
{
	u2 index_cp = normalize_index(index_byte1, index_byte2);

	char *name_method = get_name_method(f->cp, index_cp, 0);
	char *descriptor_method = get_descriptor_method(f->cp, index_cp, 0);
	char *copy_descriptor = malloc(strlen(descriptor_method) * sizeof(char));
	strcpy(copy_descriptor, descriptor_method);
	char *method_class = NULL;
	char *classPaiDaCorrente = NULL;
	loaded_class *classCorrente = NULL;
	int *parametros_cont = malloc(sizeof(int));

	if (resolve_method(f->cp, index_cp, 0))
	{

		method_class = get_class_method(f->cp, index_cp);

		classCorrente = find_class_element(jvm->classes, get_top_element(jvm->frames)->current_class);

		classPaiDaCorrente = decode_name_index_and_type(classCorrente->class_file->constant_pool, classCorrente->class_file->super_class, NAME_INDEX);

		// If não executa, else executa
		if (is_super(classCorrente->class_file->access_flags) && strcmp(method_class, classPaiDaCorrente) == 0 && (strcmp(name_method, "init") != 0 || strcmp(name_method, "clinit") != 0))
		{
		}
		else
		{
			// Vai invocar o método
			method_info *method_aux = find_class_method(jvm->classes, method_class, name_method);
			attribute_info *aux;
			int pos;
			*parametros_cont = 1 + get_param_count(descriptor_method);

			for (pos = 0; pos < method_aux->attributes_count; pos++)
			{
				aux = (*(method_aux->attributes + pos));
				loaded_class *current_class = find_class_element(jvm->classes, method_class);
				char *nameindex = decode_utf8_string(current_class->class_file->constant_pool - 1 + aux->attribute_name_index);
				if (strcmp(nameindex, "Code") == 0)
				{
					code_attribute *c = (code_attribute *)aux->info;
					frame *f_new = create_frame(method_class, c->max_locals);
					f_new = transfer_array_stack(f, f_new, parametros_cont);
					push_frame(jvm->frames, f_new);

					loaded_class *class = find_class_element(jvm->classes, method_class);

					// Achar o método na class que o contém
					method_info *methods = class->class_file->methods;
					for (method_info *aux = methods; aux < methods + class->class_file->methods_count; aux++)
					{
						// Verificar se o name e o descriptor do método que deve ser invocado são iguais ao que está sendo analisado no .class
						char *name_method_aux = decode_utf8_string(class->class_file->constant_pool - 1 + aux->name_index);
						char *descriptor_method_aux = decode_utf8_string(class->class_file->constant_pool - 1 + aux->descriptor_index);

						if (strcmp(name_method, name_method_aux) == 0 && strcmp(copy_descriptor, descriptor_method_aux) == 0)
						{
							// Executar o code do método invocado
							jvm->pc += 3;
							f->return_address = jvm->pc;
							jvm->pc = 0;
							run_method(aux, method_class, 2);
							jvm->pc = f->return_address;
						}
					}
				}
			}
		}
	}
}

int get_param_count(char *descriptor)
{
	int char_size = strlen(descriptor);
	int count = 0;
	char *aux_descriptor = descriptor;
	while (char_size > 0)
	{
		switch (*aux_descriptor)
		{
		case '(':
			break;
		case ')':
			return count;
			break;
		case 'I':
		case 'F':
		case 'B':
		case 'C':
		case 'S':
		case 'Z':
			count++;
			break;
		case 'D':
		case 'J':
			count += 2;
			break;
		default:
			break;
		}
		aux_descriptor++;
		char_size--;
	}
	return count;
}

void invokestatic_impl(frame *f, u1 index_byte1, u1 index_byte2)
{

	u2 index_cp = (index_byte1 << 8) | index_byte2;
	char *name_method = get_name_method(f->cp, index_cp, 0);
	char *descriptor_method = get_descriptor_method(f->cp, index_cp, 0);
	char *copy_descriptor = malloc(strlen(descriptor_method) * sizeof(char));
	strcpy(copy_descriptor, descriptor_method);
	int *parametros_cont = malloc(sizeof(int));
	*parametros_cont = 0;

	// Realizar a contagem de pairâmetros

	char *pch = strtok(descriptor_method, "()");
	*parametros_cont += strlen(pch);

	if (resolve_method(f->cp, index_cp, 0))
	{
		char *classnew = get_class_method(f->cp, index_cp);
		method_info *method_aux = find_class_method(jvm->classes, classnew, name_method);
		attribute_info *aux;
		int pos;
		for (pos = 0; pos < method_aux->attributes_count; pos++)
		{
			aux = (*(method_aux->attributes + pos));
			loaded_class *current_class = find_class_element(jvm->classes, classnew);
			char *nameindex = decode_utf8_string(current_class->class_file->constant_pool - 1 + aux->attribute_name_index);
			if (strcmp(nameindex, "Code") == 0)
			{
				code_attribute *c = (code_attribute *)aux->info;
				frame *f_new = create_frame(classnew, c->max_locals);

				f_new = transfer_array_stack(f, f_new, parametros_cont);
				push_frame(jvm->frames, f_new);

				loaded_class *class = find_class_element(jvm->classes, classnew);

				// Achar o método na class que o contém
				method_info *methods = class->class_file->methods;
				for (method_info *aux = methods; aux < methods + class->class_file->methods_count; aux++)
				{
					// Verificar se o name e o descriptor do método que deve ser invocado são iguais ao que está sendo analisado no .class
					char *name_method_aux = decode_utf8_string(class->class_file->constant_pool - 1 + aux->name_index);
					char *descriptor_method_aux = decode_utf8_string(class->class_file->constant_pool - 1 + aux->descriptor_index);

					if (strcmp(name_method, name_method_aux) == 0 && strcmp(copy_descriptor, descriptor_method_aux) == 0)
					{
						// Executar o code do método invocado
						jvm->pc += 3;
						f->return_address = jvm->pc;
						jvm->pc = 0;
						run_method(aux, classnew, 2);
						jvm->pc = f->return_address;
					}
				}
			}
		}
	}
}

void invokeinterface_impl(frame *f, u1 index_byte1, u1 index_byte2, u1 count)
{
	u2 index_cp = normalize_index(index_byte1, index_byte2);

	char *name_method = get_name_method(f->cp, index_cp, 1);
	char *descriptor_method = get_descriptor_method(f->cp, index_cp, 1);

	if (resolve_method(f->cp, index_cp, 1))
	{
		char *classnew = malloc(100 * sizeof(char));
		strcpy(classnew, get_top_element(jvm->frames)->current_class);
		method_info *method_aux = find_class_method(jvm->classes, classnew, name_method);
		attribute_info *aux;
		int pos;
		for (pos = 0; pos < method_aux->attributes_count; pos++)
		{
			aux = (*(method_aux->attributes + pos));
			loaded_class *current_class = find_class_element(jvm->classes, classnew);
			char *nameindex = decode_utf8_string(current_class->class_file->constant_pool - 1 + aux->attribute_name_index);
			if (strcmp(nameindex, "Code") == 0)
			{
				code_attribute *c = (code_attribute *)aux->info;
				frame *f_new = create_frame(classnew, c->max_locals);
				f_new = transfer_array_stack_count(f, f_new, count);
				push_frame(jvm->frames, f_new);

				loaded_class *class = find_class_element(jvm->classes, classnew);

				// Achar o método na class que o contém
				method_info *methods = class->class_file->methods;
				for (method_info *aux = methods; aux < methods + class->class_file->methods_count; aux++)
				{
					// Verificar se o name e o descriptor do método que deve ser invocado são iguais ao que está sendo analisado no .class
					char *name_method_aux = decode_utf8_string(class->class_file->constant_pool - 1 + aux->name_index);
					char *descriptor_method_aux = decode_utf8_string(class->class_file->constant_pool - 1 + aux->descriptor_index);

					if (strcmp(name_method, name_method_aux) == 0 && strcmp(descriptor_method, descriptor_method_aux) == 0)
					{
						run_method(aux, classnew, 2);
					}
				}
			}
		}
	}
}

void invokeinterface_ghost(frame *pair0, u1 pair1, u1 pair2)
{
}

void invokedynamic_ghost(frame *pair0, u1 pair1, u1 pair2)
{
}

void inst_new_impl(frame *f, u1 index_byte1, u1 index_byte2)
{
	u2 index_cp = (index_byte1 << 8) | index_byte2;
	char *class_name = decode_name_index_and_type(f->cp, index_cp, NAME_INDEX);
	ClassFile *class = instantiate_class(class_name);
	int numFields = get_not_static_params(class);
	jvm->objects = insert_object(jvm->objects, class, numFields);
	f->p = push_operand(f->p, -INT_MAX, class, REFERENCE_OP);
}

ClassFile *instantiate_class(char *class_name)
{
	char *out = (char *)malloc((strlen(class_name) + 8) * sizeof(char));
	strcpy(out, class_name);
	strcat(out, ".class");
	ClassFile *cf = read_file(out);
	return cf;
}

object_list *insert_object(object_list *lis, ClassFile *class, int not_static_param)
{
	object_list *object = (object_list *)malloc(sizeof(object_list));
	object->obj = class;
	object->size_data = not_static_param;
	object->data = (u4 *)malloc(not_static_param * sizeof(u4));
	object->prox = NULL;
	object->ant = NULL;

	object_list *aux;
	object_list *last;
	if (lis != NULL)
	{
		aux = lis;
		while (aux != NULL)
		{
			last = aux;
			aux = aux->prox;
		}
		last->prox = object;
		object->ant = last;
		return lis;
	}
	else
	{
		return object;
	}
}

int get_not_static_params(ClassFile *class)
{
	ClassFile *class_aux = class;
	int count = 0;
	if (class_aux->fields_count > 0)
	{
		for (field_info *field_aux = class_aux->fields; field_aux < class_aux->fields + class_aux->fields_count; field_aux++)
		{
			if (field_aux->access_flags != 0x0008)
			{
				count++;
			}
		}
	}
	return count;
}

void newarray_impl(frame *f, u1 atype, u1 pair1)
{
	operand_stack *count = pop_operand(f->p);
	i4 countnum = count->top->operand;
	int ref_size = 0;
	void *address = NULL;
	int i = 0;
	if (count < 0)
	{
		// Lançar exceção
	}
	else
	{
		switch (atype)
		{
		case T_BOOLEAN:
			address = (u1 *)malloc((countnum + 1) * sizeof(u1));
			ref_size = sizeof(u1);
			/*// Inicializar com os valuees default */
			for (void *p = address; i <= countnum; i++, p += ref_size)
			{
				// Alocar com -INT_MAX paira marcar o fim do array
				if (i == countnum)
				{
					*(u1 *)p = CHAR_MAX;
				}
				else
				{
					*(u1 *)p = 0;
				}
			}
			break;

		case T_CHAR:
			address = (i2 *)malloc((countnum + 1) * sizeof(i2));
			ref_size = sizeof(i2);
			/*// Inicializar com os valuees default */
			for (void *p = address; i <= countnum; i++, p += ref_size)
			{
				// Alocar com -INT_MAX paira marcar o fim do array
				if (i == countnum)
				{
					*(i2 *)p = SHRT_MAX;
				}
				else
				{
					*(i2 *)p = 0;
				}
			}
			break;

		case T_FLOAT:
			address = (u4 *)malloc((countnum + 1) * sizeof(u4));
			ref_size = sizeof(u4);
			/*// Inicializar com os valuees default */
			for (void *p = address; i <= countnum; i++, p += ref_size)
			{
				// Alocar com -INT_MAX paira marcar o fim do array
				if (i == countnum)
				{
					*(u4 *)p = UINT32_MAX;
				}
				else
				{
					*(u4 *)p = 0;
				}
			}
			break;

		case T_DOUBLE:
			address = (u4 *)malloc((2 * (countnum) + 1) * sizeof(u4));
			ref_size = 2 * sizeof(u4);
			/*// Inicializar com os valuees default */
			for (void *p = address; i <= countnum; i++, p += ref_size)
			{
				// Alocar com -INT_MAX paira marcar o fim do array
				if (i == countnum)
				{
					*(u4 *)p = UINT32_MAX;
				}
				else
				{
					*(u4 *)p = 0;
				}
			}
			break;

		case T_BYTE:
			address = (i1 *)malloc((countnum + 1) * sizeof(i1));
			ref_size = sizeof(i1);
			/* Inicializar com os valuees default */
			for (void *p = address; i <= countnum; i++, p += ref_size)
			{
				// Alocar com -INT_MAX paira marcar o fim do array
				if (i == countnum)
				{
					*(i1 *)p = CHAR_MAX;
				}
				else
				{
					*(i1 *)p = 0;
				}
			}
			break;

		case T_SHORT:
			address = (i2 *)malloc((countnum + 1) * sizeof(i2));
			ref_size = sizeof(i2);
			/* Inicializar com os valuees default */
			for (void *p = address; i <= countnum; i++, p += ref_size)
			{
				// Alocar com -INT_MAX paira marcar o fim do array
				if (i == countnum)
				{
					*(i2 *)p = SHRT_MAX;
				}
				else
				{
					*(i2 *)p = 0;
				}
			}
			break;

		case T_INT:
			address = (i4 *)malloc((countnum + 1) * sizeof(i4));
			ref_size = sizeof(i4);
			/*// Inicializar com os valuees default */
			for (void *p = address; i <= countnum; i++, p += ref_size)
			{
				// Alocar com -INT_MAX paira marcar o fim do array
				if (i == countnum)
				{
					*(i4 *)p = INT_MAX;
				}
				else
				{
					*(i4 *)p = 0;
				}
			}
			break;

		case T_LONG:
			address = (i4 *)malloc((2 * (countnum) + 1) * sizeof(i4));
			ref_size = 2 * sizeof(i4);
			/*// Inicializar com os valuees default */
			for (void *p = address; i <= countnum; i++, p += ref_size)
			{
				// Alocar com -INT_MAX paira marcar o fim do array
				if (i == countnum)
				{
					*(i4 *)p = INT32_MAX;
				}
				else
				{
					*(i4 *)p = 0;
				}
			}
			break;
		}

		// atype + 8 = Transformando tipo de array pra tipo de reference (ver estrutura)
		f->p = push_operand(f->p, -INT_MAX, address, atype + 7);
	}
}

void anewarray_impl(frame *f, u1 pair1, u1 pair2)
{
}

void arraylength_impl(frame *f, u1 pair1, u1 pair2)
{
	operand_stack *array_ref = pop_operand(f->p);
	void *reference = array_ref->top->ref;
	u1 reference_type = array_ref->top->operand_type;

	int size = 0;
	int ref_size = 0;
	switch (reference_type)
	{
	case REFERENCE_ARRAY_BOOLEAN_OP:
		ref_size = sizeof(u1);
		for (void *p = reference; p += ref_size; size++)
		{
			if (*(u1 *)p == CHAR_MAX)
			{
				break;
			}
		}
		break;
	case REFERENCE_ARRAY_CHAR_OP:
		ref_size = sizeof(i2);
		for (void *p = reference; p += ref_size; size++)
		{
			if (*(i2 *)p == SHRT_MAX)
			{
				break;
			}
		}
		break;
	case REFERENCE_ARRAY_FLOAT_OP:
		ref_size = sizeof(u4);
		for (void *p = reference; p += ref_size; size++)
		{
			if (*(u4 *)p == UINT32_MAX)
			{
				break;
			}
		}
		break;
	case REFERENCE_ARRAY_DOUBLE_OP:
		ref_size = 2 * sizeof(u4);
		for (void *p = reference; p += ref_size; size++)
		{
			if (*(u4 *)p == UINT32_MAX)
			{
				break;
			}
		}
		break;
	case REFERENCE_ARRAY_BYTE_OP:
		ref_size = sizeof(i1);
		for (void *p = reference; p += ref_size; size++)
		{
			if (*(i1 *)p == CHAR_MAX)
			{
				break;
			}
		}
		break;
	case REFERENCE_ARRAY_SHORT_OP:
		ref_size = sizeof(i2);
		for (void *p = reference; p += ref_size; size++)
		{
			if (*(i2 *)p == SHRT_MAX)
			{
				break;
			}
		}
		break;
	case REFERENCE_ARRAY_INT_OP:
		ref_size = sizeof(i4);
		for (void *p = reference; p += ref_size; size++)
		{
			if (*(i4 *)p == INT_MAX)
			{
				break;
			}
		}
		break;
	case REFERENCE_ARRAY_LONG_OP:
		ref_size = 2 * sizeof(i4);
		for (void *p = reference; p += ref_size; size++)
		{
			if (*(i4 *)p == INT32_MAX)
			{
				break;
			}
		}
		break;
	}

	f->p = push_operand(f->p, size, NULL, INTEGER_OP);
}

void athrow_impl(frame *f, u1 pair1, u1 pair2)
{
}

void checkcast_impl(frame *f, u1 pair1, u1 pair2)
{
}

void instanceof_impl(frame *f, u1 pair1, u1 pair2)
{
}

void monitorenter_impl(frame *f, u1 pair1, u1 pair2)
{
}

void monitorexit_impl(frame *f, u1 pair1, u1 pair2)
{
}

void wide_impl(frame *f, u1 index_byte1, u1 index_byte2)
{
	return;
}

void multianewarray_ghost(frame *f, u1 pair1, u1 pair2)
{
}
void *initialize_recursive_multiarray(void *address, i4 *counts, char *tipos)
{
	switch (tipos[1])
	{
	case '[':
	{
		address = (void **)malloc(counts[0] * sizeof(void *));
		void **address_aux = address;
		for (u4 i = 0; i < counts[0]; i++)
		{
			*address_aux = initialize_recursive_multiarray(address_aux, &counts[1], &tipos[1]);
			address_aux++;
		}
		break;
	}
	case 'I':
	{
		address = (i4 *)malloc(counts[0] * sizeof(i4));
		i4 *address_aux = address;
		for (u4 i = 0; i < counts[0]; i++)
		{
			*address_aux = 0;
			address_aux++;
		}
		break;
	}
	case 'B':
	{
		address = (i1 *)malloc(counts[0] * sizeof(i1));
		i1 *address_aux = address;
		for (u4 i = 0; i < counts[0]; i++)
		{
			*address_aux = 0;
			address_aux++;
		}
		break;
	}
	case 'S':
	{
		address = (i2 *)malloc(counts[0] * sizeof(i2));
		i2 *address_aux = address;
		for (u4 i = 0; i < counts[0]; i++)
		{
			*address_aux = 0;
			address_aux++;
		}
		break;
	}
	case 'J':
	{ //LONG
		address = (i8 *)malloc(counts[0] * sizeof(i8));
		i8 *address_aux = address;
		for (u4 i = 0; i < counts[0]; i++)
		{
			*address_aux = 0;
			address_aux++;
		}
		break;
	}
	case 'F':
	{
		address = (float *)malloc(counts[0] * sizeof(float));
		float *address_aux = address;
		for (u4 i = 0; i < counts[0]; i++)
		{
			*address_aux = 0;
			address_aux++;
		}
		break;
	}
	case 'D':
	{
		address = (double *)malloc(counts[0] * sizeof(double));
		double *address_aux = address;
		for (u4 i = 0; i < counts[0]; i++)
		{
			*address_aux = 0;
			address_aux++;
		}
		break;
	}
	default:
		break;
	}
	return address;
}
void multianewarray_impl(frame *f, u1 index_byte1, u1 index_byte2, u1 dimensions)
{

	u2 index_cp = normalize_index(index_byte1, index_byte2);
	cp_info *item = f->cp - 1 + index_cp;
	char *tipos = decode_utf8_string(f->cp - 1 + item->cp_union.CONSTANT_Class_info.name_index);
	i4 *counts = (i4 *)malloc(dimensions * sizeof(i4));
	void *address = NULL;
	for (u1 i = dimensions; i > 0; i--)
	{
		operand_stack *value = pop_operand(f->p);
		counts[i - 1] = value->top->operand;
		if (counts[i] < 0)
		{
			//Lancar NegativeArraySizeException
		}
	}

	address = initialize_recursive_multiarray(address, counts, tipos);

	f->p = push_operand(f->p, -INT_MAX, address, REFERENCE_OP);
	free(counts);
}

void ifnull_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{
	operand_stack *value = pop_operand(f->p);

	i2 offset = 0;

	if (value->top->ref == NULL)
	{
		i1 v1 = (i1)branch_byte1;
		i1 v2 = (i1)branch_byte2;
		offset = (v1 << 8) | v2;
		jvm->pc += offset;
	}
}

void ifnonnull_impl(frame *f, u1 branch_byte1, u1 branch_byte2)
{
	operand_stack *value = pop_operand(f->p);

	i2 offset = 0;

	if (value->top->ref != NULL)
	{
		i1 v1 = (i1)branch_byte1;
		i1 v2 = (i1)branch_byte2;
		offset = (v1 << 8) | v2;
		jvm->pc += offset;
	}
}

void goto_w_impl(frame *f, u1 pair1, u1 pair2)
{
}

void jsr_w_impl(frame *f, u1 pair1, u1 pair2)
{
}

double decode_double_value(u4 high, u4 low)
{
	u8 value = ((u8)(high) << 32) | (u8)low;

	double ret = (double)(*(double *)&value);
	return ret;
}

long decode_long_value(u4 high, u4 low)
{
	u8 value = ((u8)(high) << 32) | (u8)low;
	long ret = (long)(*(long *)&value);
	return ret;
}

float decode_float_value(u4 value)
{
	float ret = (float)(*(float *)&value);
	return ret;
}
