#include "exibidor.h"
#include <string.h>

void print_name_tag(u1 tag)
{
	char *name;

	switch (tag)
	{
	case CONSTANT_Class:
		name = "CONSTANT_Class_Info";
		break;
	case CONSTANT_Fieldref:
		name = "CONSTANT_Field_Info";
		break;
	case CONSTANT_Methodref:
		name = "CONSTANT_Method_Info";
		break;
	case CONSTANT_InterfaceMethodref:
		name = "CONSTANT_InterfaceMethod_Info";
		break;
	case CONSTANT_String:
		name = "CONSTANT_String_Info";
		break;
	case CONSTANT_Integer:
		name = "CONSTANT_Integer_Info";
		break;
	case CONSTANT_Float:
		name = "CONSTANT_Float_Info";
		break;
	case CONSTANT_Long:
		name = "CONSTANT_Long_Info";
		break;
	case CONSTANT_Double:
		name = "CONSTANT_Double_Info";
		break;
	case CONSTANT_NameAndType:
		name = "CONSTANT_NameAndType_Info";
		break;
	case CONSTANT_Utf8:
		name = "CONSTANT_Utf8_Info";
		break;
	case CONSTANT_MethodHandle:
		name = "CONSTANT_MethodHandle_Info";
		break;
	case CONSTANT_MethodType:
		name = "CONSTANT_MethodType_Info";
		break;
	case CONSTANT_InvokeDynamic:
		name = "CONSTANT_InvokeDynamic_Info";
		break;
	default:
		printf("TAG: Undefined\n");
		return;
	}

	printf("TAG: %s\n", name);
}

char *decode_utf8_string(cp_info *cp)
{
	char *decoded = malloc((cp->cp_union.CONSTANT_Utf8_info.length + 1) * sizeof(char));
	char *decodedAux = decoded;

	for (u1 *aux = cp->cp_union.CONSTANT_Utf8_info.bytes; aux < cp->cp_union.CONSTANT_Utf8_info.bytes + cp->cp_union.CONSTANT_Utf8_info.length; aux++)
	{
		*decodedAux = (char)*aux;
		decodedAux++;
	}

	*decodedAux = '\0';

	return decoded;
}

char *decode_name_index_and_type(cp_info *cp, u2 index, u1 type)
{
	char *decoded = malloc(100 * sizeof(char));

	cp_info *aux;
	cp_info *aux2;
	cp_info *aux3;

	aux = cp + index - 1;

	switch (type)
	{
	case NAME_INDEX:
		aux2 = cp + (aux->cp_union.CONSTANT_Class_info.name_index - 1);

		decoded = decode_utf8_string(aux2);
		break;

	case NAME_AND_TYPE:
		aux2 = cp + (aux->cp_union.CONSTANT_NameAndType_info.name_index - 1);
		aux3 = cp + (aux->cp_union.CONSTANT_NameAndType_info.descriptor_index - 1);

		decoded = decode_utf8_string(aux2);
		strcat(decoded, ": ");
		strcat(decoded, decode_utf8_string(aux3));
		break;

	case STRING_INDEX:
	case CLASS_INDEX:
	case NAME_AND_TYPE_INFO_NAME_INDEX:
	case NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX:
		decoded = decode_utf8_string(aux);
		break;

	default:
		decoded = "";
	}

	return decoded;
}

char *decode_access_flags(u2 flag)
{
	char *decoded = malloc(100 * sizeof(char));
	strcpy(decoded, "|");

	if (flag & ACC_PUBLIC)
		strcat(decoded, " PUBLIC |");

	if (flag & ACC_FINAL)
		strcat(decoded, " FINAL |");

	if (flag & ACC_SUPER)
		strcat(decoded, " SUPER |");

	if (flag & ACC_INTERFACE)
		strcat(decoded, " INTERFACE |");

	if (flag & ACC_ABSTRACT)
		strcat(decoded, " ABSTRACT |");

	if (flag & ACC_SYNTHETIC)
		strcat(decoded, " SYNTHETIC |");

	if (flag & ACC_ANNOTATION)
		strcat(decoded, " ANNOTATION |");

	if (flag & ACC_ENUM)
		strcat(decoded, " ENUM |");

	return (decoded);
}

