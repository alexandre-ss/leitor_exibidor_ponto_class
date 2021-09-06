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
	long value = ((long)cp->cp_union.CONSTANT_Double_info.high_bytes << 32) + cp->cp_union.CONSTANT_Double_info.low_bytes;

	int sign = ((value >> 63) == 0) ? 1 : -1;
	int exp = ((value >> 52) & 0x7ffL);
	long mant = (exp == 0) ? ((value & 0xfffffffffffffL) << 1) : ((value & 0xfffffffffffffL) | 0x10000000000000L);

	double doubleValue = sign * mant * pow(2, exp - 1075);
	return doubleValue;
}

long decode_long_info(cp_info *cp)
{
	long longValue = (((long)cp->cp_union.CONSTANT_Long_info.high_bytes) << 32) | ((long)cp->cp_union.CONSTANT_Long_info.low_bytes);
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
	exception_table *exceptionTableAux;
	uint32_t contador = 1;
	char *ponteiroprint;

	printf("\n-----| INFO |-----\n\n");
	printf("Magic: %08x\n", cf->magic);
	printf("Minor Version: %04x\n", cf->minor_version);
	printf("Major Version: %04x\n", cf->major_version);
	printf("Constant Pool Count: %04x\n", cf->constant_pool_count);
	printf("Access Flags: %04x\n", cf->access_flags);
	printf("This Class: %04x\n", cf->this_class);
	printf("Super Class: %04x\n", cf->super_class);
	printf("Interfaces Count: %04x\n", cf->interfaces_count);
	printf("Fields Count: %04x\n", cf->fields_count);
	printf("Methods Count: %04x\n", cf->methods_count);
	printf("Atributes Count: %02x\n", cf->attributes_count);

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
			printf("Class Name: cp_info#%02d <%s>\n", cp_aux->cp_union.CONSTANT_Class_info.name_index, ponteiroprint);
			break;
		case CONSTANT_Fieldref:
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_Fieldref_info.class_index, NAME_INDEX);
			printf("Class Name: cp_info#%02d <%s>\n", cp_aux->cp_union.CONSTANT_Fieldref_info.class_index, ponteiroprint);
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_Fieldref_info.name_and_type_index, NAME_AND_TYPE);
			printf("Name and Type: cp_info#%02d <%s>\n", cp_aux->cp_union.CONSTANT_Fieldref_info.name_and_type_index, ponteiroprint);
			break;
		case CONSTANT_Methodref:
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_Methodref_info.class_index, NAME_INDEX);
			printf("Class Name: cp_info#%02d <%s>\n", cp_aux->cp_union.CONSTANT_Methodref_info.class_index, ponteiroprint);
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_Methodref_info.name_and_type_index, NAME_AND_TYPE);
			printf("Name and Type: cp_info#%02d <%s>\n", cp_aux->cp_union.CONSTANT_Methodref_info.name_and_type_index, ponteiroprint);
			break;
		case CONSTANT_InterfaceMethodref:
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_String_info.string_index, NAME_INDEX);
			printf("Class Name: cp_info#%02d <%s>\n", cp_aux->cp_union.CONSTANT_InterfaceMethodref_info.class_index, ponteiroprint);
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_String_info.string_index, NAME_AND_TYPE);
			printf("Name and Type Index: cp_info#%02d <%s>\n", cp_aux->cp_union.CONSTANT_InterfaceMethodref_info.name_and_type_index, ponteiroprint);
			break;
		case CONSTANT_String:
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_String_info.string_index, STRING_INDEX);
			printf("String: cp_info#%02d <%s>\n", cp_aux->cp_union.CONSTANT_String_info.string_index, ponteiroprint);
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
			printf("Long: %lu\n", longValue);
			break;
		case CONSTANT_Double:
			valor = decode_double_info(cp_aux);
			printf("Double High Bytes: 0x%08x\n", cp_aux->cp_union.CONSTANT_Double_info.high_bytes);
			printf("Double Low Bytes: 0x%08x\n", cp_aux->cp_union.CONSTANT_Double_info.low_bytes);
			printf("Double: %lf\n", valor);
			break;
		case CONSTANT_NameAndType:
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_NameAndType_info.name_index, NAME_AND_TYPE_INFO_NAME_INDEX);
			printf("Name: cp_info#%02d <%s>\n", cp_aux->cp_union.CONSTANT_NameAndType_info.name_index, ponteiroprint);
			ponteiroprint = decode_name_index_and_type(cf->constant_pool, cp_aux->cp_union.CONSTANT_NameAndType_info.descriptor_index, NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX);
			printf("Descriptor: cp_info#%02d <%s>\n", cp_aux->cp_union.CONSTANT_NameAndType_info.descriptor_index, ponteiroprint);
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
			printf("DefaultImpressao\n");
			break;
		}
	}

	printf("\n\n-----| INTERFACES |-----\n\n");
	contador = 0;
	for (u2 *auxInterfaces = cf->interfaces; auxInterfaces < cf->interfaces + cf->interfaces_count; auxInterfaces++)
	{
		ponteiroprint = decode_name_index_and_type(cf->constant_pool, *auxInterfaces, NAME_INDEX);
		printf("Interface: cp_info#%d <%s>\n", *auxInterfaces, ponteiroprint);
	}

	printf("\n\n-----| FIELDS |-----\n\n");
	contador = 0;
	for (auxField = cf->fields; auxField < cf->fields + cf->fields_count; auxField++, contador++)
	{
		printf("--------------| Field [%d] |--------------\n\n", contador);
		ponteiroprint = decode_utf8_string(cf->constant_pool - 1 + auxField->name_index);
		printf("Name: cp_info#%d <%s>\n", auxField->name_index, ponteiroprint);
		ponteiroprint = decode_utf8_string(cf->constant_pool - 1 + auxField->descriptor_index);
		printf("Descriptor: cp_info#%d <%s>\n", auxField->descriptor_index, ponteiroprint);
		ponteiroprint = decode_access_flags(auxField->access_flags);
		printf("Access Flags: 0x%04x [%s]\n", auxField->access_flags, ponteiroprint);
		printf("Attributes Count: %d\n", auxField->attributes_count);

		if (auxField->attributes_count > 0)
		{
			fieldAttrAux = auxField->attributes;

			for (int posicaoFields = 0; posicaoFields < auxField->attributes_count; posicaoFields++)
			{
				ponteiroprint = decode_utf8_string(cf->constant_pool - 1 + (*(fieldAttrAux + posicaoFields))->attribute_name_index);
				printf("Attribute Name Index: cp_info#%d <%s>\n", (*(fieldAttrAux + posicaoFields))->attribute_name_index, ponteiroprint);
				printf("Attribute Length: %d\n", (*(fieldAttrAux + posicaoFields))->attribute_length);

				if (strcmp(ponteiroprint, "ConstantValue") == 0)
				{
					constant_value_attribute *cvAux = (constant_value_attribute *)(*(fieldAttrAux + posicaoFields))->info;
					cp_info *cpInfoAux = cf->constant_pool - 1 + cvAux->constant_value_index;

					// FLOAT
					if (cpInfoAux->tag == CONSTANT_Float)
					{
						float valueFloat = decode_float_info(cf->constant_pool - 1 + cvAux->constant_value_index);
						printf("Constant Value Index: cp_info#%d <%f>\n", cvAux->constant_value_index, valueFloat);

						// Integer-Byte-Boolean-Short-Char
					}
					else if (cpInfoAux->tag == CONSTANT_Integer)
					{
						int valueInteger = (cf->constant_pool - 1 + cvAux->constant_value_index)->cp_union.CONSTANT_Integer_info.bytes;
						printf("Constant Value Index: cp_info#%d <%d>\n", cvAux->constant_value_index, valueInteger);

						// STRING
					}
					else if (cpInfoAux->tag == CONSTANT_String)
					{
						char *valueString = decode_name_index_and_type(cf->constant_pool, cvAux->constant_value_index, NAME_INDEX);
						printf("Constant Value Index: cp_info#%d <%s>\n", cvAux->constant_value_index, valueString);

						// DOUBLE
					}
					else if (cpInfoAux->tag == CONSTANT_Double)
					{
						double doubleValue = decode_double_info(cf->constant_pool - 1 + cvAux->constant_value_index);
						printf("Constant Value Index: cp_info#%d <%lf>\n", cvAux->constant_value_index, doubleValue);

						// LONG
					}
					else if (cpInfoAux->tag == CONSTANT_Long)
					{
						uint64_t longValue = decode_long_info(cf->constant_pool - 1 + cvAux->constant_value_index);
						printf("Constant Value Index: cp_info#%d <%lu>\n", cvAux->constant_value_index, longValue);
					}
				}
				else if (strcmp(ponteiroprint, "Signature") == 0)
				{
					signature_attribute *sig = (signature_attribute *)(*(fieldAttrAux + posicaoFields))->info;

					char *sigIndexValue = decode_utf8_string(cf->constant_pool - 1 + sig->signature_index);
					printf("Signature index: cp_info#%d <%s>\n", sig->signature_index, sigIndexValue);
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
		printf("Name: cp_info#%d <%s>\n", auxMethod->name_index, ponteiroprint);
		ponteiroprint = decode_utf8_string(cf->constant_pool - 1 + auxMethod->descriptor_index);
		printf("Descriptor: cp_info#%d <%s>\n", auxMethod->descriptor_index, ponteiroprint);
		ponteiroprint = decode_access_flags(auxMethod->access_flags);
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

	printf("\n\n-----| ATRIBUTOS DA CLASSE |-----\n\n");

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
				accessFlagsInner = decode_access_flags((*(vetorClasses + posicaoInncerC))->inner_class_access_flags);
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
