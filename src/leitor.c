#include "leitor.h"

ClassFile *read_file(char *filename)
{
  ClassFile *classFile;
  classFile = (ClassFile *)malloc(sizeof(ClassFile));
  FILE *file = fopen(filename, "rb");

  classFile->magic = u4_READ(file);
  classFile->minor_version = u2_READ(file);
  classFile->major_version = u2_READ(file);
  classFile->constant_pool_count = u2_READ(file);

  classFile->constant_pool = read_constant_pool(file, classFile->constant_pool, classFile->constant_pool_count);

  classFile->access_flags = u2_READ(file);
  classFile->this_class = u2_READ(file);
  classFile->super_class = u2_READ(file);

  classFile->interfaces_count = u2_READ(file);
  classFile->interfaces = (classFile->interfaces_count > 0) ? read_interfaces(file, classFile->interfaces, classFile->interfaces_count) : NULL;

  classFile->fields_count = u2_READ(file);
  classFile->fields = (classFile->fields_count > 0) ? read_fields(file, classFile->fields, classFile->fields_count, classFile->constant_pool) : NULL;

  classFile->methods_count = u2_READ(file);
  classFile->methods = (classFile->methods_count > 0) ? read_methods(file, classFile->methods, classFile->methods_count, classFile->constant_pool) : NULL;

  classFile->attributes_count = u2_READ(file);

  printf("Magic: %08x\n", classFile->magic);
  printf("Minor Version: %04x\n", classFile->minor_version);
  printf("Major Version: %04x\n", classFile->major_version);
  printf("Constant Pool Count: %04x\n", classFile->constant_pool_count);
  printf("Access Flags: %04x\n", classFile->access_flags);
  printf("This Class: %04x\n", classFile->this_class);
  printf("Super Class: %04x\n", classFile->super_class);
  printf("Interfaces Count: %04x\n", classFile->interfaces_count);
  printf("Fields Count: %04x\n", classFile->fields_count);
  printf("Methods Count: %04x\n", classFile->methods_count);
  printf("Atributes Count: %02x\n", classFile->attributes_count);

  free(classFile);
  fclose(file);
}