char *decode_field_access_flags(u2 flag)
{
	char *decoded = malloc(100 * sizeof(char));
	strcpy(decoded, "|");

	if (flag & FIELD_ACC_PUBLIC)
		strcat(decoded, "PUBLIC | ");

	if (flag & FIELD_ACC_PROTECTED)
		strcat(decoded, "PROTECTED | ");

	if (flag & FIELD_ACC_STATIC)
		strcat(decoded, "STATIC | ");

	if (flag & FIELD_ACC_FINAL)
		strcat(decoded, "FINAL | ");

	if (flag & FIELD_ACC_VOLATILE)
		strcat(decoded, "VOLATILE | ");

	if (flag & FIELD_ACC_TRANSIENT)
		strcat(decoded, "TRANSIENT | ");

	if (flag & FIELD_ACC_SYNTHETIC)
		strcat(decoded, "SYNTHETIC | ");

	return (decoded);
}

double decode_double_info(cp_info *cp)
{
	int64_t value = ((int64_t)cp->cp_union.CONSTANT_Double_info.high_bytes << 32) + cp->cp_union.CONSTANT_Double_info.low_bytes;

	int16_t sign = ((value >> 63) == 0) ? 1 : -1;
	int16_t exp = ((value >> 52) & 0x7ffL);
	int64_t mant = (exp == 0) ? ((value & 0xfffffffffffffL) << 1) : ((value & 0xfffffffffffffL) | 0x10000000000000L);

	double doubleValue = sign * mant * pow(2, exp - 1075);
	return doubleValue;
}

int64_t decode_long_info(cp_info *cp)
{
	int64_t longValue = (((int64_t)cp->cp_union.CONSTANT_Long_info.high_bytes) << 32) | ((int64_t)cp->cp_union.CONSTANT_Long_info.low_bytes);
	return longValue;
}

float decode_float_info(cp_info *cp)
{
	float floatValue;

	int value = cp->cp_union.CONSTANT_Float_info.bytes;
	int sign = ((value >> 31) == 0) ? 1 : -1;
	int exp = ((value >> 23) & 0xff);
	int mant = (exp == 0) ? (value & 0x7fffff) << 1 : (value & 0x7fffff) | 0x800000;

	floatValue = sign * mant * pow(2, exp - 150);

	return floatValue;
}

