/**
 * Esse arquivo tem como objetivo a implementação do cliente em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS
*/

#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "client.h"
#include "queue.h"
#include "shared.h"

pthread_t *client_threads;
int n_threads;

// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg){
    client_t* client = (client_t*) arg;

    debug("[EXIT] - O turista saiu do parque.\n");
    pthread_exit(NULL);
}

// Funcao onde o cliente compra as moedas para usar os brinquedos
void buy_coins(client_t *self){
    // Sua lógica aqui
}

// Função onde o cliente espera a liberacao da bilheteria para adentrar ao parque.
void wait_ticket(client_t *self){
    // Sua lógica aqui
}

// Funcao onde o cliente entra na fila da bilheteria
void queue_enter(client_t *self){
    // Sua lógica aqui.
    debug("[WAITING] - Turista [%d] entrou na fila do portao principal\n", self->id);

    // Sua lógica aqui.
    buy_coins(self);

    // Sua lógica aqui.
    debug("[CASH] - Turista [%d] comprou [%d] moedas.\n", self->id, self->coins);
}

// Essa função recebe como argumento informações sobre o cliente e deve iniciar os clientes.
void open_gate(client_args *args){
    n_threads = args->n;
    client_threads = (pthread_t *) malloc(sizeof(pthread_t)*args->n);
    for (int i = 0; i < args->n; i++) {
        pthread_create(&client_threads[i], NULL, enjoy, (void *)args->clients[i]);
    }
}

// Essa função deve finalizar os clientes
void close_gate(){
    for (int i = 0; i < n_threads; i++)
        pthread_join(client_threads[i], NULL);
    free(client_threads);
}
