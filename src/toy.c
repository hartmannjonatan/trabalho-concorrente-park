/**
 * Esse arquivo tem como objetivo a implementação de um brinquedo em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "toy.h"
#include "shared.h"

pthread_t *threads_toys; // Vetor de threads de brinquedos inicializadas no open_toys
int n_toys; // Número de brinquedos recebido como parâmetro da main

void access_toy(toy_t* self){
    // 1. Garantir que não passe da capacidade máxima do brinquedo
    // 2. Garantir que o brinquedo seja ligado somente se houver mais de um cliente
    // 3. Garantir que o cliente saia somente após o término do tempo de execução do brinquedo e outros clientes não entrem
    // 4. Nenhum cliente entre dentro do brinquedo enquanto estiver em execução
    // 5. Se houver apenas um cliente esperando em uma quantidade x de tempo no brinquedo ou a capacidade máxima
    //    for atingida, o brinquedo deve ser iniciado

    // Soluções
    // 1. semáforo que inicia com a capacidade máxima do brinquedo (sem_wait no semaforo em access_toy)
    // 2. semáforo que inicia com 0 (sem_post no semaforo em access_toy)
    // 3. semáforo binário que simboliza a execução do brinquedo

    sem_post // Indica que um cliente quer entrar no brinquedo
    mutex_lock 
    return;
}

// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args){

    toy_t* toy = (toy_t*) args; // converte o void* para toy_t*
    
    debug("[ON] - O brinquedo  [%d] foi ligado.\n", toy->id); // Altere para o id do brinquedo

    debug("[OFF] - O brinquedo [%d] foi desligado.\n", toy->id); // Altere para o id do brinquedo

    mutex_unlock

    pthread_exit(NULL);
}


// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args){
    n_toys = args->n; // n_toys recebe o número de threads recebida de args (da main)
    threads_toys = (pthread_t*) malloc(sizeof(pthread_t)*n_toys); // aloca o vetor de n_toys
    for (int i = 0; i < n_toys; i++) {
        threads_toys[i] = args->toys[i]->thread; // associa cada elemento do vetor de threads com a thread já definida em toys_t
        pthread_create(&threads_toys[i], NULL, turn_on, (void *)args->toys[i]); // cria uma thread por brinquedo chamando o método turn_on
    }
}

// Desligando os brinquedos
void close_toys(){
    for (int i = 0; i < n_toys; i++)
        pthread_join(threads_toys[i], NULL); // finaliza a thread de cada brinquedo

    free(threads_toys); // desaloca o vetor de threads
}