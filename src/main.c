/*
  DISCIPLINA: Software Básico - 2021/1
  ALUNOS:
    - 18/0011847 - Alexandre Santana Sousa
    - 18/0033816 - João Victor Novais Magalhães
    - 18/0053485 - Kallebe de Sousa Silva
    - 18/0112317 - Conrado Nunes Barbosa Neto
 */

#include "leitor.h"
#include "exibidor.h"

int main(int argc, char *argv[]) {
  char *filename;
  int opcao = 0;

  if (argc > 1) {
    filename = (char *) malloc(strlen(argv[1]) * sizeof(char));
    filename = argv[1];
  }

  ClassFile *cf = read_file(filename);

  if (cf != NULL) {
    // Opcoes para leitura do .class e/ou execucao da JVM
    while (opcao < 1 || opcao > 3) {
      printf("Escolha uma das opcoes:\n");
      printf("\t1. Leitura do .class\n");
      printf("\t2. Execucao da JVM\n");
      printf("\t3. Leitura do .class e Execucao da JVM\n");
      scanf("Sua escolha: %d", &opcao);
    }

    switch (opcao) {
      case 1:
        printf("\n--------| Realizando a leitura do .class\n\n");
        print_class_file(cf);
        break;
      case 2:
        printf("\n--------| Exacutando a JVM\n\n");
        run_jvm();
        break;
      case 3:
        printf("\n--------| Realizando a leitura do .class\n\n");
        print_class_file(cf);

        printf("\n--------| Exacutando a JVM\n\n");
        run_jvm();
        break;
      default: break;
    }

    free(cf);
  } else {
    printf("Magic Number inválido\n");
  }

  free(filename);
  return 0;
}
