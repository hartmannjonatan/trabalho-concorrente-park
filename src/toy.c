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


    //   fila das próximas rodadas    |->   clientes a entrar    |->   brinquedo
    sem_wait(&self->capacity_semaphore); // preso na fila das próximas rodadas
    // sem_wait(&self->enter_semaphore); // preso na entrada do brinquedo (em execução)
    // debug("[INFO] - Cliente [%d] entrou no brinquedo [%d].\n",client_id, self->id);
    // pthread_mutex_lock(&self->n_clientes_mutex);
    // self->clients_in_toy++;
    // if(self->clients_in_toy == 1)
    //     sem_post(&self->join_semaphore);
    // pthread_mutex_unlock(&self->n_clientes_mutex);
    // sem_post(&self->enter_semaphore); // libera o cliente atrás dele a entrar no brinquedo também

    pthread_mutex_lock(&self->enter_mutex);
    while(!self->enter_toy) {
        pthread_cond_wait(&self->enter_cond, &self->enter_mutex);
    }
    pthread_mutex_unlock(&self->enter_mutex);
    debug("[INFO] - Cliente [%d] entrou no brinquedo [%d].\n",client_id, self->id);
    pthread_mutex_lock(&self->n_clientes_mutex);
    self->clients_in_toy++;
    if(self->clients_in_toy == 1)
        sem_post(&self->join_semaphore);
    pthread_mutex_unlock(&self->n_clientes_mutex);

    // sem_wait(&self->exit_semaphore);
    // debug("[INFO] - Cliente [%d] saiu do brinquedo [%d].\n",client_id, self->id);
    // pthread_mutex_lock(&self->n_clientes_mutex);
    // self->clients_in_toy--;
    // if(self->clients_in_toy == 0)
    //     sem_post(&self->ready_semaphore);
    // pthread_mutex_unlock(&self->n_clientes_mutex);
    // sem_post(&self->exit_semaphore);

    pthread_mutex_lock(&self->exit_mutex);
    while(!self->exit_toy) {
        pthread_cond_wait(&self->exit_cond, &self->exit_mutex);
    }
    pthread_mutex_unlock(&self->exit_mutex);
    debug("[INFO] - Cliente [%d] saiu do brinquedo [%d].\n",client_id, self->id);
    pthread_mutex_lock(&self->n_clientes_mutex);
    self->clients_in_toy--;
    if(self->clients_in_toy == 0)
        sem_post(&self->ready_semaphore);
    pthread_mutex_unlock(&self->n_clientes_mutex);

    sem_post(&self->capacity_semaphore);

}

// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args){
    toy_t* toy = (toy_t*) args; // converte o void* para toy_t*

    debug("[INFO] - Brinquedo [%d] em funcionamento!\n", toy->id);
    while(TRUE) {
        debug("[IN] - Brinquedo [%d] aguardando clientes entrarem!\n", toy->id);
        // sem_post(&toy->enter_semaphore);
        toy->enter_toy = 1;
        pthread_cond_broadcast(&toy->enter_cond);
        sem_wait(&toy->join_semaphore);
        // if(!park_status)
        //     break;
        sleep(WAIT_TIME_TOY);
        // sem_wait(&toy->enter_semaphore);
        toy->enter_toy = 0;

        debug("[ON] - Brinquedo [%d] foi ligado!\n", toy->id);
        sleep(EXECUTION_TIME_TOY);
        debug("[OFF] - Brinquedo [%d] foi desligado!\n", toy->id);

        debug("[OUT] - Brinquedo [%d] aguardando clientes sairem!\n", toy->id);
        // sem_post(&toy->exit_semaphore);
        toy->exit_toy = 1;
        pthread_cond_broadcast(&toy->exit_cond);

        sem_wait(&toy->ready_semaphore);
        // sem_wait(&toy->exit_semaphore);
        toy->exit_toy = 0;
    }
    debug("[INFO] - Brinquedo [%d] encerrado!\n", toy->id);

    pthread_exit(NULL);
}


// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args){
    n_toys = args->n; // n_toys recebe o número de threads recebida de args (da main)
    array_toys = args->toys;
    threads_toys = (pthread_t*) malloc(sizeof(pthread_t)*n_toys); // aloca o vetor de n_toys
    for (int i = 0; i < n_toys; i++) {
        threads_toys[i] = args->toys[i]->thread; // associa cada elemento do vetor de threads com a thread já definida em toys_t
        args->toys[i]->clients_in_toy = 0;
        pthread_mutex_init(&args->toys[i]->n_clientes_mutex, NULL);
        sem_init(&args->toys[i]->capacity_semaphore, 0, args->toys[i]->capacity);
        // sem_init(&args->toys[i]->enter_semaphore, 0, 0);
        args->toys[i]->enter_toy = 0;
        pthread_mutex_init(&args->toys[i]->enter_mutex, NULL);
        pthread_cond_init(&args->toys[i]->enter_cond, NULL);
        sem_init(&args->toys[i]->join_semaphore, 0, 0);
        // sem_init(&args->toys[i]->exit_semaphore, 0, 0);
        args->toys[i]->exit_toy = 0;
        pthread_mutex_init(&args->toys[i]->exit_mutex, NULL);
        pthread_cond_init(&args->toys[i]->exit_cond, NULL);
        sem_init(&args->toys[i]->ready_semaphore, 0, 0);
        pthread_create(&threads_toys[i], NULL, turn_on, (void *)args->toys[i]); // cria uma thread por brinquedo chamando o método turn_on
    }
}

// Desligando os brinquedos
void close_toys(){
    // lembrar de avisar os brinquedos para finalizar sua execução
    for (int i = 0; i < n_toys; i++) {
        pthread_join(threads_toys[i], NULL); // finaliza a thread de cada brinquedo
        pthread_mutex_destroy(&array_toys[i]->n_clientes_mutex);
        sem_destroy(&array_toys[i]->capacity_semaphore);
        // sem_destroy(&array_toys[i]->enter_semaphore);
        pthread_mutex_destroy(&array_toys[i]->enter_mutex);
        pthread_cond_destroy(&array_toys[i]->enter_cond);
        sem_destroy(&array_toys[i]->join_semaphore);
        // sem_destroy(&array_toys[i]->exit_semaphore);
        pthread_mutex_destroy(&array_toys[i]->exit_mutex);
        pthread_cond_destroy(&array_toys[i]->exit_cond);
        sem_destroy(&array_toys[i]->ready_semaphore);
    }

    free(threads_toys); // desaloca o vetor de threads
}