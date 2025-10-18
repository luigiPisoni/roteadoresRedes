#include "headers/handler.h"
#include "headers/receiver.h"
#include "headers/roteador.h"
#include "headers/sender.h"
#include "headers/terminal.h"
#include "pthread.h"
#include <arpa/inet.h>
#include <bits/pthreadtypes.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

pthread_t thread_receiver, thread_sender, thread_handler, thread_terminal;

int meu_id;
int adjacencia[MAX_ROTEADORES]; // o index representa o roteador de destino
Roteador *roteadores[];

Fila fila_e;
Fila fila_s;

void get_roteadores() {
  FILE *f = fopen("config/roteadores.config", "r");
  if (f == NULL) {
    printf("erro ao ler o arquivo roteadores.config");
    return;
  }

  int id, porta;
  char ip[16];

  // printf("aiusdiu9hyysad");
  while (fscanf(f, "%d %d %15s", &id, &porta, ip) == 3) {
    Roteador *novo_roteador = malloc(sizeof(Roteador));

    if (novo_roteador == NULL) {
      fprintf(stderr, "falha alocando memoria a um novo roteador.\n");
      fclose(f);
      return;
    }
    // printf("%d %d", id, porta);
    novo_roteador->id = id;
    novo_roteador->porta = porta;
    strncpy(novo_roteador->ip, ip, 15);

    roteadores[id - 1] = novo_roteador;
  }
  return;
  // printa os roteadores lidos do .config
  for (int i = 0; i < MAX_ROTEADORES; i++) {
    printf("%d %d %s\n", roteadores[i]->id, roteadores[i]->porta,
           roteadores[i]->ip);
  }
}

void get_enlaces() {
  FILE *f = fopen("config/enlaces.config", "r");
  if (f == NULL) {
    printf("erro ao ler o arquivo enlaces.config");
    return;
  }

  // esse for faz com que
  for (int i = 0; i < MAX_ROTEADORES; i++) {
    adjacencia[i] = -1;
  }
  adjacencia[meu_id] = 0; // o custo de 1 pra ele mesmo e 0
  int id_origem, id_destino, custo;

  while (fscanf(f, "%d %d %d", &id_origem, &id_destino, &custo) == 3) {
    // printf("%d %d %d\n", id_origem, id_destino, custo);
    if (id_origem == meu_id) {
      adjacencia[id_destino] = custo;
    } else if (id_destino == meu_id) {
      adjacencia[id_origem] = custo;
    }
  }
  return;
  for (int i = 0; i < MAX_ROTEADORES; i++) {
    printf("%d, ", adjacencia[i]);
  }
  printf("\n");
}

void iniciar_fila(Fila *f) {
  f->first = 0;
  f->next = 0;
  sem_init(&f->sem, 0, 0);
  pthread_mutex_init(&f->mutex, NULL);
}

void die(char *s) {
  perror(s);
  exit(1);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("ERRO: inicializacao invalida.");
    return 1;
  }
  meu_id = atoi(argv[1]);

  // inicia as filas
  iniciar_fila(&fila_e);
  iniciar_fila(&fila_s);

  get_roteadores();
  get_enlaces(); // adiciona os enlaces no vetor de adjacencia

  // -------    INICIALIZACAO DE THREADS    -------
  if (pthread_create(&thread_handler, NULL, &handler, NULL) != 0) {
    printf("ERRO: criacao de thread handler falhou");
    return -1;
  }
  if (pthread_create(&thread_receiver, NULL, &receiver, NULL) != 0) {
    printf("ERRO: criacao de thread receiver falhou");
    return -1;
  }
  if (pthread_create(&thread_sender, NULL, &sender, NULL) != 0) {
    printf("ERRO: criacao de thread  falhou");
    return -1;
  }
  if (pthread_create(&thread_terminal, NULL, &terminal, NULL) != 0) {
    printf("ERRO: criacao de thread terminal falhou");
    return -1;
  }

  pthread_join(thread_handler, NULL);
  pthread_join(thread_receiver, NULL);
  pthread_join(thread_sender, NULL);
  pthread_join(thread_terminal, NULL);
}