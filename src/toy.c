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


void access_toy(toy_t* self, int client_id){ //ATENÇÃO, remover o client_id depois
    // 1. Garantir que não passe da capacidade máxima do brinquedo                                                              OK
    // 2. Garantir que o brinquedo seja ligado somente se houver mais de um cliente                                             Não
    // 3. Garantir que o cliente saia somente após o término do tempo de execução do brinquedo e outros clientes não entrem     OK
    // 4. Nenhum cliente entre dentro do brinquedo enquanto estiver em execução                                                 OK
    // 5. Se houver apenas um cliente esperando em uma quantidade x de tempo no brinquedo ou a capacidade máxima                Em andamento
    //    for atingida, o brinquedo deve ser iniciado
    
}

// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args){

    // toy_t* toy = (toy_t*) args; // converte o void* para toy_t*


    pthread_exit(NULL);
}


// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args){
    n_toys = args->n; // n_toys recebe o número de threads recebida de args (da main)
    array_toys = args->toys;
    threads_toys = (pthread_t*) malloc(sizeof(pthread_t)*n_toys); // aloca o vetor de n_toys
    for (int i = 0; i < n_toys; i++) {
        threads_toys[i] = args->toys[i]->thread; // associa cada elemento do vetor de threads com a thread já definida em toys_t
        pthread_create(&threads_toys[i], NULL, turn_on, (void *)args->toys[i]); // cria uma thread por brinquedo chamando o método turn_on
    }
}

// Desligando os brinquedos
void close_toys(){
    // lembrar de avisar os brinquedos para finalizar sua execução
    for (int i = 0; i < n_toys; i++) {
        pthread_join(threads_toys[i], NULL); // finaliza a thread de cada brinquedo
    }

    free(threads_toys); // desaloca o vetor de threads
}