#include "leitor.h"

ClassFile *read_file(char *filename)
{
  ClassFile *classFile;
  classFile = (ClassFile *)malloc(sizeof(ClassFile));
  FILE *file = fopen(filename, "rb");

  u4 magic = u4_READ(file);

  if (magic != 0xcafebabe)
  {
    free(classFile);
    return NULL;
  }
  else
  {
    classFile->magic = magic;
  }

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

  if (classFile->attributes_count > 0)
  {
    classFile->attributes = (attribute_info **)malloc(classFile->attributes_count * sizeof(attribute_info *));
    for (int i = 0; i < classFile->attributes_count; i++)
    {
      *(classFile->attributes + i) = read_attributes(file, *(classFile->attributes + i), classFile->constant_pool);
    }
  }

  fclose(file);

  return classFile;
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

  return constant_pool;
}

method_info *read_methods(FILE *file, method_info *method, u2 method_count, cp_info *cp)
{
  method = (method_info *)malloc(method_count * sizeof(method_info));

  method_info *m;
  for (m = method; m < method + method_count; m++)
  {
    m->access_flags = u2_READ(file);
    m->name_index = u2_READ(file);
    m->descriptor_index = u2_READ(file);
    m->attributes_count = u2_READ(file);

    attribute_info *attributes = (attribute_info *)malloc(m->attributes_count * sizeof(attribute_info));

    if (m->attributes_count > 0)
    {
      m->attributes = (attribute_info **)malloc(m->attributes_count * sizeof(attribute_info *));
      for (int i = 0; i < m->attributes_count; i++)
      {
        *(m->attributes + i) = read_attributes(file, *(m->attributes + i), cp);
      }
    }
  }

  return method;
}

u2 *read_interfaces(FILE *file, u2 *interface, u2 interface_count)
{
  interface = (u2 *)malloc(interface_count * sizeof(u2));

  u2 *i;
  for (i = interface; i < interface + interface_count; i++)
  {
    *interface = u2_READ(file);
  }
}