void print_class_file(ClassFile *cf)
{
	cp_info *cp_aux;
	double valor;
	uint64_t longValue;
	method_info *auxMethod;
	field_info *auxField;
	attribute_info **auxAttributeClasse;
	attribute_info **fieldAttrAux;
	// exception_table *exceptionTableAux;
	uint32_t contador = 1;
	char *ponteiroprint;

	printf("\n-----| INFO |-----\n\n");
	printf("Magic: 0x%08x\n", cf->magic);

	if ((cf->major_version - 44) < 5)
		printf("Java Version: 1.%d", (cf->major_version - 44));
	else
		printf("Java Version: %d", (cf->major_version - 44));

	printf(".%d\n", cf->minor_version);

	printf("Constant Pool Count: %d\n", cf->constant_pool_count);
	ponteiroprint = decode_access_flags(cf->access_flags);
	printf("Access Flags: %s\n", ponteiroprint);

	ponteiroprint = decode_name_index_and_type(cf->constant_pool, cf->this_class, NAME_INDEX);
	printf("This Class: %s\n", ponteiroprint);

	ponteiroprint = decode_name_index_and_type(cf->constant_pool, cf->super_class, NAME_INDEX);
	printf("Super Class: %s\n", ponteiroprint);
	
	printf("Interfaces Count: %d\n", cf->interfaces_count);
	printf("Fields Count: %d\n", cf->fields_count);
	printf("Methods Count: %d\n", cf->methods_count);
	printf("Atributes Count: %d\n", cf->attributes_count);

	printf("\n\n-----| CONSTANT POOL |-----\n\n");
	for (cp_aux = cf->constant_pool; cp_aux < cf->constant_pool + cf->constant_pool_count - 1; cp_aux++)
	{
		printf("\n--------------| Entrada [%d] |--------------\n\n", contador);
		print_name_tag(cp_aux->tag);
		contador++;

		switch (cp_aux->tag)
		{
		case CONSTANT_Class:
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_Class_info.name_index, CLASS_INDEX);
			printf("Class Name: %s\n", ponteiroprint);
			break;
		case CONSTANT_Fieldref:
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_Fieldref_info.class_index, NAME_INDEX);
			printf("Class Name: %s\n", ponteiroprint);
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_Fieldref_info.name_and_type_index, NAME_AND_TYPE);
			printf("Name and Type: %s\n", ponteiroprint);
			break;
		case CONSTANT_Methodref:
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_Methodref_info.class_index, NAME_INDEX);
			printf("Class Name: %s\n", ponteiroprint);
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_Methodref_info.name_and_type_index, NAME_AND_TYPE);
			printf("Name and Type: %s\n", ponteiroprint);
			break;
		case CONSTANT_InterfaceMethodref:
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_String_info.string_index, NAME_INDEX);
			printf("Class Name: %s\n", ponteiroprint);
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_String_info.string_index, NAME_AND_TYPE);
			printf("Name and Type Index: %s\n", ponteiroprint);
			break;
		case CONSTANT_String:
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_String_info.string_index, STRING_INDEX);
			printf("String: %s\n", ponteiroprint);
			break;
		case CONSTANT_Integer:
			printf("Bytes: %04x\n", cp_aux->cp_union.CONSTANT_Integer_info.bytes);
			printf("Integer: %d\n", cp_aux->cp_union.CONSTANT_Integer_info.bytes);
			break;
		case CONSTANT_Float:
			printf("Bytes: %04x\n", cp_aux->cp_union.CONSTANT_Float_info.bytes);
			printf("Float: %d\n", cp_aux->cp_union.CONSTANT_Float_info.bytes);
			break;
		case CONSTANT_Long:
			longValue = decode_long_info(cp_aux);
			printf("Long High Bytes: 0x%08x\n", cp_aux->cp_union.CONSTANT_Long_info.high_bytes);
			printf("Long Low Bytes: 0x%08x\n", cp_aux->cp_union.CONSTANT_Long_info.low_bytes);
			printf("Long: %I64u\n", longValue);
			break;
		case CONSTANT_Double:
			valor = decode_double_info(cp_aux);
			printf("Double High Bytes: 0x%08x\n", cp_aux->cp_union.CONSTANT_Double_info.high_bytes);
			printf("Double Low Bytes: 0x%08x\n", cp_aux->cp_union.CONSTANT_Double_info.low_bytes);
			printf("Double: %.2lf\n", valor);
			break;
		case CONSTANT_NameAndType:
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_NameAndType_info.name_index, NAME_AND_TYPE_INFO_NAME_INDEX);
			printf("Name: %s\n", ponteiroprint);
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_NameAndType_info.descriptor_index, NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX);
			printf("Descriptor: %s\n", ponteiroprint);
			break;
		case CONSTANT_Utf8:
			printf("Length of byte array: %d\n", (int)cp_aux->cp_union.CONSTANT_Utf8_info.length);
			printf("Length of string: %d\n", (int)cp_aux->cp_union.CONSTANT_Utf8_info.length);
			printf("String: ");
			for (u1 *i = cp_aux->cp_union.CONSTANT_Utf8_info.bytes; i < cp_aux->cp_union.CONSTANT_Utf8_info.bytes + cp_aux->cp_union.CONSTANT_Utf8_info.length; i++)
			{
				printf("%c", (char)(*i));
			}
			printf("\n");
			break;
		case CONSTANT_MethodHandle:
			printf("MethodHandle Reference Kind: %02x\n", cp_aux->cp_union.CONSTANT_MethodHandle_info.reference_kind);
			printf("MethodHandle Reference Index: %04x\n", cp_aux->cp_union.CONSTANT_MethodHandle_info.reference_index);
			break;
		case CONSTANT_MethodType:
			printf("MethodType Descriptor Index: %04x\n", cp_aux->cp_union.CONSTANT_MethodType_info.descriptor_index);
			break;
		case CONSTANT_InvokeDynamic:
			printf("InvokeDynamic - Bootstrap Method Attr Index: %04x\n", cp_aux->cp_union.CONSTANT_InvokeDynamic_info.bootstrap_method_attr_index);
			printf("InvokeDynamic - Name and Type Index: %04x\n", cp_aux->cp_union.CONSTANT_InvokeDynamic_info.name_and_type_index);
			break;
		default:
			break;
		}
	}

	printf("\n\n-----| INTERFACES |-----\n\n");
	contador = 0;
	for (u2 *auxInterfaces = cf->interfaces; auxInterfaces < cf->interfaces + cf->interfaces_count; auxInterfaces++)
	{
		ponteiroprint = decode_name_index_and_type(cf->constant_pool, *auxInterfaces, NAME_INDEX);
		printf("Interface: %s\n", ponteiroprint);
	}

	printf("\n\n-----| FIELDS |-----\n\n");
	contador = 0;
	for (auxField = cf->fields; auxField < cf->fields + cf->fields_count; auxField++, contador++)
	{
		printf("--------------| Field [%d] |--------------\n\n", contador);
		ponteiroprint = decode_utf8_string(cf->constant_pool - 1 + auxField->name_index);
		printf("Name: %s\n", ponteiroprint);
		ponteiroprint = decode_utf8_string(cf->constant_pool - 1 + auxField->descriptor_index);
		printf("Descriptor: %s\n", ponteiroprint);
		ponteiroprint = decode_field_access_flags(auxField->access_flags);
		printf("Access Flags: 0x%04x [%s]\n", auxField->access_flags, ponteiroprint);
		printf("Attributes Count: %d\n", auxField->attributes_count);

		if (auxField->attributes_count > 0)
		{
			fieldAttrAux = auxField->attributes;

			for (int posicaoFields = 0; posicaoFields < auxField->attributes_count; posicaoFields++)
			{
				ponteiroprint = decode_utf8_string(cf->constant_pool - 1 + (*(fieldAttrAux + posicaoFields))->attribute_name_index);
				printf("Attribute Name Index: %s\n", ponteiroprint);
				printf("Attribute Length: %d\n", (*(fieldAttrAux + posicaoFields))->attribute_length);

				if (strcmp(ponteiroprint, "ConstantValue") == 0)
				{
					constant_value_attribute *cvAux = (constant_value_attribute *)(*(fieldAttrAux + posicaoFields))->info;
					cp_info *cpInfoAux = cf->constant_pool - 1 + cvAux->constant_value_index;

					// FLOAT
					if (cpInfoAux->tag == CONSTANT_Float)
					{
						float valueFloat = decode_float_info(cf->constant_pool - 1 + cvAux->constant_value_index);
						printf("Constant Value Index: %f\n", valueFloat);

						// Integer-Byte-Boolean-Short-Char
					}
					else if (cpInfoAux->tag == CONSTANT_Integer)
					{
						int valueInteger = (cf->constant_pool - 1 + cvAux->constant_value_index)->cp_union.CONSTANT_Integer_info.bytes;
						printf("Constant Value Index: %d\n", valueInteger);

						// STRING
					}
					else if (cpInfoAux->tag == CONSTANT_String)
					{
						char *valueString = decode_name_index_and_type(cf->constant_pool, cvAux->constant_value_index, NAME_INDEX);
						printf("Constant Value Index: %s\n", valueString);

						// DOUBLE
					}
					else if (cpInfoAux->tag == CONSTANT_Double)
					{
						double doubleValue = decode_double_info(cf->constant_pool - 1 + cvAux->constant_value_index);
						printf("Constant Value Index: %lf\n", doubleValue);

						// LONG
					}
					else if (cpInfoAux->tag == CONSTANT_Long)
					{
						uint64_t longValue = decode_long_info(cf->constant_pool - 1 + cvAux->constant_value_index);
						printf("Constant Value Index: %I64u\n", longValue);
					}
				}
				else if (strcmp(ponteiroprint, "Signature") == 0)
				{
					signature_attribute *sig = (signature_attribute *)(*(fieldAttrAux + posicaoFields))->info;

					char *sigIndexValue = decode_utf8_string(cf->constant_pool - 1 + sig->signature_index);
					printf("Signature index: %s\n", sigIndexValue);
				}
			}
		}
	}

	printf("\n\n-----| METODOS |-----\n\n");
	contador = 0;
	for (auxMethod = cf->methods; auxMethod < cf->methods + cf->methods_count; auxMethod++, contador++)
	{
		printf("--------------| Metodo [%d] |--------------\n\n", contador);
		ponteiroprint = decode_utf8_string(cf->constant_pool - 1 + auxMethod->name_index);
		printf("Name: %s\n", ponteiroprint);
		ponteiroprint = decode_utf8_string(cf->constant_pool - 1 + auxMethod->descriptor_index);
		printf("Descriptor: %s\n", ponteiroprint);
		ponteiroprint = decode_field_access_flags(auxMethod->access_flags);
		printf("Access Flags: 0x%04x [%s]\n", auxMethod->access_flags, ponteiroprint);
		printf("Attributes Count: %d\n", auxMethod->attributes_count);

		printf("Atributos:\n");
		attribute_info **auxAttrCompleto = auxMethod->attributes;
		for (int posicao = 0; posicao < auxMethod->attributes_count; posicao++)
		{
			ponteiroprint = decode_utf8_string(cf->constant_pool - 1 + (*(auxAttrCompleto + posicao))->attribute_name_index);
			printf("Attribute Name Index: cp_info#%d <%s>\n", (*(auxAttrCompleto + posicao))->attribute_name_index, ponteiroprint);
			printf("Attribute Length: %d\n", (*(auxAttrCompleto + posicao))->attribute_length);

			if (strcmp(ponteiroprint, "Code") == 0)
			{
				code_attribute *auxCodePontual = (code_attribute *)(*(auxAttrCompleto + posicao))->info;
				printf("Max Stack: %d\n", auxCodePontual->max_stack);
				printf("Max Locals: %d\n", auxCodePontual->max_locals);
				printf("Code length: %d\n", auxCodePontual->code_length);

				printf("Code:");
				for (int i = 0; i < auxCodePontual->code_length; i++) {
					decode_opcode(auxCodePontual->code[i]);
				}
			}
			else if (strcmp(ponteiroprint, "Exceptions") == 0)
			{
				exception_attribute *excpAux = (exception_attribute *)(*(auxAttrCompleto + posicao))->info;
				int contadorExcp = 0;
				char *exceptionIndexString;
				printf("Nr.\t\tException\t\t\tVerbose\n");
				for (u2 *indexExcp = excpAux->exception_index_table; indexExcp < excpAux->exception_index_table + excpAux->number_of_exceptions; indexExcp++)
				{
					exceptionIndexString = decode_name_index_and_type(cf->constant_pool, *indexExcp, NAME_INDEX);
					printf("%d\t\tcp_info#%d\t\t\t%s\n", contadorExcp, *indexExcp, exceptionIndexString);
					contadorExcp++;
				}
			}
			else if (strcmp(ponteiroprint, "Signature") == 0)
			{
				signature_attribute *sig = (signature_attribute *)(*(auxAttrCompleto + posicao))->info;
				char *Signature_Index = decode_utf8_string(cf->constant_pool - 1 + sig->signature_index);
				printf("Signature index: cp_info#%d <%s>\n", sig->signature_index, Signature_Index);
			}
		}
	}

	printf("\n-----| ATRIBUTOS DA CLASSE |-----\n\n");

	auxAttributeClasse = cf->attributes;
	
	for (int posicao = 0; posicao < cf->attributes_count; posicao++)
	{
		ponteiroprint = decode_utf8_string(cf->constant_pool + (*(auxAttributeClasse + posicao))->attribute_name_index - 1);
		printf("Attribute Name Index: cp_info#%d <%s>\n", (*(auxAttributeClasse + posicao))->attribute_name_index, ponteiroprint);
		printf("Attribute Length: %d\n", (int)(*(auxAttributeClasse + posicao))->attribute_length);

		if (strcmp(ponteiroprint, "SourceFile") == 0)
		{
			source_file_attribute *SourceFile = ((source_file_attribute *)((*(auxAttributeClasse + posicao))->info));
			printf("Source File Name Index: cp_info#%d <%s>\n", SourceFile->sourcefile_index, decode_utf8_string(cf->constant_pool + SourceFile->sourcefile_index - 1));
		}
		else if (strcmp(ponteiroprint, "InnerClasses") == 0)
		{
			inner_classes_attribute *innerC = ((inner_classes_attribute *)((*(auxAttributeClasse + posicao))->info));
			printf("Nr.\t\tInner Class\t\t\tOuter Class\t\tInner Name\t\tAccess Flags\n");
			char *innerClassString, *outerClassString, *innerNameIndex, *accessFlagsInner;
			classes **vetorClasses = innerC->classes;

			for (int posicaoInncerC = 0; posicaoInncerC < innerC->number_of_classes; posicaoInncerC++)
			{
				innerClassString = decode_name_index_and_type(cf->constant_pool, (*(vetorClasses + posicaoInncerC))->inner_class_info_index, NAME_INDEX);
				outerClassString = decode_name_index_and_type(cf->constant_pool, (*(vetorClasses + posicaoInncerC))->outer_class_info_index, NAME_INDEX);
				innerNameIndex = decode_utf8_string(cf->constant_pool - 1 + (*(vetorClasses + posicaoInncerC))->inner_name_index);
				accessFlagsInner = decode_field_access_flags((*(vetorClasses + posicaoInncerC))->inner_class_access_flags);
				printf("%d\t\tcp_info#%d\t\t\tcp_info#%d\t\tcp_info#%d\t\t0x%04x\n", posicaoInncerC, (*(vetorClasses + posicaoInncerC))->inner_class_info_index, (*(vetorClasses + posicaoInncerC))->outer_class_info_index, (*(vetorClasses + posicaoInncerC))->inner_name_index, (*(vetorClasses + posicaoInncerC))->inner_class_access_flags);
				printf("  \t\t%s\t\t%s\t\t%s\t\t\t%s\n", innerClassString, outerClassString, innerNameIndex, accessFlagsInner);
			}
		}
		else if (strcmp(ponteiroprint, "Signature") == 0)
		{
			signature_attribute *sig = (signature_attribute *)((*(auxAttributeClasse + posicao))->info);
			char *Signature_Index = decode_utf8_string(cf->constant_pool - 1 + sig->signature_index);
			printf("Signature index: cp_info#%d <%s>\n", sig->signature_index, Signature_Index);
		}
	}
}

