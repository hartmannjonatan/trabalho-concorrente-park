/**
 * Esse arquivo tem como objetivo a implementação da bilheteria em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS.
*/
#include <stdio.h>
#include <stdlib.h>
#include <tickets.h>
#include <unistd.h>

#include <queue.h>
#include "shared.h"

pthread_t *threads_tickets; // Vetor de threads de atendentes inicializadas no open_tickets
int n_tickets; // Número de atendentes recebido como parâmetro da main
int counter = 0; // Contador de atendentes inicializados (utilizado para determinar quando a bilheteria abriu - último funcionário)
pthread_mutex_t counter_mutex; // Mutex para o contadorde atendentes (variável global)

// Thread que implementa uma bilheteria
void *sell(void *args){

    pthread_mutex_lock(&counter_mutex); // Mutex que bloqueia a região crítica do counter (e também neste caso da condition tickets_isopen)
    counter++; // contador é incrementado, assim conseguimos saber quantos atendentes foram inicializados
    if (counter == n_tickets) { // condição para abrir a bilheteria (apenas último atendente)
        debug("[INFO] - Bilheteria Abriu!\n"); // printa que a bilheteria abriu (apenas uma vez)
        tickets_isopen = 1; // a variável global que identifica se a bilheteria está aberta é alterada para 1 (true)
        pthread_cond_broadcast(&open_tickets_cond); // utilizamos o cond_boradcast para sinalizar a TODAS as threads dos clientes que a bilheteria está aberta
    }
    pthread_mutex_unlock(&counter_mutex);

    while (TRUE) { // Utiliza a ideia de bag of work para as threads funcionárias
        pthread_mutex_lock(&queue_mutex); // Bloqueia o mutex, para que a retirada do cliente da fila sejá feita de forma atômica
        if (is_queue_empty(gate_queue)) // Verifica se a fila não está vazia
            break; // Se a fila estiver vazia, todas as threads funcionárias devem encerrar seu trabalho, ou seja, dá um break no while
        int id_cliente = dequeue(gate_queue); // Senão, retira o cliente da fila para que seja atendido 
        pthread_mutex_unlock(&queue_mutex); // Desbloqueia o mutex do acesso a fila 
        sem_post(&array_clients[id_cliente-1]->semaphore); // Destrava o cliente que será atendido, ou seja, o cliente que foi retirado da fila, com base no seu id
    }

    pthread_exit(NULL); // Encerra as threads funcionária
}

// Essa função recebe como argumento informações sobre a bilheteria e deve iniciar os atendentes.
void open_tickets(tickets_args *args){
    n_tickets = args->n; // n_tickets recebe o número de threads recebida de args (da main)
    threads_tickets = (pthread_t*) malloc(sizeof(pthread_t)*n_tickets); // aloca o vetor de n_tickets
    for (int i = 0; i < n_tickets; i++) {
        threads_tickets[i] = args->tickets[i]->thread; // associa cada elemento do vetor de threads com a thread já definida em tickets_t
        pthread_create(&threads_tickets[i], NULL, sell, (void *)args->tickets[i]); // cria uma thread por atendente chamando o método sell
    }
}

// Essa função deve finalizar a bilheteria
void close_tickets(){
    for (int i = 0; i < n_tickets; i++)
        pthread_join(threads_tickets[i], NULL); // finaliza a thread de cada atendente

    debug("[INFO] - Bilheteria Fechou!\n") // Printa que a bilheteria fechou, após todos os atendentes finalizarem as vendas
    tickets_isopen = 0; // Seta variável que controla o funcionamento da bilheteria para 0 (embora seja utilizado apenas na inicialização)

    free(threads_tickets); // desaloca o vetor de threads
}