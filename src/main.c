/*
  DISCIPLINA: Software Básico - 2021/1
  ALUNOS:
    - 18/0011847 - Alexandre Santana Sousa
    - 18/0033816 - João Victor Novais Magalhães
    - 18/0053485 - Kallebe de Sousa Silva
 */
#include "leitor.h"

int main(int argc, char *argv[]) {
  char *filename;

  if (argc > 1)
  {
      filename = (char *)malloc(strlen(argv[1]) * sizeof(char));
      strcpy(filename, argv[1]);
  }

  read_file(filename);

  return 0;
}
