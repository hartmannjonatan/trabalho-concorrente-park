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

// Declaração das assinaturas das funções privadas (não disponíveis em client.h)
void *enjoy(void *arg);
void buy_coins(client_t *self);
void wait_ticket(client_t *self);
void queue_enter(client_t *self);

pthread_t *threads_clientes; // Vetor de threads de clientes inicializadas no open_gate
int n_clientes; // Número de clientes recebido como parâmetro da main

// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg){
    client_t* client = (client_t*) arg; // converte o void* para client_t*
    wait_ticket(client); // cliente espera pela liberação da bilheteria

    debug("[EXIT] - O turista saiu do parque.\n");
    pthread_exit(NULL);
}

// Funcao onde o cliente compra as moedas para usar os brinquedos
void buy_coins(client_t *self){
    // Sua lógica aqui
}

// Função onde o cliente espera a liberacao da bilheteria para adentrar ao parque.
void wait_ticket(client_t *self){
    debug("[WAITING] - Cliente [%d] esperando pela abertura da bilheteria\n", self->id);
    // essa função bloqueia o cliente até que a condition "open_tickets" seja satisfeita
    pthread_mutex_lock(&tickets_mutex);  // Trava o mutex para garantir acesso exclusivo à condição
    while (!tickets_isopen) {  // Verifica o estado da condição para evitar Spurious Wakeups
        pthread_cond_wait(&open_tickets_cond, &tickets_mutex);  // Espera pela liberação da bilheteria
        // vale lembrar que o pthread_cond_wait quando chamado faz um unlock no tickets_mutex
        // e até que o sinal de condição válida (feito em ticket.c) seja feito, a thread do cliente fica bloqueada
    }
    pthread_mutex_unlock(&tickets_mutex); // Destrava o mutex relacionado à espera, significando que a bilheteria foi aberta

    debug("[WAITING] - Cliente [%d] pode acessar a fila da bilheteria\n", self->id);

    //queue_enter(self);
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
    n_clientes = args->n; // n_clientes recebe o número de threads recebida de args (da main)
    threads_clientes = (pthread_t*) malloc(sizeof(pthread_t)*n_clientes); // aloca o vetor de n_clientes
    for (int i = 0; i < n_clientes; i++) {
        pthread_create(&threads_clientes[i], NULL, enjoy, (void *)args->clients[i]); // cria uma thread por cliente chamando o método enjoy
    }
}

// Essa função deve finalizar os clientes
void close_gate(){
    for (int i = 0; i < n_clientes; i++)
        pthread_join(threads_clientes[i], NULL); // finaliza a thread de cada cliente
    free(threads_clientes); // desaloca o vetor de threads
}
