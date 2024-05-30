#include "shared.h"

#include <queue.h>


// Você pode declarar novas funções (ou variaveis compartilhadas) aqui
pthread_mutex_t tickets_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex usado para controlar a abertura da bilheteria (clientes aguardam que esta seja aberta)
pthread_cond_t open_tickets_cond = PTHREAD_COND_INITIALIZER; // Condição que verifica se a bilheteria já foi aberta
int tickets_isopen = 0; // variável que identifica se a bilheteria está aberta ou não

/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
Queue *gate_queue = NULL;