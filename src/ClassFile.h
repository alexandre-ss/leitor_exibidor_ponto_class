#ifndef CLASS_FILE_H
#define CLASS_FILE_H

#include <stdint.h>
#include <stdio.h>
#define NAME_INDEX 1
#define NAME_AND_TYPE 2
#define STRING_INDEX 3
#define CLASS_INDEX 4
#define NAME_AND_TYPE_INFO_NAME_INDEX 5
#define NAME_AND_TYPE_INFO_DESCRIPTOR_INDEX 6

/** Definição dos tipos de inteiro sem sinal */
#ifndef UNSIGNED_TYPES
#define UNSIGNED_TYPES
typedef uint8_t u1;   /**< Tipo \c unsigned \c int de 8 bits */
typedef uint16_t u2;  /**< Tipo \c unsigned \c int de 16 bits */
typedef uint32_t u4;  /**< Tipo \c unsigned \c int de 32 bits */
typedef uint64_t u8;  /**< Tipo \c unsigned \c int de 64 bits */
#endif

/** Definição dos tipos de inteiro com sinal */
#ifndef SIGNED_TYPES
#define SIGNED_TYPES
typedef int8_t i1;   /**< Tipo \c int com sinal, de 8 bits */
typedef int16_t i2;  /**< Tipo \c int com sinal, de 16 bits */
typedef int32_t i4;  /**< Tipo \c int com sinal, de 32 bits */
typedef int64_t i8;  /**< Tipo \c int com sinal, de 64 bits */
#endif

/** */
#ifndef FLOAT_DEF
#define FLOAT_DEF
#define exponent(x) ((x << 1) >> 24)
#define mantissa(x) ((x << 9) >> 9)
#define signal(x) (x >> 31)
#endif

#ifndef DOUBLE_DEF
#define DOUBLE_DEF
#define exponent_d(x) ((x << 1) >> 53)
#define mantissa_d(x) ((x << 12) >> 12)
#define signal_d(x) (x >> 63)
#endif

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

// Enum para tipo de array
typedef enum array_type
{
  T_BOOLEAN = 4,
  T_CHAR,
	T_FLOAT,
	T_DOUBLE,
	T_BYTE,
	T_SHORT,
	T_INT,
	T_LONG
} array_type;

// 4.1

typedef enum access_flags
{
  ACC_PUBLIC      = 1,
  ACC_PRIVATE     = 2,
  ACC_PROTECTED   = 4,
  ACC_STATIC      = 8,
  ACC_FINAL       = 16,
  ACC_SUPER       = 32,
  ACC_VOLATILE    = 64,
  ACC_TRANSIENT   = 128, 
  ACC_INTERFACE   = 512,
  ACC_ABSTRACT    = 1024,
  ACC_SYNTHETIC   = 4096,
  ACC_ANNOTATION  = 8192,
  ACC_ENUM        = 16384
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

typedef struct static_data
{
  u4 *low;
  u4 *high;
  u1 *string;
} static_data;

typedef struct field_info
{
  u2 access_flags;
  u2 name_index;
  u2 descriptor_index;
  u2 attributes_count;
  attribute_info **attributes;
  static_data *static_data;
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

typedef struct local_variable
{
  u4 *variable;
  u1 type;
} local_variable;

typedef struct stack_map_table_attribute
{
  u2 attribute_name_index;
  u4 attribute_length;
  u2 number_of_entries;
  stack_map_frame **entries;
} stack_map_table_attribute;

typedef enum operand_types {
	BOOLEAN_OP = 1,
	BYTE_OP,
	CHAR_OP,
	SHORT_OP,
	INTEGER_OP,
	FLOAT_OP,
	LONG_OP,
	DOUBLE_OP,
	RETURN_ADDRESS_OP,
	REFERENCE_OP,
	REFERENCE_ARRAY_BOOLEAN_OP,
	REFERENCE_ARRAY_CHAR_OP,
	REFERENCE_ARRAY_FLOAT_OP,
	REFERENCE_ARRAY_DOUBLE_OP,
	REFERENCE_ARRAY_BYTE_OP,
	REFERENCE_ARRAY_SHORT_OP,
	REFERENCE_ARRAY_INT_OP,
	REFERENCE_ARRAY_LONG_OP,
	REFERENCE_STRING_OP,
} operand_types;

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

/**
 * @struct operand_stack
 * @brief Estrutura de dados da pilha de operandos.
 * @see operand_list
 */
typedef struct operand_stack {
	struct operand_list *top; /**< Ponteiro para o topo da pilha de operandos */
} operand_stack;

/**
 * @struct  object_list
 * @brief   Lista de objetos
 */
typedef struct object_list {
    ClassFile *obj;
    u1 size_data;
    u4 *data;
    struct object_list *prox;
    struct object_list *ant;
} object_list;

typedef struct frame
{
  u4 return_address;
  operand_stack *p;
  local_variable *v;
  u2 vetor_length;
  cp_info *cp;
  char *current_class;
} frame;

#endif // CLASS_FILE_H
