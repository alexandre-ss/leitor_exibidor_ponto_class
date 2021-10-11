/*
  DISCIPLINA: Software Básico - 2021/1
  ALUNOS:
    - 18/0011847 - Alexandre Santana Sousa
    - 18/0033816 - João Victor Novais Magalhães
    - 18/0053485 - Kallebe de Sousa Silva
    -
 */

#include "leitor.h"
#include "exibidor.h"

int main(int argc, char *argv[])
{
  char *filename;

  if (argc > 1)
  {
    filename = (char *)malloc(strlen(argv[1]) * sizeof(char));
    filename = argv[1];
  }

  ClassFile *cf = read_file(filename);

  if (cf != NULL)
  {
    // Oferece opção para o usuário de utilizar apenas o exibidor ou apenas a jvm

    // JVM
    run_jvm();




    // Exibidor
    print_class_file(cf);
    free(cf);
  }
  else{
    printf("Magic Number inválido\n");
  }

  return 0;
}
