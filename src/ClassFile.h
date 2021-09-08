#ifndef CLASS_FILE_H
#define CLASS_FILE_H

#include <stdint.h>

typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;

#define NAME_INDEX 1
#define NAME_AND_TYPE 2
#define STRING_INDEX 3
#define CLASS_INDEX 4
#define NAME_AND_TYPE_INFO_NAME_INDEX 5
#define NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX 6

typedef struct attribute_info
{
  u2 attribute_name_index;
  u4 attribute_length;
  void *info;
} attribute_info;

typedef struct exception_table
{
  u2 start_pc;
  u2 end_pc;
  u2 handread_pc;
  u2 catch_type;
} exception_table;

// 4.1

typedef enum access_flags
{
  ACC_PUBLIC = 0x0001,
  ACC_FINAL = 0x0010,
  ACC_SUPER = 0x0020,
  ACC_INTERFACE = 0x0200,
  ACC_ABSTRACT = 0x0400,
  ACC_SYNTHETIC = 0x1000,
  ACC_ANNOTATION = 0x2000,
  ACC_ENUM = 0x4000
} access_flags;

// 4.4. Constant Pool

// Tags para a Pool de Constantes
typedef enum constant_pool_tags
{
  CONSTANT_Utf8 = 1,
  CONSTANT_Integer = 3,
  CONSTANT_Float = 4,
  CONSTANT_Long = 5,
  CONSTANT_Double = 6,
  CONSTANT_Class = 7,
  CONSTANT_String = 8,
  CONSTANT_Fieldref = 9,
  CONSTANT_Methodref = 10,
  CONSTANT_InterfaceMethodref = 11,
  CONSTANT_NameAndType = 12,
  CONSTANT_MethodHandle = 15,
  CONSTANT_MethodType = 16,
  CONSTANT_InvokeDynamic = 18
} constant_pool_tags;

typedef struct cp_info
{
  u1 tag;

  union
  {
    struct
    {
      u1 tag;
      u2 name_index;
    } CONSTANT_Class_info;

    struct
    {
      u1 tag;
      u2 class_index;
      u2 name_and_type_index;
    } CONSTANT_Fieldref_info;

    struct
    {
      u1 tag;
      u2 class_index;
      u2 name_and_type_index;
    } CONSTANT_Methodref_info;

    struct
    {
      u1 tag;
      u2 class_index;
      u2 name_and_type_index;
    } CONSTANT_InterfaceMethodref_info;

    struct
    {
      u1 tag;
      u2 string_index;
    } CONSTANT_String_info;

    struct
    {
      u1 tag;
      u4 bytes;
    } CONSTANT_Integer_info;

    struct
    {
      u1 tag;
      u4 bytes;
    } CONSTANT_Float_info;

    struct
    {
      u1 tag;
      u4 high_bytes;
      u4 low_bytes;
    } CONSTANT_Long_info;

    struct
    {
      u1 tag;
      u4 high_bytes;
      u4 low_bytes;
    } CONSTANT_Double_info;

    struct
    {
      u1 tag;
      u2 name_index;
      u2 descriptor_index;
    } CONSTANT_NameAndType_info;

    struct
    {
      u1 tag;
      u2 length;
      u1 *bytes;
    } CONSTANT_Utf8_info;

    struct
    {
      u1 tag;
      u1 reference_kind;
      u2 reference_index;
    } CONSTANT_MethodHandle_info;

    struct
    {
      u1 tag;
      u2 descriptor_index;
    } CONSTANT_MethodType_info;

    struct
    {
      u1 tag;
      u2 bootstrap_method_attr_index;
      u2 name_and_type_index;
    } CONSTANT_InvokeDynamic_info;
  } cp_union;
} cp_info;

// 4.5. Fields

typedef enum field_access_flags
{
  FIELD_ACC_PUBLIC = 1,
  FIELD_ACC_PRIVATE = 2,
  FIELD_ACC_PROTECTED = 4,
  FIELD_ACC_STATIC = 8,
  FIELD_ACC_FINAL = 16,
  FIELD_ACC_VOLATILE = 64,
  FIELD_ACC_TRANSIENT = 128,
  FIELD_ACC_SYNTHETIC = 4096,
  FIELD_ACC_ENUM = 16384
} field_access_flags;

typedef struct field_info
{
  u2 access_flags;
  u2 name_index;
  u2 descriptor_index;
  u2 attributes_count;
  attribute_info **attributes;
} field_info;

// 4.6. Methods