cp_info *read_constant_pool(FILE *file, cp_info *constant_pool, u2 constant_pool_count)
{
  constant_pool = (cp_info *)malloc(constant_pool_count * sizeof(cp_info));

  cp_info *cp;
  for (cp = constant_pool; cp < constant_pool + constant_pool_count - 1; cp++)
  {
    cp->tag = u1_READ(file);

    switch (cp->tag)
    {
    case CONSTANT_Class:
      cp->cp_union.CONSTANT_Class_info.tag = CONSTANT_Class;
      cp->cp_union.CONSTANT_Class_info.name_index = u2_READ(file);
      break;

    case CONSTANT_Fieldref:
      cp->cp_union.CONSTANT_Fieldref_info.tag = CONSTANT_Fieldref;
      cp->cp_union.CONSTANT_Fieldref_info.class_index = u2_READ(file);
      cp->cp_union.CONSTANT_Fieldref_info.name_and_type_index = u2_READ(file);
      break;

    case CONSTANT_Methodref:
      cp->cp_union.CONSTANT_Methodref_info.class_index = u2_READ(file);
      cp->cp_union.CONSTANT_Methodref_info.name_and_type_index = u2_READ(file);
      break;

    case CONSTANT_InterfaceMethodref:
      cp->cp_union.CONSTANT_InterfaceMethodref_info.tag = CONSTANT_InterfaceMethodref;
      cp->cp_union.CONSTANT_InterfaceMethodref_info.class_index = u2_READ(file);
      cp->cp_union.CONSTANT_InterfaceMethodref_info.name_and_type_index = u2_READ(file);
      break;

    case CONSTANT_String:
      cp->cp_union.CONSTANT_String_info.tag = CONSTANT_String;
      cp->cp_union.CONSTANT_String_info.string_index = u2_READ(file);
      break;

    case CONSTANT_Integer:
      cp->cp_union.CONSTANT_Integer_info.tag = CONSTANT_Integer;
      cp->cp_union.CONSTANT_Integer_info.bytes = u4_READ(file);
      break;

    case CONSTANT_Float:
      cp->cp_union.CONSTANT_Float_info.tag = CONSTANT_Float;
      cp->cp_union.CONSTANT_Float_info.bytes = u4_READ(file);
      break;

    case CONSTANT_Long:
      cp->cp_union.CONSTANT_Long_info.tag = CONSTANT_Long;
      cp->cp_union.CONSTANT_Long_info.high_bytes = u4_READ(file);
      cp->cp_union.CONSTANT_Long_info.low_bytes = u4_READ(file);
      cp++;
      break;

    case CONSTANT_Double:
      cp->cp_union.CONSTANT_Double_info.tag = CONSTANT_Double;
      cp->cp_union.CONSTANT_Double_info.high_bytes = u4_READ(file);
      cp->cp_union.CONSTANT_Double_info.low_bytes = u4_READ(file);
      cp++;
      break;

    case CONSTANT_NameAndType:
      cp->cp_union.CONSTANT_NameAndType_info.tag = CONSTANT_NameAndType;
      cp->cp_union.CONSTANT_NameAndType_info.name_index = u2_READ(file);
      cp->cp_union.CONSTANT_NameAndType_info.descriptor_index = u2_READ(file);
      break;

    case CONSTANT_Utf8:
      cp->cp_union.CONSTANT_Utf8_info.tag = CONSTANT_Utf8;
      cp->cp_union.CONSTANT_Utf8_info.length = u2_READ(file);
      cp->cp_union.CONSTANT_Utf8_info.bytes = malloc(cp->cp_union.CONSTANT_Utf8_info.length * sizeof(u1));
      for (u1 *i = cp->cp_union.CONSTANT_Utf8_info.bytes; i < cp->cp_union.CONSTANT_Utf8_info.bytes + cp->cp_union.CONSTANT_Utf8_info.length; i++)
      {
        *i = u1_READ(file);
      }
      break;

    case CONSTANT_MethodHandle:
      cp->cp_union.CONSTANT_MethodHandle_info.tag = CONSTANT_MethodHandle;
      cp->cp_union.CONSTANT_MethodHandle_info.reference_kind = u1_READ(file);
      cp->cp_union.CONSTANT_MethodHandle_info.reference_index = u2_READ(file);
      break;

    case CONSTANT_MethodType:
      cp->cp_union.CONSTANT_MethodType_info.tag = CONSTANT_MethodType;
      cp->cp_union.CONSTANT_MethodType_info.descriptor_index = u2_READ(file);
      break;

    case CONSTANT_InvokeDynamic:
      cp->cp_union.CONSTANT_InvokeDynamic_info.tag = CONSTANT_InvokeDynamic;
      cp->cp_union.CONSTANT_InvokeDynamic_info.bootstrap_method_attr_index = u2_READ(file);
      cp->cp_union.CONSTANT_InvokeDynamic_info.name_and_type_index = u2_READ(file);
      break;

    default:
      break;
    }
  }

  free(constant_pool);
}

method_info *read_methods(FILE *file, method_info *method, u2 method_count, cp_info *cp)
{
  method = (method_info *)malloc(method_count * sizeof(method_info));

  method_info *m;
  for (m = method; m < method + method_count - 1; m++) // Revisar -1
  {
    m->access_flags = u2_READ(file);
    m->name_index = u2_READ(file);
    m->descriptor_index = u2_READ(file);
    m->attributes_count = u2_READ(file);

    attribute_info *attributes = (attribute_info *)malloc(m->attributes_count * sizeof(attribute_info));

    while (m->attributes_count > 0)
    {
      m->attributes = (attribute_info **)malloc(m->attributes_count * sizeof(attribute_info *));
      for (int i = 0; i < m->attributes_count; i++)
      {
        *(m->attributes + 1) = read_attributes(file, *(m->attributes + i), cp);
      }
    }
  }

  return method;
}

