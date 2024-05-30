#include "shared.h"

#include <queue.h>


// Você pode declarar novas funções (ou variaveis compartilhadas) aqui
client_t** array_clients = NULL; // Array de clientes
int n_clients = -1;

pthread_mutex_t tickets_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex usado para controlar a abertura da bilheteria (clientes aguardam que esta seja aberta)
pthread_cond_t open_tickets_cond = PTHREAD_COND_INITIALIZER; // Condição que verifica se a bilheteria já foi aberta
int tickets_isopen = 0; // variável que identifica se a bilheteria está aberta ou não

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex usado para controlar a entrada dos clientes na fila da bilheteria
sem_t queue_semaphore; // Semáforo usado para controlar a entrada e saída dos clientes na fila da bilheteria

/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
Queue *gate_queue = NULL;