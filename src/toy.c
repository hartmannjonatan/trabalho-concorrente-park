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

    // Soluções
    // 1. semáforo que inicia com a capacidade máxima do brinquedo (sem_wait no semaforo em access_toy)
    // 2. semáforo que inicia com 0 (sem_post no semaforo em access_toy)
    // 3. semáforo binário que simboliza a execução do brinquedo

    sem_wait(&self->capacity_semaphore); // Indica que um cliente quer entrar no brinquedo

    // ** prende o cliente na fila
    // essa função bloqueia o acesso do cliente ao brinquedo até que a execução do brinquedo termine (!toy_isrunning)
    pthread_mutex_lock(&self->toy_mutex);  // Trava o mutex para garantir acesso exclusivo à condição
    while (self->toy_isrunning) {  // Verifica o estado da condição para evitar Spurious Wakeups
        pthread_cond_wait(&self->running_toy_cond, &self->toy_mutex);  // Espera pela liberação do brinquedo
        // vale lembrar que o pthread_cond_wait quando chamado faz um unlock no self->toy_mutex
        // e até que o sinal de condição válida (feito na thread do brinquedo) seja feito, a thread do cliente fica bloqueada
    }
    pthread_mutex_unlock(&self->toy_mutex); // Destrava o mutex relacionado à espera, significando que o brinquedo está disponível

    sem_post(&self->join_semaphore); // Indica que o cliente entrou no brinquedo

    debug("[INFO] - Cliente [%d] entrou no brinquedo [%d]!\n", client_id, self->id);    // ATENÇÃOOOOO, remover isso depois (somente para debug)

    // ** prende o cliente no brinquedo
    // essa função bloqueia a saída do cliente do brinquedo até que a execução do brinquedo termine (!toy_isrunning)
    pthread_mutex_lock(&self->toy_mutex);  // Trava o mutex para garantir acesso exclusivo à condição
    while (!self->toy_isfinish) {  // Verifica o estado da condição para evitar Spurious Wakeups
        debug("Cliente [%d] entrou no while 2\n", client_id);
        pthread_cond_wait(&self->running_toy_cond, &self->toy_mutex);  // Espera pela liberação do brinquedo
        // vale lembrar que o pthread_cond_wait quando chamado faz um unlock no self->toy_mutex
        // e até que o sinal de condição válida (feito na thread do brinquedo) seja feito, a thread do cliente fica bloqueada
    }
    pthread_mutex_unlock(&self->toy_mutex); // Destrava o mutex relacionado à espera, significando que o brinquedo está disponível

    // Cliente saiu do brinquedo e liberou um espaço (aumentando
    // a capacidade disponível e diminuindo o número de pessoas no brinquedo)
    sem_post(&self->capacity_semaphore);
    sem_wait(&self->join_semaphore);
    
}

// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args){

    toy_t* toy = (toy_t*) args; // converte o void* para toy_t*

    int cont = 0;
    while (cont < 3) {
        debug("[INFO] - Clientes podem entrar no brinquedo [%d]!\n", toy->id); // printa que o brinquedo está disponível
        toy->toy_isrunning = 0; // a variável que identifica se o brinquedo está em execução é alterada para 0 (false)
        toy->toy_isfinish = 1;
        pthread_cond_broadcast(&toy->running_toy_cond); // utilizamos o cond_broadcast para sinalizar a TODAS as threads dos clientes que o brinquedo está disponível

        sem_wait(&toy->join_semaphore);
        sleep(WAIT_TIME_TOY);
        toy->toy_isrunning = 1;
        toy->toy_isfinish = 0;
        sem_post(&toy->join_semaphore);

        debug("[ON] - O brinquedo  [%d] foi ligado.\n", toy->id); // Altere para o id do brinquedo
        sleep(EXECUTION_TIME_TOY);
        debug("[OFF] - O brinquedo [%d] foi desligado.\n", toy->id); // Altere para o id do brinquedo

        cont++;

        //mutex_unlock
    }

    pthread_exit(NULL);
}


// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args){
    n_toys = args->n; // n_toys recebe o número de threads recebida de args (da main)
    array_toys = args->toys;
    threads_toys = (pthread_t*) malloc(sizeof(pthread_t)*n_toys); // aloca o vetor de n_toys
    for (int i = 0; i < n_toys; i++) {
        args->toys[i]->toy_isrunning = 0; // inicializa a variável que indica se o brinquedo está executando
        args->toys[i]->toy_isfinish = 1; // inicializa a variável que indica se o brinquedo já finalizou sua execução
        pthread_mutex_init(&args->toys[i]->toy_mutex, NULL); // inicializa o mutex relacionado ao controle de execução do brinquedo
        pthread_cond_init(&args->toys[i]->running_toy_cond, NULL); // inicializa a condição relacionada à execução do brinquedo
        threads_toys[i] = args->toys[i]->thread; // associa cada elemento do vetor de threads com a thread já definida em toys_t
        sem_init(&args->toys[i]->capacity_semaphore, 0, args->toys[i]->capacity); // inicializa o semáforo de capacidade em cada brinquedo
        sem_init(&args->toys[i]->join_semaphore, 0, 0); // inicializa o semáforo de controle de execução em cada brinquedo
        pthread_create(&threads_toys[i], NULL, turn_on, (void *)args->toys[i]); // cria uma thread por brinquedo chamando o método turn_on
    }
}

// Desligando os brinquedos
void close_toys(){
    // lembrar de avisar os brinquedos para finalizar sua execução
    for (int i = 0; i < n_toys; i++) {
        pthread_join(threads_toys[i], NULL); // finaliza a thread de cada brinquedo
        sem_destroy(&array_toys[i]->capacity_semaphore);  // finaliza o semáforo de capacidade em cada brinquedo
        sem_destroy(&array_toys[i]->join_semaphore);      // finaliza o semáforo de entrada de clientes em cada brinquedo
        pthread_mutex_destroy(&array_toys[i]->toy_mutex); // Destroy o mutex de controle da execução do brinquedo
        pthread_cond_destroy(&array_toys[i]->running_toy_cond); // Destroy a condição de execução do brinquedo 
    }

    free(threads_toys); // desaloca o vetor de threads
}