u2 *read_interfaces(FILE *file, u2 *interface, u2 interface_count)
{
  interface = (u2 *)malloc(interface_count * sizeof(u2));

  u2 *i;
  for (i = interface; i < interface + interface_count - 1; i++) // Revisar -1
  {
    *interface = u2_READ(file);
  }
}

field_info *read_fields(FILE *file, field_info *field, u2 field_count, cp_info *cp)
{
  field = (field_info *)malloc(field_count * sizeof(field_info));

  field_info *f;
  for (f = field; f < field + field_count - 1; f++) // Revisar -1
  {
    f->access_flags = u2_READ(file);
    f->name_index = u2_READ(file);
    f->descriptor_index = u2_READ(file);
    f->attributes_count = u2_READ(file);

    while (f->attributes_count > 0)
    {
      f->attributes = (attribute_info **)malloc(f->attributes_count * sizeof(attribute_info *));
      for (int i = 0; i < f->attributes_count; i++)
      {
        *(f->attributes + 1) = read_attributes(file, *(f->attributes + i), cp);
      }
    }
  }
}

attribute_info *read_attributes(FILE *file, attribute_info *attribute, cp_info *cp)
{

  attribute = (attribute_info *)malloc(sizeof(attribute_info));
  attribute->attribute_name_index = u2_READ(file);
  attribute->attribute_length = u4_READ(file);

  if (attribute->attribute_length > 0)
  {
    char *string_name;
    string_name = decode_utf8_string(cp + attribute->attribute_name_index - 1);

    if (strcmp(string_name, "SourceFile") == 0)
    {
      source_file_attribute *source_file = NULL;
      source_file = read_source_file(file);
      attribute->info = NULL;
      attribute->info = (source_file_attribute *)source_file;
    }
    else if (strcmp(string_name, "Code") == 0){
      code_attribute *code_attr = NULL;
      code_attr = read_code(file, cp);
      attribute->info = (code_attribute *)code_attr;
    }
    else if (strcmp(string_name, "LineNumberTable") == 0)
		{
			line_number_table *line_ntable = NULL;
			line_ntable = read_line_number_table(file, cp);
			attribute->info = (line_number_table *)line_ntable;
		}
		else if (strcmp(string_name, "StackMapTable") == 0)
		{
			stack_map_table_attribute *stack_mtable = NULL;
			stack_mtable = read_stack_map_table(file);
			attribute->info = (stack_map_table_attribute *)stack_mtable;
		}
		else if (strcmp(string_name, "InnerClasses") == 0)
		{
			inner_classes_attribute *inner_classes = NULL;
			inner_classes = read_inner_classes(file, cp);
			attribute->info = (inner_classes_attribute *)inner_classes;
		}
		else if (strcmp(string_name, "Signature") == 0)
		{
			signature_attribute *signature_attr = NULL;
			signature_attr = read_signature(file);
			attribute->info = (signature_attribute *)signature_attr;
		}
		else if (strcmp(string_name, "ConstantValue") == 0)
		{
			constant_value_attribute *constant_value = NULL;
			constant_value = read_constant_value(file);
			attribute->info = (constant_value_attribute *)constant_value;
		}
		else if (strcmp(string_name, "Exceptions") == 0)
		{
			exception_attribute *exceptions = NULL;
			exceptions = read_exceptions_attribute(file);
			attribute->info = (exception_attribute *)exceptions;
		}
  }
}

//TO BE DONE

exception_attribute* read_exceptions_attribute (FILE *fp){}
signature_attribute* read_signature (FILE *fp){}
inner_classes_attribute* read_inner_classes (FILE *fp, cp_info *cp){}
stack_map_table_attribute* read_stack_map_table (FILE *fp){}
line_number_table* read_line_number_table(FILE * fp, cp_info * cp){}
code_attribute* read_code(FILE * fp, cp_info *cp){}
constant_value_attribute* read_constant_value (FILE *fp){}
source_file_attribute* read_source_file (FILE  *fp){}