field_info *read_fields(FILE *file, field_info *field, u2 field_count, cp_info *cp)
{
  field = (field_info *)malloc(field_count * sizeof(field_info));

  field_info *f;
  for (f = field; f < field + field_count; f++)
  {
    f->access_flags = u2_READ(file);
    f->name_index = u2_READ(file);
    f->descriptor_index = u2_READ(file);
    f->attributes_count = u2_READ(file);

    if (f->attributes_count > 0)
    {
      f->attributes = (attribute_info **)malloc(f->attributes_count * sizeof(attribute_info *));
      for (int i = 0; i < f->attributes_count; i++)
      {
        *(f->attributes + i) = read_attributes(file, *(f->attributes + i), cp);
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
    else if (strcmp(string_name, "Code") == 0)
    {
      code_attribute *code_attr = NULL;
      code_attr = read_code(file, cp);
      attribute->info = (code_attribute *)code_attr;
    }
    else if (strcmp(string_name, "LineNumberTable") == 0)
    {
      line_number_table_attribute *line_ntable = NULL;
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

  return attribute;
}

exception_attribute *read_exceptions_attribute(FILE *file)
{
  exception_attribute *exceptions = (exception_attribute *)malloc(sizeof(exception_attribute));

  exceptions->number_of_exceptions = u2_READ(file);
  exceptions->exception_index_table = NULL; // Revisar

  if (exceptions->number_of_exceptions > 0)
  {
    exceptions->exception_index_table = (u2 *)malloc(exceptions->number_of_exceptions * sizeof(u2));
    for (u2 *exception_aux = exceptions->exception_index_table; exception_aux < exceptions->exception_index_table + exceptions->number_of_exceptions; exception_aux++)
    {
      *exception_aux = u2_READ(file);
    }
  }

  return exceptions;
}

signature_attribute *read_signature(FILE *file)
{
  signature_attribute *signature = (signature_attribute *)malloc(sizeof(signature_attribute));
  signature->signature_index = u2_READ(file);
  return signature;
}

inner_classes_attribute *read_inner_classes(FILE *file, cp_info *cp)
{
  inner_classes_attribute *inner_classes = (inner_classes_attribute *)malloc(sizeof(inner_classes_attribute));
  for (int pos = 0; pos < inner_classes->number_of_classes; pos++)
  {
    *(inner_classes->classes + pos) = read_classes(file);
  }
}

classes *read_classes(FILE *file)
{
  classes *class = (classes *)malloc(sizeof(classes));
  class->inner_class_info_index = u2_READ(file);
  class->outer_class_info_index = u2_READ(file);
  class->inner_name_index = u2_READ(file);
  class->inner_class_access_flags = u2_READ(file);

  return class;
}

stack_map_table_attribute *read_stack_map_table(FILE *file)
{
  stack_map_table_attribute *stack_map_table = (stack_map_table_attribute *)malloc(sizeof(stack_map_table_attribute));

  stack_map_table->number_of_entries = u2_READ(file);

  if (stack_map_table->number_of_entries > 0)
  {
    stack_map_table->entries = (stack_map_frame **)malloc(stack_map_table->number_of_entries * sizeof(stack_map_frame *));

    int i;
    for (i = 0; i < stack_map_table->number_of_entries; i++)
    {
      *(stack_map_table->entries + i) = read_stack_map_frame(file);
    }
  }

  return stack_map_table;
}

stack_map_frame *read_stack_map_frame(FILE *file)
{
  stack_map_frame *stack_map_f = (stack_map_frame *)malloc(sizeof(stack_map_frame));
  stack_map_f->frame_type = u1_READ(file);
  if (stack_map_f->frame_type >= 0 && stack_map_f->frame_type <= 63)
  {
  }
  else if (stack_map_f->frame_type >= 64 && stack_map_f->frame_type <= 127)
  {
    stack_map_f->map_frame_type.same_locals_1_stack_item_frame.stack = (verification_type_info **)malloc(sizeof(verification_type_info *));
    *(stack_map_f->map_frame_type.same_locals_1_stack_item_frame.stack) = read_verification_type_info(file);
  }
  else if (stack_map_f->frame_type == 247)
  {
    stack_map_f->map_frame_type.same_locals_1_stack_item_frame_extended.offset_delta = u2_READ(file);
    stack_map_f->map_frame_type.same_locals_1_stack_item_frame_extended.stack = (verification_type_info **)malloc(sizeof(verification_type_info *));
    *(stack_map_f->map_frame_type.same_locals_1_stack_item_frame_extended.stack) = read_verification_type_info(file);
  }
  else if (stack_map_f->frame_type >= 248 && stack_map_f->frame_type <= 250)
  {
    stack_map_f->map_frame_type.chop_frame.offset_delta = u2_READ(file);
  }
  else if (stack_map_f->frame_type == 251)
  {
    stack_map_f->map_frame_type.same_frame_extended.offset_delta = u2_READ(file);
  }
  else if (stack_map_f->frame_type >= 252 && stack_map_f->frame_type <= 254)
  {
    stack_map_f->map_frame_type.append_frame.offset_delta = u2_READ(file);
    u2 sizeMalloc = (stack_map_f->frame_type - 251);
    stack_map_f->map_frame_type.append_frame.locals = (verification_type_info **)malloc(sizeMalloc * sizeof(verification_type_info *));
    for (int i = 0; i < sizeMalloc; i++)
    {
      *(stack_map_f->map_frame_type.append_frame.locals + i) = read_verification_type_info(file);
    }
  }
  else if (stack_map_f->frame_type == 255)
  {
    stack_map_f->map_frame_type.full_frame.offset_delta = u2_READ(file);
    stack_map_f->map_frame_type.full_frame.num_locals = u2_READ(file);
    if (stack_map_f->map_frame_type.full_frame.num_locals > 0)
    {
      stack_map_f->map_frame_type.full_frame.locals = (verification_type_info **)malloc(stack_map_f->map_frame_type.full_frame.num_locals * sizeof(verification_type_info *));
      for (int i = 0; i < stack_map_f->map_frame_type.full_frame.num_locals; i++)
      {
        *(stack_map_f->map_frame_type.full_frame.locals + i) = read_verification_type_info(file);
        if ((*(stack_map_f->map_frame_type.full_frame.locals + i))->tag == 7)
        {
        }
      }
    }
    stack_map_f->map_frame_type.full_frame.num_stack_items = u2_READ(file);
    if (stack_map_f->map_frame_type.full_frame.num_stack_items > 0)
    {
      stack_map_f->map_frame_type.full_frame.stack = (verification_type_info **)malloc(stack_map_f->map_frame_type.full_frame.num_stack_items * sizeof(verification_type_info *));
      for (int i = 0; i < stack_map_f->map_frame_type.full_frame.num_stack_items; i++)
      {
        *(stack_map_f->map_frame_type.full_frame.stack + i) = read_verification_type_info(file);
      }
    }
  }
  return stack_map_f;
}

verification_type_info *read_verification_type_info(FILE *file)
{
  verification_type_info *v_type_info = (verification_type_info *)malloc(sizeof(verification_type_info));
  v_type_info->tag = u1_READ(file);
  switch (v_type_info->tag)
  {
  case 7:
    v_type_info->type_info.object_variable_info.cp = u2_READ(file);
    break;
  case 8:
    v_type_info->type_info.uninitialized_variable_info.offset = u2_READ(file);
    break;
  default:
    break;
  }
  return v_type_info;
}

line_number_table_attribute *read_line_number_table(FILE *file, cp_info *cp)
{
  line_number_table_attribute *line_nt = (line_number_table_attribute *)malloc(sizeof(line_number_table_attribute));
  line_nt->line_number_table_length = u2_READ(file);
  if (line_nt->line_number_table_length > 0)
  {
    line_nt->info = (line_number_table *)malloc(line_nt->line_number_table_length * sizeof(line_number_table));
    for (line_number_table *line_info = line_nt->info; line_info < line_nt->info + line_nt->line_number_table_length; line_info++) //verificar
    {
      line_info->start_pc = u2_READ(file);
      line_info->line_number = u2_READ(file);
    }
  }
  return line_nt;
}

code_attribute *read_code(FILE *file, cp_info *cp)
{
  code_attribute *code_attributes = NULL; // Revisar
  code_attributes = (code_attribute *)malloc(sizeof(code_attribute));
  code_attributes->max_stack = u2_READ(file);
  code_attributes->max_locals = u2_READ(file);
  code_attributes->code_length = u4_READ(file);

  if (code_attributes->code_length > 0)
  {
    code_attributes->code = malloc(code_attributes->code_length * sizeof(u1));

    for (u1 *p = code_attributes->code; p < code_attributes->code + code_attributes->code_length; p++)
      *p = u1_READ(file);
  }

  code_attributes->exception_table_length = u2_READ(file);

  if (code_attributes->exception_table_length > 0)
  {
    code_attributes->exception_table = NULL;
    code_attributes->exception_table = read_exception_table(file, code_attributes->exception_table_length);
  }

  code_attributes->attributes_count = u2_READ(file);

  if (code_attributes->attributes_count > 0)
  {
    code_attributes->attributes = (attribute_info **)malloc(code_attributes->attributes_count * sizeof(attribute_info *));

    int i;
    for (i = 0; i < code_attributes->attributes_count; i++)
    {
      *(code_attributes->attributes + i) = read_attributes(file, *(code_attributes->attributes + i), cp);
    }
  }

  return code_attributes;
}

constant_value_attribute *read_constant_value(FILE *file)
{
  constant_value_attribute *constant_value = (constant_value_attribute *)malloc(sizeof(constant_value_attribute));
  constant_value->constant_value_index = u2_READ(file);
  return constant_value;
}

source_file_attribute *read_source_file(FILE *file)
{
  source_file_attribute *source_file = NULL;
  source_file = (source_file_attribute *)malloc(sizeof(source_file_attribute));
  source_file->sourcefile_index = u2_READ(file);
  return source_file;
}

exception_table *read_exception_table(FILE *file, u2 size)
{
  exception_table *e_table = (exception_table *)malloc(size * sizeof(exception_table));
  for (exception_table *exception = e_table; exception < e_table + size; exception++)
  {
    exception->start_pc = u2_READ(file);
    exception->end_pc = u2_READ(file);
    exception->handread_pc = u2_READ(file);
    exception->catch_type = u2_READ(file);
  }
  return e_table;
}
