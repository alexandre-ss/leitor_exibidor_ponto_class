#ifndef LISTA_DE_CLASSES_H
#define LISTA_DE_CLASSES_H

#include "ClassFile.h"
#include "leitor.h"
#include "exibidor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct loaded_class
 * @brief  Estrutuda de classes carregadas
 */
typedef struct loaded_class {
  ClassFile *class_file;      /**< Ponteiro para o ClassFile */
  struct loaded_class *next;  /**< Ponteiro de acesso ao próximo nó da lista */
  struct loaded_class *prev;  /**< Ponteiro de acesso ao nó anterior da lista */
} loaded_class;

/**
 * @brief	  Inicia a lista de classes.
 * @return  Lista de classes com valor NULL
 */
loaded_class  *create_class_list();

/**
 * @brief		              Insere um elemento de classe no fim da lista.
 * @param 		list        Ponteiro para a lista de classes carregada
 * @param 		class_file  Ponteiro para o arquivo de classes
 */
loaded_class  *insert_class_element(loaded_class *, ClassFile *);

/**
 * @brief		        Remove o último elemento de classe da lista.
 * @param 		list  Ponteiro para a lista de classes carregada
 * @return          Lista de classes com o último elemento removido
 */
loaded_class  *remove_last_class_element(loaded_class *);

/**
 * @brief		        Remove um elemento de classe da lista.
 * @param 		list  Ponteiro para a lista de classes carregada
 * @param     word  Nome da classe que será buscada para ser removida da lista
 * @return          Lista de classes com o elemento de classe removido
 */
loaded_class  *remove_class_element(loaded_class *, char *);

/**
 * @brief		        Busca um elemento de classe na lista.
 * @param 		list  Ponteiro para a lista de classes carregada
 * @param     word  Nome da classe que será buscada 
 * @return          Ponteiro para o elemento encontrado ou NULL caso não encontre-o
 */
loaded_class  *find_class_element(loaded_class *, char *);

/**
 * @brief		        Busca a pool de constante da classe atual.
 * @param 		list  Ponteiro para a lista de classes carregada
 * @param     word  Nome da classe do qual deverá ser retornada a pool de constantes
 * @return          Ponteiro para a pool de constantes
 */
cp_info       *find_class_constant_pool(loaded_class *, char *);

/**
 * @brief		              Busca um método da classe atual.
 * @param 		list        Ponteiro para a lista de classes carregada
 * @param     name        Nome da classe no qual deverá ser buscado o método
 * @param     method_name Nome do método a ser buscado 
 * @return                Estrutura method info do método buscado ou NULL caso não o encontre
 */
method_info   *find_class_method(loaded_class *, char *, char *);

/**
 * @brief		              Busca um campo da classe atual.
 * @param 		list        Ponteiro para a lista de classes carregada
 * @param     class_name  Nome da classe no qual deverá ser buscado o campo
 * @param     field_name  Nome do campo o qual deverá ser buscado
 * @return                Estrutura field que foi buscada
 */
field_info    *find_class_field(loaded_class *, char *, char *);

/**
 * @brief 	  	Imprime na tela a lista de classes.
 * @param list 	Ponteiro para a lista de classes a ser impressa
 */
void print_class_list(loaded_class *);

/**
 * @brief		  Libera a memória utilizada pela lista de classes.
 * @param list 	Ponteiro para a lista de classes a ser liberada
 */
void free_class_list(loaded_class *);


#endif