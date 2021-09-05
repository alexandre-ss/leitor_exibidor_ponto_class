#include "exibidor.h"
#include <string.h>

void print_name_tag(u1 tag) {
  char name[30];

  switch (tag) {
    case CONSTANT_Class:
			strcpy(name,"CONSTANT_Class_Info");
			break;
		case CONSTANT_Fieldref:
			strcpy(name,"CONSTANT_Field_Info");
			break;
		case CONSTANT_Methodref:
			strcpy(name,"CONSTANT_Method_Info");
			break;
		case CONSTANT_InterfaceMethodref:
			strcpy(name,"CONSTANT_InterfaceMethod_Info");
			break;
		case CONSTANT_String:
			strcpy(name,"CONSTANT_String_Info");
			break;
		case CONSTANT_Integer:
			strcpy(name,"CONSTANT_Integer_Info");
			break;
		case CONSTANT_Float:
			strcpy(name,"CONSTANT_Float_Info");
			break;
		case CONSTANT_Long:
			strcpy(name,"CONSTANT_Long_Info");
			break;
		case CONSTANT_Double:
			strcpy(name,"CONSTANT_Double_Info");
			break;
		case CONSTANT_NameAndType:
			strcpy(name,"CONSTANT_NameAndType_Info");
			break;
		case CONSTANT_Utf8:
			strcpy(name,"CONSTANT_Utf8_Info");
			break;
		case CONSTANT_MethodHandle:
			strcpy(name,"CONSTANT_MethodHandle_Info");
			break;
		case CONSTANT_MethodType:
			strcpy(name,"CONSTANT_MethodType_Info");
			break;
		case CONSTANT_InvokeDynamic:
			strcpy(name,"CONSTANT_InvokeDynamic_Info");
			break;
		default:
			return;
  }

  printf("TAG: %s\n", name);
}

char* decode_utf8_string(cp_info *cp){
	char decoded[cp->cp_union.CONSTANT_Utf8_info.length + 1];
  int i;

	for (i = 0; i < cp->cp_union.CONSTANT_Utf8_info.length; i++){
		decoded[i] = (char) cp->cp_union.CONSTANT_Utf8_info.bytes[i];
	}

	decoded[i] = '\0';

	return decoded;
}
