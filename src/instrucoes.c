#include "instrucoes.h"

const int count_enum = jsr_w-nop;             ////////// REVISAR. Por que -nop?

instruction *build_instructions(void) {
    instruction *instructions = (instruction *)malloc(count_enum * sizeof(struct instruction));

	for (int i = 0; i < count_enum; i++)
	{
		switch(i){
			case nop:
				strcpy(instructions[i].inst_name, "nop");
				instructions[i].opcode = nop;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case aconst_null:
				strcpy(instructions[i].inst_name, "aconst_null");
				instructions[i].opcode = aconst_null;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case iconst_m1:
				strcpy(instructions[i].inst_name, "iconst_m1");
				instructions[i].opcode = iconst_m1;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case iconst_0:
				strcpy(instructions[i].inst_name, "iconst_0");
				instructions[i].opcode = iconst_0;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case iconst_1:
				strcpy(instructions[i].inst_name, "iconst_1");
				instructions[i].opcode = iconst_1;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case iconst_2:
				strcpy(instructions[i].inst_name, "iconst_2");
				instructions[i].opcode = iconst_2;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case iconst_3:
				strcpy(instructions[i].inst_name, "iconst_3");
				instructions[i].opcode = iconst_3;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case iconst_4:
				strcpy(instructions[i].inst_name, "iconst_4");
				instructions[i].opcode = iconst_4;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case iconst_5:
				strcpy(instructions[i].inst_name, "iconst_5");
				instructions[i].opcode = iconst_5;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case lconst_0:
				strcpy(instructions[i].inst_name, "lconst_0");
				instructions[i].opcode = lconst_0;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case lconst_1:
				strcpy(instructions[i].inst_name, "lconst_1");
				instructions[i].opcode = lconst_1;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case fconst_0:
				strcpy(instructions[i].inst_name, "fconst_0");
				instructions[i].opcode = fconst_0;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case fconst_1:
				strcpy(instructions[i].inst_name, "fconst_1");
				instructions[i].opcode = fconst_1;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case fconst_2:
				strcpy(instructions[i].inst_name, "fconst_2");
				instructions[i].opcode = fconst_2;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case dconst_0:
				strcpy(instructions[i].inst_name, "dconst_0");
				instructions[i].opcode = dconst_0;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case dconst_1:
				strcpy(instructions[i].inst_name, "dconst_1");
				instructions[i].opcode = dconst_1;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case bipush:
				strcpy(instructions[i].inst_name, "bipush");
				instructions[i].opcode = bipush;
				instructions[i].arg_count = 1;
				instructions[i].op_count = 0;
			break;
			case sipush:
				strcpy(instructions[i].inst_name, "sipush");
				instructions[i].opcode = sipush;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 0;
			break;
			case ldc:
				strcpy(instructions[i].inst_name, "ldc");
				instructions[i].opcode = ldc;
				instructions[i].arg_count = 1;
				instructions[i].op_count = 0;
			break;
			case ldc_w:
				strcpy(instructions[i].inst_name, "ldc_w");
				instructions[i].opcode = ldc_w;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 0;
			break;
			case ldc2_w:
				strcpy(instructions[i].inst_name, "ldc2_w");
				instructions[i].opcode = ldc2_w;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 0;
			break;
			case iload:
				strcpy(instructions[i].inst_name, "iload");
				instructions[i].opcode = iload;
				instructions[i].arg_count = 1;
				instructions[i].op_count = 0;
			break;
			case lload:
				strcpy(instructions[i].inst_name, "lload");
				instructions[i].opcode = lload;
				instructions[i].arg_count = 1;
				instructions[i].op_count = 0;
			break;
			case fload:
				strcpy(instructions[i].inst_name, "fload");
				instructions[i].opcode = fload;
				instructions[i].arg_count = 1;
				instructions[i].op_count = 0;
			break;
			case dload:
				strcpy(instructions[i].inst_name, "dload");
				instructions[i].opcode = dload;
				instructions[i].arg_count = 1;
				instructions[i].op_count = 0;
			break;
			case aload:
				strcpy(instructions[i].inst_name, "aload");
				instructions[i].opcode = aload;
				instructions[i].arg_count = 1;
				instructions[i].op_count = 0;
			break;
			case iload_0:
				strcpy(instructions[i].inst_name, "iload_0");
				instructions[i].opcode = iload_0;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case iload_1:
				strcpy(instructions[i].inst_name, "iload_1");
				instructions[i].opcode = iload_1;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case iload_2:
				strcpy(instructions[i].inst_name, "iload_2");
				instructions[i].opcode = iload_2;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case iload_3:
				strcpy(instructions[i].inst_name, "iload_3");
				instructions[i].opcode = iload_3;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case lload_0:
				strcpy(instructions[i].inst_name, "lload_0");
				instructions[i].opcode = lload_0;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case lload_1:
				strcpy(instructions[i].inst_name, "lload_1");
				instructions[i].opcode = lload_1;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case lload_2:
				strcpy(instructions[i].inst_name, "lload_2");
				instructions[i].opcode = lload_2;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case lload_3:
				strcpy(instructions[i].inst_name, "lload_3");
				instructions[i].opcode = lload_3;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case fload_0:
				strcpy(instructions[i].inst_name, "fload_0");
				instructions[i].opcode = fload_0;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case fload_1:
				strcpy(instructions[i].inst_name, "fload_1");
				instructions[i].opcode = fload_1;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case fload_2:
				strcpy(instructions[i].inst_name, "fload_2");
				instructions[i].opcode = fload_2;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case fload_3:
				strcpy(instructions[i].inst_name, "fload_3");
				instructions[i].opcode = fload_3;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case dload_0:
				strcpy(instructions[i].inst_name, "dload_0");
				instructions[i].opcode = dload_0;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case dload_1:
				strcpy(instructions[i].inst_name, "dload_1");
				instructions[i].opcode = dload_1;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case dload_2:
				strcpy(instructions[i].inst_name, "dload_2");
				instructions[i].opcode = dload_2;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case dload_3:
				strcpy(instructions[i].inst_name, "dload_3");
				instructions[i].opcode = dload_3;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case aload_0:
				strcpy(instructions[i].inst_name, "aload_0");
				instructions[i].opcode = aload_0;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case aload_1:
				strcpy(instructions[i].inst_name, "aload_1");
				instructions[i].opcode = aload_1;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case aload_2:
				strcpy(instructions[i].inst_name, "aload_2");
				instructions[i].opcode = aload_2;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case aload_3:
				strcpy(instructions[i].inst_name, "aload_3");
				instructions[i].opcode = aload_3;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case iaload:
				strcpy(instructions[i].inst_name, "iaload");
				instructions[i].opcode = iaload;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case laload:
				strcpy(instructions[i].inst_name, "laload");
				instructions[i].opcode = laload;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case faload:
				strcpy(instructions[i].inst_name, "faload");
				instructions[i].opcode = faload;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case daload:
				strcpy(instructions[i].inst_name, "daload");
				instructions[i].opcode = daload;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case aaload:
				strcpy(instructions[i].inst_name, "aaload");
				instructions[i].opcode = aaload;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case baload:
				strcpy(instructions[i].inst_name, "baload");
				instructions[i].opcode = baload;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case caload:
				strcpy(instructions[i].inst_name, "caload");
				instructions[i].opcode = caload;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case saload:
				strcpy(instructions[i].inst_name, "saload");
				instructions[i].opcode = saload;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case istore:
				strcpy(instructions[i].inst_name, "istore");
				instructions[i].opcode = istore;
				instructions[i].arg_count = 1;
				instructions[i].op_count = 1;
			break;
			case lstore:
				strcpy(instructions[i].inst_name, "lstore");
				instructions[i].opcode = lstore;
				instructions[i].arg_count = 1;
				instructions[i].op_count = 1;
			break;
			case fstore:
				strcpy(instructions[i].inst_name, "fstore");
				instructions[i].opcode = fstore;
				instructions[i].arg_count = 1;
				instructions[i].op_count = 1;
			break;
			case dstore:
				strcpy(instructions[i].inst_name, "dstore");
				instructions[i].opcode = dstore;
				instructions[i].arg_count = 1;
				instructions[i].op_count = 1;
			break;
			case astore:
				strcpy(instructions[i].inst_name, "astore");
				instructions[i].opcode = astore;
				instructions[i].arg_count = 1;
				instructions[i].op_count = 1;
			break;
			case istore_0:
				strcpy(instructions[i].inst_name, "istore_0");
				instructions[i].opcode = istore_0;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case istore_1:
				strcpy(instructions[i].inst_name, "istore_1");
				instructions[i].opcode = istore_1;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case istore_2:
				strcpy(instructions[i].inst_name, "istore_2");
				instructions[i].opcode = istore_2;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case istore_3:
				strcpy(instructions[i].inst_name, "istore_3");
				instructions[i].opcode = istore_3;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case lstore_0:
				strcpy(instructions[i].inst_name, "lstore_0");
				instructions[i].opcode = lstore_0;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case lstore_1:
				strcpy(instructions[i].inst_name, "lstore_1");
				instructions[i].opcode = lstore_1;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case lstore_2:
				strcpy(instructions[i].inst_name, "lstore_2");
				instructions[i].opcode = lstore_2;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case lstore_3:
				strcpy(instructions[i].inst_name, "lstore_3");
				instructions[i].opcode = lstore_3;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case fstore_0:
				strcpy(instructions[i].inst_name, "fstore_0");
				instructions[i].opcode = fstore_0;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case fstore_1:
				strcpy(instructions[i].inst_name, "fstore_1");
				instructions[i].opcode = fstore_1;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case fstore_2:
				strcpy(instructions[i].inst_name, "fstore_2");
				instructions[i].opcode = fstore_2;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case fstore_3:
				strcpy(instructions[i].inst_name, "fstore_3");
				instructions[i].opcode = fstore_3;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case dstore_0:
				strcpy(instructions[i].inst_name, "dstore_0");
				instructions[i].opcode = dstore_0;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case dstore_1:
				strcpy(instructions[i].inst_name, "dstore_1");
				instructions[i].opcode = dstore_1;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case dstore_2:
				strcpy(instructions[i].inst_name, "dstore_2");
				instructions[i].opcode = dstore_2;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case dstore_3:
				strcpy(instructions[i].inst_name, "dstore_3");
				instructions[i].opcode = dstore_3;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case astore_0:
				strcpy(instructions[i].inst_name, "astore_0");
				instructions[i].opcode = astore_0;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case astore_1:
				strcpy(instructions[i].inst_name, "astore_1");
				instructions[i].opcode = astore_1;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case astore_2:
				strcpy(instructions[i].inst_name, "astore_2");
				instructions[i].opcode = astore_2;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case astore_3:
				strcpy(instructions[i].inst_name, "astore_3");
				instructions[i].opcode = astore_3;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case iastore:
				strcpy(instructions[i].inst_name, "iastore");
				instructions[i].opcode = iastore;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 3;
			break;
			case lastore:
				strcpy(instructions[i].inst_name, "lastore");
				instructions[i].opcode = lastore;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 3;
			break;
			case fastore:
				strcpy(instructions[i].inst_name, "fastore");
				instructions[i].opcode = fastore;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 3;
			break;
			case dastore:
				strcpy(instructions[i].inst_name, "dastore");
				instructions[i].opcode = dastore;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 3;
			break;
			case aastore:
				strcpy(instructions[i].inst_name, "aastore");
				instructions[i].opcode = aastore;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 3;
			break;
			case bastore:
				strcpy(instructions[i].inst_name, "bastore");
				instructions[i].opcode = bastore;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 3;
			break;
			case castore:
				strcpy(instructions[i].inst_name, "castore");
				instructions[i].opcode = castore;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 3;
			break;
			case sastore:
				strcpy(instructions[i].inst_name, "sastore");
				instructions[i].opcode = sastore;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 3;
			break;
			case pop:
				strcpy(instructions[i].inst_name, "pop");
				instructions[i].opcode = pop;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;

			break;
			case pop2:
				strcpy(instructions[i].inst_name, "pop2");
				instructions[i].opcode = pop2;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case dup:
				strcpy(instructions[i].inst_name, "dup");
				instructions[i].opcode = dup;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case dup_x1:
				strcpy(instructions[i].inst_name, "dup_x1");
				instructions[i].opcode = dup_x1;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case dup_x2:
				strcpy(instructions[i].inst_name, "dup_x2");
				instructions[i].opcode = dup_x2;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 3;
			break;
			case dup2:
				strcpy(instructions[i].inst_name, "dup2");
				instructions[i].opcode = dup2;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case dup2_x1:
				strcpy(instructions[i].inst_name, "dup2_x1");
				instructions[i].opcode = dup2_x1;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 3;
			break;
			case dup2_x2:
				strcpy(instructions[i].inst_name, "dup2_x2");
				instructions[i].opcode = dup2_x2;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 4;
			break;
			case swap:
				strcpy(instructions[i].inst_name, "swap");
				instructions[i].opcode = swap;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case iadd:
				strcpy(instructions[i].inst_name, "iadd");
				instructions[i].opcode = iadd;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case ladd:
				strcpy(instructions[i].inst_name, "ladd");
				instructions[i].opcode = ladd;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case fadd:
				strcpy(instructions[i].inst_name, "fadd");
				instructions[i].opcode = fadd;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case dadd:
				strcpy(instructions[i].inst_name, "dadd");
				instructions[i].opcode = dadd;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case isub:
				strcpy(instructions[i].inst_name, "isub");
				instructions[i].opcode = isub;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case lsub:
				strcpy(instructions[i].inst_name, "lsub");
				instructions[i].opcode = lsub;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case fsub:
				strcpy(instructions[i].inst_name, "fsub");
				instructions[i].opcode = fsub;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case dsub:
				strcpy(instructions[i].inst_name, "dsub");
				instructions[i].opcode = dsub;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case imul:
				strcpy(instructions[i].inst_name, "imul");
				instructions[i].opcode = imul;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case lmul:
				strcpy(instructions[i].inst_name, "lmul");
				instructions[i].opcode = lmul;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case fmul:
				strcpy(instructions[i].inst_name, "fmul");
				instructions[i].opcode = fmul;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case dmul:
				strcpy(instructions[i].inst_name, "dmul");
				instructions[i].opcode = dmul;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case idiv:
				strcpy(instructions[i].inst_name, "idiv");
				instructions[i].opcode = idiv;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case inst_ldiv:
				strcpy(instructions[i].inst_name, "ldiv");
				instructions[i].opcode = inst_ldiv;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case fdiv:
				strcpy(instructions[i].inst_name, "fdiv");
				instructions[i].opcode = fdiv;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case ddiv:
				strcpy(instructions[i].inst_name, "ddiv");
				instructions[i].opcode = ddiv;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case irem:
				strcpy(instructions[i].inst_name, "irem");
				instructions[i].opcode = irem;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case lrem:
				strcpy(instructions[i].inst_name, "lrem");
				instructions[i].opcode = lrem;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case frem:
				strcpy(instructions[i].inst_name, "frem");
				instructions[i].opcode = frem;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case inst_drem:
				strcpy(instructions[i].inst_name, "drem");
				instructions[i].opcode = inst_drem;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case ineg:
				strcpy(instructions[i].inst_name, "ineg");
				instructions[i].opcode = ineg;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case lneg:
				strcpy(instructions[i].inst_name, "lneg");
				instructions[i].opcode = lneg;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case fneg:
				strcpy(instructions[i].inst_name, "fneg");
				instructions[i].opcode = fneg;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case dneg:
				strcpy(instructions[i].inst_name, "dneg");
				instructions[i].opcode = dneg;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case ishl:
				strcpy(instructions[i].inst_name, "ishl");
				instructions[i].opcode = ishl;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case lshl:
				strcpy(instructions[i].inst_name, "lshl");
				instructions[i].opcode = lshl;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case ishr:
				strcpy(instructions[i].inst_name, "ishr");
				instructions[i].opcode = ishr;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case lshr:
				strcpy(instructions[i].inst_name, "lshr");
				instructions[i].opcode = lshr;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case iushr:
				strcpy(instructions[i].inst_name, "iushr");
				instructions[i].opcode = iushr;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case lushr:
				strcpy(instructions[i].inst_name, "lushr");
				instructions[i].opcode = lushr;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case iand:
				strcpy(instructions[i].inst_name, "iand");
				instructions[i].opcode = iand;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case land:
				strcpy(instructions[i].inst_name, "land");
				instructions[i].opcode = land;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case ior:
				strcpy(instructions[i].inst_name, "ior");
				instructions[i].opcode = ior;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case lor:
				strcpy(instructions[i].inst_name, "lor");
				instructions[i].opcode = lor;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case ixor:
				strcpy(instructions[i].inst_name, "ixor");
				instructions[i].opcode = ixor;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case lxor:
				strcpy(instructions[i].inst_name, "lxor");
				instructions[i].opcode = lxor;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case iinc:
				strcpy(instructions[i].inst_name, "iinc");
				instructions[i].opcode = iinc;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 0;
			break;
			case i2l:
				strcpy(instructions[i].inst_name, "i2l");
				instructions[i].opcode = i2l;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case i2f:
				strcpy(instructions[i].inst_name, "i2f");
				instructions[i].opcode = i2f;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case i2d:
				strcpy(instructions[i].inst_name, "i2d");
				instructions[i].opcode = i2d;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case l2i:
				strcpy(instructions[i].inst_name, "l2i");
				instructions[i].opcode = l2i;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case l2f:
				strcpy(instructions[i].inst_name, "l2f");
				instructions[i].opcode = l2f;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case l2d:
				strcpy(instructions[i].inst_name, "l2d");
				instructions[i].opcode = l2d;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case f2i:
				strcpy(instructions[i].inst_name, "f2i");
				instructions[i].opcode = f2i;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case f2l:
				strcpy(instructions[i].inst_name, "f2l");
				instructions[i].opcode = f2l;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case f2d:
				strcpy(instructions[i].inst_name, "f2d");
				instructions[i].opcode = f2d;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case d2i:
				strcpy(instructions[i].inst_name, "d2i");
				instructions[i].opcode = d2i;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case d2l:
				strcpy(instructions[i].inst_name, "d2l");
				instructions[i].opcode = d2l;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case d2f:
				strcpy(instructions[i].inst_name, "d2f");
				instructions[i].opcode = d2f;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case i2b:
				strcpy(instructions[i].inst_name, "i2b");
				instructions[i].opcode = i2b;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case i2c:
				strcpy(instructions[i].inst_name, "i2c");
				instructions[i].opcode = i2c;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case i2s:
				strcpy(instructions[i].inst_name, "i2s");
				instructions[i].opcode = i2s;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case lcmp:
				strcpy(instructions[i].inst_name, "lcmp");
				instructions[i].opcode = lcmp;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case fcmpl:
				strcpy(instructions[i].inst_name, "fcmpl");
				instructions[i].opcode = fcmpl;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case fcmpg:
				strcpy(instructions[i].inst_name, "fcmpg");
				instructions[i].opcode = fcmpg;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case dcmpl:
				strcpy(instructions[i].inst_name, "dcmpl");
				instructions[i].opcode = dcmpl;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case dcmpg:
				strcpy(instructions[i].inst_name, "dcmpg");
				instructions[i].opcode = dcmpg;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 2;
			break;
			case ifeq:
				strcpy(instructions[i].inst_name, "ifeq");
				instructions[i].opcode = ifeq;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 1;
			break;
			case ifne:
				strcpy(instructions[i].inst_name, "ifne");
				instructions[i].opcode = ifne;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 1;
			break;
			case iflt:
				strcpy(instructions[i].inst_name, "iflt");
				instructions[i].opcode = iflt;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 1;
			break;
			case ifge:
				strcpy(instructions[i].inst_name, "ifge");
				instructions[i].opcode = ifge;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 1;
			break;
			case ifgt:
				strcpy(instructions[i].inst_name, "ifgt");
				instructions[i].opcode = ifgt;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 1;
			break;
			case ifle:
				strcpy(instructions[i].inst_name, "ifle");
				instructions[i].opcode = ifle;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 1;
			break;
			case if_icmpeq:
				strcpy(instructions[i].inst_name, "if_icmpeq");
				instructions[i].opcode = if_icmpeq;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 2;
			break;
			case if_icmpne:
				strcpy(instructions[i].inst_name, "if_icmpne");
				instructions[i].opcode = if_icmpne;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 2;
			break;
			case if_icmplt:
				strcpy(instructions[i].inst_name, "if_icmplt");
				instructions[i].opcode = if_icmplt;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 2;
			break;
			case if_icmpge:
				strcpy(instructions[i].inst_name, "if_icmpge");
				instructions[i].opcode = if_icmpge;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 2;
			break;
			case if_icmpgt:
				strcpy(instructions[i].inst_name, "if_icmpgt");
				instructions[i].opcode = if_icmpgt;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 2;
			break;
			case if_icmple:
				strcpy(instructions[i].inst_name, "if_icmple");
				instructions[i].opcode = if_icmple;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 2;
			break;
			case if_acmpeq:
				strcpy(instructions[i].inst_name, "if_acmpeq");
				instructions[i].opcode = if_acmpeq;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 2;
			break;
			case if_acmpne:
				strcpy(instructions[i].inst_name, "if_acmpne");
				instructions[i].opcode = if_acmpne;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 2;
			break;
			case inst_goto:
				strcpy(instructions[i].inst_name, "goto");
				instructions[i].opcode = inst_goto;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 0;
			break;
			case jsr:
				strcpy(instructions[i].inst_name, "jsr");
				instructions[i].opcode = jsr;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 0;
			break;
			case ret:
				strcpy(instructions[i].inst_name, "ret");
				instructions[i].opcode = ret;
				instructions[i].arg_count = 1;
				instructions[i].op_count = 0;
			break;
			case tableswitch:
				strcpy(instructions[i].inst_name, "tableswitch");
				instructions[i].opcode = tableswitch;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case lookupswitch:
				strcpy(instructions[i].inst_name, "lookupswitch");
				instructions[i].opcode = lookupswitch;
				instructions[i].arg_count = 1;
				instructions[i].op_count = 1;
			break;
			case ireturn:
				strcpy(instructions[i].inst_name, "ireturn");
				instructions[i].opcode = ireturn;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case lreturn:
				strcpy(instructions[i].inst_name, "lreturn");
				instructions[i].opcode = lreturn;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case freturn:
				strcpy(instructions[i].inst_name, "freturn");
				instructions[i].opcode = freturn;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case dreturn:
				strcpy(instructions[i].inst_name, "dreturn");
				instructions[i].opcode = dreturn;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case areturn:
				strcpy(instructions[i].inst_name, "areturn");
				instructions[i].opcode = areturn;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case inst_return:
				strcpy(instructions[i].inst_name, "return");
				instructions[i].opcode = inst_return;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case getstatic:
				strcpy(instructions[i].inst_name, "getstatic");
				instructions[i].opcode = getstatic;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 0;
			break;
			case putstatic:
				strcpy(instructions[i].inst_name, "putstatic");
				instructions[i].opcode = putstatic;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 1;
			break;
			case getfield:
				strcpy(instructions[i].inst_name, "getfield");
				instructions[i].opcode = getfield;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 1;
			break;
			case putfield:
				strcpy(instructions[i].inst_name, "putfield");
				instructions[i].opcode = putfield;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 2;
			break;
			case invokevirtual:
				strcpy(instructions[i].inst_name, "invokevirtual");
				instructions[i].opcode = invokevirtual;
				instructions[i].arg_count = 2;
				instructions[i].op_count = -1;
			break;
			case invokespecial:
				strcpy(instructions[i].inst_name, "invokespecial");
				instructions[i].opcode = invokespecial;
				instructions[i].arg_count = 2;
				instructions[i].op_count = -1;
			break;
			case invokestatic:
				strcpy(instructions[i].inst_name, "invokestatic");
				instructions[i].opcode = invokestatic;
				instructions[i].arg_count = 2;
				instructions[i].op_count = -1;
			break;
			case invokeinterface:
				strcpy(instructions[i].inst_name, "invokeinterface");
				instructions[i].opcode = invokeinterface;
				instructions[i].arg_count = 4;
				instructions[i].op_count = -1;
			break;
			case inst_new:
				strcpy(instructions[i].inst_name, "new");
				instructions[i].opcode = inst_new;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 0;
			break;
			case newarray:
				strcpy(instructions[i].inst_name, "newarray");
				instructions[i].opcode = newarray;
				instructions[i].arg_count = 1;
				instructions[i].op_count = 1;
			break;
			case anewarray:
				strcpy(instructions[i].inst_name, "anewarray");
				instructions[i].opcode = anewarray;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 1;
			break;
			case arraylength:
				strcpy(instructions[i].inst_name, "arraylength");
				instructions[i].opcode = arraylength;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case athrow:
				strcpy(instructions[i].inst_name, "athrow");
				instructions[i].opcode = athrow;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case checkcast:
				strcpy(instructions[i].inst_name, "checkcast");
				instructions[i].opcode = checkcast;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 1;
			break;
			case instanceof:
				strcpy(instructions[i].inst_name, "instanceof");
				instructions[i].opcode = instanceof;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 1;
			break;
			case monitorenter:
				strcpy(instructions[i].inst_name, "monitorenter");
				instructions[i].opcode = monitorenter;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case monitorexit:
				strcpy(instructions[i].inst_name, "monitorexit");
				instructions[i].opcode = monitorexit;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 1;
			break;
			case wide:
				strcpy(instructions[i].inst_name, "wide");
				instructions[i].opcode = wide;
				instructions[i].arg_count = 0;
				instructions[i].op_count = 0;
			break;
			case multianewarray:
				strcpy(instructions[i].inst_name, "multianewarray");
				instructions[i].opcode = multianewarray;
				instructions[i].arg_count = 3;
				instructions[i].op_count = -1;
			break;
			case ifnull:
				strcpy(instructions[i].inst_name, "ifnull");
				instructions[i].opcode = ifnull;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 1;
			break;
			case ifnonnull:
				strcpy(instructions[i].inst_name, "ifnonnull");
				instructions[i].opcode = ifnonnull;
				instructions[i].arg_count = 2;
				instructions[i].op_count = 1;
			break;
			case goto_w:
				strcpy(instructions[i].inst_name, "goto_w");
				instructions[i].opcode = goto_w;
				instructions[i].arg_count = 4;
				instructions[i].op_count = 0;
			break;
			case jsr_w:
				strcpy(instructions[i].inst_name, "jsr_w");
				instructions[i].opcode = jsr_w;
				instructions[i].arg_count = 4;
				instructions[i].op_count = 0;
			break;

			default:
				strcpy(instructions[i].inst_name, "undefined");
				instructions[i].opcode = 255;
				instructions[i].arg_count = -1;
				instructions[i].op_count = -1;
			break;
		}

		instructions[i].pc_instrucao = instructions[i].arg_count + 1;
	}

	return(instructions);
}