void decode_opcode(u1 code) {
	switch(code) {
		case 0x00: printf("\tnop\n"); break;
		case 0x01: printf("\ttaconst_null\n"); break;
		case 0x02: printf("\ticonst_ml\n"); break;
		case 0x03: printf("\ticonst_0\n"); break;
		case 0x04: printf("\ticonst_1\n"); break;
		case 0x05: printf("\ticonst_2\n"); break;
		case 0x06: printf("\ticonst_3\n"); break;
		case 0x07: printf("\ticonst_4\n"); break;
		case 0x08: printf("\ticonst_5\n"); break;
		case 0x09: printf("\tlconst_0\n"); break;
		case 0x0a: printf("\tlconst_1\n"); break;
		case 0x0b: printf("\tfconst_0\n"); break;
		case 0x0c: printf("\tfconst_1\n"); break;
		case 0x0d: printf("\tfconst_2\n"); break;
		case 0x0e: printf("\tdconst_0\n"); break;
		case 0x0f: printf("\tdconst_1\n"); break;
		case 0x10: printf("\tbipush\n"); break;
		case 0x11: printf("\tsipush\n"); break;
		case 0x12: printf("\tldc\n"); break;
		case 0x13: printf("\tldc_w\n"); break;
		case 0x14: printf("\tldc2_w\n"); break;
		case 0x15: printf("\tiload\n"); break;
		case 0x16: printf("\tlload\n"); break;
		case 0x17: printf("\tfload\n"); break;
		case 0x18: printf("\tdload\n"); break;
		case 0x19: printf("\taload\n"); break;
		case 0x1a: printf("\tiload_0\n"); break;
		case 0x1b: printf("\tiload_1\n"); break;
		case 0x1c: printf("\tiload_2\n"); break;
		case 0x1d: printf("\tiload_3\n"); break;
		case 0x1e: printf("\tlload_0\n"); break;
		case 0x1f: printf("\tlload_1\n"); break;
		case 0x20: printf("\tiload_2\n"); break;
		case 0x21: printf("\tiload_3\n"); break;
		case 0x22: printf("\tfload_0\n"); break;
		case 0x23: printf("\tfload_1\n"); break;
		case 0x24: printf("\tfload_2\n"); break;
		case 0x25: printf("\tfload_3\n"); break;
		case 0x26: printf("\tdload_0\n"); break;
		case 0x27: printf("\tdload_1\n"); break;
		case 0x28: printf("\tdload_2\n"); break;
		case 0x29: printf("\tdload_3\n"); break;
		case 0x2a: printf("\taload_0\n"); break;
		case 0x2b: printf("\taload_1\n"); break;
		case 0x2c: printf("\taload_2\n"); break;
		case 0x2d: printf("\taload_3\n"); break;
		case 0x2e: printf("\tiaload\n"); break;
		case 0x2f: printf("\tlaload\n"); break;
		case 0x30: printf("\tfaload\n"); break;
		case 0x31: printf("\tdaload\n"); break;
		case 0x32: printf("\taaload\n"); break;
		case 0x33: printf("\tbaload\n"); break;
		case 0x34: printf("\tcaload\n"); break;
		case 0x35: printf("\tsaload\n"); break;
		case 0x36: printf("\tistore\n"); break;
		case 0x37: printf("\tlstore\n"); break;
		case 0x38: printf("\tfstore\n"); break;
		case 0x39: printf("\tdstore\n"); break;
		case 0x3a: printf("\tastore\n"); break;
		case 0x3b: printf("\tistore_0\n"); break;
		case 0x3c: printf("\tistore_1\n"); break;
		case 0x3d: printf("\tistore_2\n"); break;
		case 0x3e: printf("\tistore_3\n"); break;
		case 0x3f: printf("\tlstore_0\n"); break;
		case 0x40: printf("\tlstore_1\n"); break;
		case 0x41: printf("\tlstore_2\n"); break;
		case 0x42: printf("\tlstore_3\n"); break;
		case 0x43: printf("\tfstore_0\n"); break;
		case 0x44: printf("\tfstore_1\n"); break;
		case 0x45: printf("\tfstore_2\n"); break;
		case 0x46: printf("\tfstore_3\n"); break;
		case 0x47: printf("\tdstore_0\n"); break;
		case 0x48: printf("\tdstore_1\n"); break;
		case 0x49: printf("\tdstore_2\n"); break;
		case 0x4a: printf("\tdstore_3\n"); break;
		case 0x4b: printf("\tastore_0\n"); break;
		case 0x4c: printf("\tastore_1\n"); break;
		case 0x4d: printf("\tastore_2\n"); break;
		case 0x4e: printf("\tastore_3\n"); break;
		case 0x4f: printf("\tiastore\n"); break;
		case 0x50: printf("\tlastore\n"); break;
		case 0x51: printf("\tfastore\n"); break;
		case 0x52: printf("\tdastore\n"); break;
		case 0x53: printf("\taastore\n"); break;
		case 0x54: printf("\tbastore\n"); break;
		case 0x55: printf("\tcastore\n"); break;
		case 0x56: printf("\tsastore\n"); break;
		case 0x57: printf("\tpop\n"); break;
		case 0x58: printf("\tpop2\n"); break;
		case 0x59: printf("\tdup\n"); break;
		case 0x5a: printf("\tdup_x1\n"); break;
		case 0x5b: printf("\tdup_x2\n"); break;
		case 0x5c: printf("\tdup2\n"); break;
		case 0x5d: printf("\tdup2_x1\n"); break;
		case 0x5e: printf("\tdup2_x2\n"); break;
		case 0x5f: printf("\tswap\n"); break;
		case 0x60: printf("\tiadd\n"); break;
		case 0x61: printf("\tladd\n"); break;
		case 0x62: printf("\tfadd\n"); break;
		case 0x63: printf("\tdadd\n"); break;
		case 0x64: printf("\tisub\n"); break;
		case 0x65: printf("\tlsub\n"); break;
		case 0x66: printf("\tfsub\n"); break;
		case 0x67: printf("\tdsub\n"); break;
		case 0x68: printf("\timul\n"); break;
		case 0x69: printf("\tlmul\n"); break;
		case 0x6a: printf("\tfmul\n"); break;
		case 0x6b: printf("\tdmul\n"); break;
		case 0x6c: printf("\tidiv\n"); break;
		case 0x6d: printf("\tldiv\n"); break;
		case 0x6e: printf("\tfdiv\n"); break;
		case 0x6f: printf("\tddiv\n"); break;
		case 0x70: printf("\tirem\n"); break;
		case 0x71: printf("\tlrem\n"); break;
		case 0x72: printf("\tfrem\n"); break;
		case 0x73: printf("\tdrem\n"); break;
		case 0x74: printf("\tineg\n"); break;
		case 0x75: printf("\tlneg\n"); break;
		case 0x76: printf("\tfneg\n"); break;
		case 0x77: printf("\tdneg\n"); break;
		case 0x78: printf("\tishl\n"); break;
		case 0x79: printf("\tlshl\n"); break;
		case 0x7a: printf("\tishr\n"); break;
		case 0x7b: printf("\tlshr\n"); break;
		case 0x7c: printf("\tiushr\n"); break;
		case 0x7d: printf("\tlushr\n"); break;
		case 0x7e: printf("\tiand\n"); break;
		case 0x7f: printf("\tland\n"); break;
		case 0x80: printf("\tior\n"); break;
		case 0x81: printf("\tlor\n"); break;
		case 0x82: printf("\tixor\n"); break;
		case 0x83: printf("\tlxor\n"); break;
		case 0x84: printf("\tiinc\n"); break;
		case 0x85: printf("\ti2l\n"); break;
		case 0x86: printf("\ti2f\n"); break;
		case 0x87: printf("\ti2d\n"); break;
		case 0x88: printf("\tl2i\n"); break;
		case 0x89: printf("\tl2f\n"); break;
		case 0x8a: printf("\tl2d\n"); break;
		case 0x8b: printf("\tf2i\n"); break;
		case 0x8c: printf("\tf2l\n"); break;
		case 0x8d: printf("\tf2d\n"); break;
		case 0x8e: printf("\td2i\n"); break;
		case 0x8f: printf("\td2l\n"); break;
		case 0x90: printf("\td2f\n"); break;
		case 0x91: printf("\ti2b\n"); break;
		case 0x92: printf("\ti2c\n"); break;
		case 0x93: printf("\ti2s\n"); break;
		case 0x94: printf("\tlcmp\n"); break;
		case 0x95: printf("\tfcmpl\n"); break;
		case 0x96: printf("\tfcmpg\n"); break;
		case 0x97: printf("\tdcmpl\n"); break;
		case 0x98: printf("\tdcmpg\n"); break;
		case 0x99: printf("\tifeq\n"); break;
		case 0x9a: printf("\tifne\n"); break;
		case 0x9b: printf("\tiflt\n"); break;
		case 0x9c: printf("\tifge\n"); break;
		case 0x9d: printf("\tifgt\n"); break;
		case 0x9e: printf("\tifle\n"); break;
		case 0x9f: printf("\tif_icmpeq\n"); break;		
		case 0xa0: printf("\tif_icmpne\n"); break;
		case 0xa1: printf("\tif_icmplt\n"); break;
		case 0xa2: printf("\tif_icmpge\n"); break;
		case 0xa3: printf("\tif_icmpgt\n"); break;
		case 0xa4: printf("\tif_icmple\n"); break;
		case 0xa5: printf("\tif_acmpeq\n"); break;
		case 0xa6: printf("\tif_acmpne\n"); break;
		case 0xa7: printf("\tgoto\n"); break;
		case 0xa8: printf("\tjsr\n"); break;
		case 0xa9: printf("\tret\n"); break;
		case 0xaa: printf("\ttableswitch\n"); break;
		case 0xab: printf("\tlookupswitch\n"); break;
		case 0xac: printf("\tireturn\n"); break;
		case 0xad: printf("\tlreturn\n"); break;
		case 0xae: printf("\tfreturn\n"); break;
		case 0xaf: printf("\tdreturn\n"); break;
		case 0xb0: printf("\tareturun\n"); break;
		case 0xb1: printf("\tret\n"); break;
		case 0xb2: printf("\tgetstatic\n"); break;
		case 0xb3: printf("\tputstatic\n"); break;
		case 0xb4: printf("\tgetfield\n"); break;
		case 0xb5: printf("\tputfield\n"); break;
		case 0xb6: printf("\tinvokevirtual\n"); break;
		case 0xb7: printf("\tinvokespecial\n"); break;
		case 0xb8: printf("\tinvokestatic\n"); break;
		case 0xb9: printf("\tinvokeinterface\n"); break;
		case 0xba: printf("\tinvokedynamic\n"); break;
		case 0xbb: printf("\tnew\n"); break;
		case 0xbc: printf("\tnewarray\n"); break;
		case 0xbd: printf("\tanewarray\n"); break;
		case 0xbe: printf("\tarraylength\n"); break;
		case 0xbf: printf("\tathrow\n"); break;
		case 0xc0: printf("\tcheckcast\n"); break;
		case 0xc1: printf("\tinstanceof\n"); break;
		case 0xc2: printf("\tmonitorenter\n"); break;
		case 0xc3: printf("\tmonitorexit\n"); break;
		case 0xc4: printf("\twide\n"); break;
		case 0xc5: printf("\tmultianewarray\n"); break;
		case 0xc6: printf("\tifnull\n"); break;
		case 0xc7: printf("\tifnonnull\n"); break;
		case 0xc8: printf("\tgoto_w\n"); break;
		case 0xc9: printf("\tjsr_w\n"); break;
		case 0xca: printf("\tbreakpoint\n"); break;
		case 0xfe: printf("\timpdep1\n"); break;
		case 0xff: printf("\timpdep2\n"); break;
	}
}
