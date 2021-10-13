/*
  DISCIPLINA: Software Básico - 2021/1
  ALUNOS:
    - 18/0011847 - Alexandre Santana Sousa
    - 18/0112317 - Conrado Nunes Barbosa Neto
    - 18/0033816 - João Victor Novais Magalhães
    - 18/0053485 - Kallebe de Sousa Silva
 */

#include "leitor.h"
#include "exibidor.h"
#include "jvm.h"

int main(int argc, char *argv[]) {
  char *filename;
  int opcao = 0;

  if (argc > 1) {
    filename = (char *) malloc(strlen(argv[1]) * sizeof(char));
    filename = argv[1];
  }

  ClassFile *cf = read_file(filename);
  initialize();
  jvm->classes = insert_class_element(jvm->classes, cf);

  if (cf != NULL) {
    // Opcoes para leitura do .class e/ou execucao da JVM
    while (opcao < 1 || opcao > 3) {
      printf("Escolha uma das opcoes:\n");
      printf("\t1. Leitura do .class\n");
      printf("\t2. Execucao da JVM\n");
      printf("\t3. Execucao da JVM com opcodes\n");
      printf("Escolha sua opcao: ");
      scanf("%d", &opcao);
    }

    switch (opcao) {
      case 1:
        printf("\n--------------------| Realizando a leitura do .class\n\n");
        print_class_file(cf);
        break;
      case 2:
        printf("\n--------------------| Executando a JVM\n\n");
        run_jvm();
        break;
      case 3:
        printf("\n--------------------| Executando a JVM com opcodes\n\n");
        SHOW_OPCODES = true;
        run_jvm();
        break;
      default: break;
    }

    free(cf);
    free(filename);
  } else {
    printf("Magic Number inválido\n");
  }

  free(jvm);

  return 0;
}