typedef enum method_access_flags
{
  METHOD_ACC_PUBLIC = 1,
  METHOD_ACC_PRIVATE = 2,
  METHOD_ACC_PROTECTED = 4,
  METHOD_ACC_STATIC = 8,
  METHOD_ACC_FINAL = 16,
  METHOD_ACC_SYNCHRONIZED = 32,
  METHOD_ACC_BRIDGE = 64,
  METHOD_ACC_VARARGS = 128,
  METHOD_ACC_NATIVE = 256,
  METHOD_ACC_ABSTRACT = 1024,
  METHOD_ACC_STRICT = 2048,
  METHOD_ACC_SYNTHETIC = 4096,
} method_access_flags;

typedef struct method_info
{
  u2 access_flags;
  u2 name_index;
  u2 descriptor_index;
  u2 attributes_count;
  attribute_info **attributes;
} method_info;

// 4.7.2
typedef struct constant_value_attribute
{
  u2 attribute_name_index;
  u4 attribute_length;
  u2 constant_value_index;
} constant_value_attribute;

// 4.7.3 The Code Atribute
typedef struct code_attribute
{
  u2 attribute_name_index;
  u4 attribute_length;
  u2 max_stack;
  u2 max_locals;
  u4 code_length;

  u1 *code;

  u2 exception_table_length;

  exception_table *exception_table;

  u2 attributes_count;

  attribute_info **attributes;
} code_attribute;

// 4.7.5
typedef struct exception_attribute
{
  u2 attribute_name_index;
  u4 attribute_length;
  u2 number_of_exceptions;
  u2 *exception_index_table;
} exception_attribute;

// 4.7.6
typedef struct classes
{
  u2 inner_class_info_index;
  u2 outer_class_info_index;
  u2 inner_name_index;
  u2 inner_class_access_flags;
} classes;

typedef struct inner_classes_attribute
{
  u2 attribute_name_index;
  u4 attribute_length;
  u2 number_of_classes;
  classes **classes;
} inner_classes_attribute;

// 4.7.8
typedef struct synthetic_attribute
{
  u2 attribute_name_index;
  u4 atribute_length;
} synthetic_attribute;

// // 4.7.9
typedef struct signature_attribute
{
  u2 attribute_name_index;
  u4 attribute_length;
  u2 signature_index;
} signature_attribute;

// // 4.7.10
typedef struct source_file_attribute
{
  u2 attribute_name_index;
  u4 attribute_length;
  u2 sourcefile_index;
} source_file_attribute;

// // 4.7.11
// typedef struct source_debug_extension_attribute {
//   u2 attribute_name_index;
//   u4 attribute_length;
//   u1 *debug_extension;
// } source_debug_extension_attribute;

// // 4.7.12
typedef struct line_number_table
{
  u2 start_pc;
  u2 line_number;
} line_number_table;

typedef struct line_number_table_attribute
{
  u2 attribute_name_index;
  u4 attribute_length;
  u2 line_number_table_length;
  line_number_table *info;

} line_number_table_attribute;


typedef struct verification_type_info
{
    // Byte de tag de tipo de entrada
    u1 tag;

    // Union de verificação de tipos
    union
    {
        struct
        {
          u2 cp;
        } object_variable_info;

        struct
        {
          u2 offset;
        } uninitialized_variable_info;

    } type_info;
} verification_type_info;

/* MISSNG THIS PART */
typedef struct stack_map_frame
{
  u1 frame_type;

    union
    {
        struct
        {
          verification_type_info **stack;
        } same_locals_1_stack_item_frame;

        struct
        {
            u2 offset_delta;
            verification_type_info **stack;
        } same_locals_1_stack_item_frame_extended;

        struct
        {
          u2 offset_delta;
        } chop_frame;

        struct
        {
          u2 offset_delta;
        } same_frame_extended;

        struct
        {
          u2 offset_delta;
          verification_type_info **locals;
        } append_frame;

        struct
        {
          u2 offset_delta;
          u2 num_locals;
          u2 num_stack_items;

          verification_type_info **locals;
          verification_type_info ** stack;
        } full_frame;

    } map_frame_type;
} stack_map_frame;

typedef struct stack_map_table_attribute
{
  u2 attribute_name_index;
  u4 attribute_length;
  u2 number_of_entries;
  stack_map_frame **entries;
} stack_map_table_attribute;

typedef struct ClassFile
{
  u4 magic;
  u2 minor_version;
  u2 major_version;
  u2 constant_pool_count;
  cp_info *constant_pool;
  u2 access_flags;
  u2 this_class;
  u2 super_class;
  u2 interfaces_count;
  u2 *interfaces;
  u2 fields_count;
  field_info *fields;
  u2 methods_count;
  method_info *methods;
  u2 attributes_count;
  attribute_info **attributes;
} ClassFile;

#endif // CLASS_FILE_H
