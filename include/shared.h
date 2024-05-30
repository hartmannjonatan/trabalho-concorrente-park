#ifndef __SHARED_H__
#define __SHARED_H__

#include <defs.h>

// Você pode declarar novas funções (ou variaveis compartilhadas) aqui

extern pthread_mutex_t tickets_mutex; // Mutex usado para controlar a abertura da bilheteria (clientes aguardam que esta seja aberta)
extern pthread_cond_t open_tickets_cond; // Condição que verifica se a bilheteria já foi aberta
extern int tickets_isopen; // variável que identifica se a bilheteria está aberta ou não


/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
extern Queue *gate_queue;

#endif