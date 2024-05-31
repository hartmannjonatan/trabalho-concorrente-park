#ifndef __DEFS_H__
#define __DEFS_H__

/* Essa biblioteca implementa definicoes que sao usadas pelo programa. */
/* ATENÇÃO: NÃO APAGUE OU EDITE O NOMES FORNECIDOS */

#include <pthread.h>
#include <semaphore.h>

#define TRUE 1              // Em C nao temos True ou False (como no Python). Usamos macro TRUE para True (como no Python). 
#define FALSE 0             // e FALSE para False (como no Python).


#define MAX_CAPACITY_TOY    5  // Capacidade maxima dos brinquedos.
#define MIN_CAPACITY_TOY    3   // Capacidade minima dos brinquedos.
#define EXECUTION_TIME_TOY  3   // Tempo de execução dos brinquedos.
#define WAIT_TIME_TOY       5   // Tempo de espera para entrada de clientes
#define MAX_COINS           3  // Maximo de moedas que um cliente pode comprar

#define DEBUG               1   //  Alterne (0 or 1) essa macro se voce espera desabilitar todas as mensagens de debug.



/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct ticket{
  pthread_t thread;         // A thread do funcionário que fica na bilheteria.
  int id;                   // O id do funcionario que fica na bilheteria.
} ticket_t;

/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct toy{
  int id;                    // O id de um brinquedo.
  int capacity;              // A capacidade total de um brinquedo.
  sem_t capacity_semaphore;  // Semáforo para controle da capacidade máxima de pessoas no brinquedo
  sem_t join_semaphore;      // Semáforo para controle dos clientes que entraram no brinquedo
  pthread_mutex_t toy_mutex; // Mutex usado para controlar a execução do brinquedo
  pthread_cond_t running_toy_cond; // Condição que verifica se está em execução
  int toy_isrunning; // variável que identifica se o brinquedo está executando ou não
  pthread_t thread;          // A thread de um brinquedo.
} toy_t;

/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct client{
  int id;                   // O id do cliente.
  int coins;                // Quantidade de moedas do cliente.
  int number_toys;          // Numero de brinquedos disponiveis.
  toy_t **toys;             // (Copy) Array de brinquedos.
  sem_t semaphore;          // Semaforo do cliente
} client_t;

/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct{
  ticket_t **tickets;        // Array de funcionarios.                       
  int n;                    // Quantidade de funcionários da bilheteria.
} tickets_args;

/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct{
  toy_t **toys;             // Array de brinquedos.
  int n;                    // Quantidade de brinquedos.
} toy_args;

/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct{
  client_t **clients;       // Array de clientes.
  int n;                    // Quantidade de clientes.
} client_args;


/**************************************************
 *                    ATENÇÃO                     *
 *   NÃO EDITAR ESSE ARQUIVO A PARTIR DESTE PONTO *
 *                    ATENÇÃO                     *
 *************************************************/

// Estrutura da fila
typedef struct node {
  int data;                 // Dado da fila.
  struct node *next;        // Objeto do proximo item da fila.
} Node;

// Fila
typedef struct queue {
  Node *front;              // Primeiro elemento da fila.
  Node *rear;               // Ultimo elemento da fila.
} Queue;

#if DEBUG
#define debug(...) printf(__VA_ARGS__);
#else
#define debug(...);
#endif


#endif