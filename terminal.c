#include "headers/roteador.h"
#include <semaphore.h>
#include <stdio.h>
#include <string.h>

void terminal(void *arg) {
  char comando[10];
  int id_destino;
  char conteudo[100];

  while (1) {
    printf("Digite um comando do terminal\n::>");
    scanf("%9s %d %99s", comando, &id_destino, conteudo);
    Mensagem msg;

    if (strcmp(comando, "enviar") == 0) {
      msg.tipo = DADO;
    } else if (strcmp(comando, "controle") == 0) {
      msg.tipo = CONTROLE;
    } else {
      printf("Comando invalido");
      return;
    }

    msg.id_origem = meu_id;
    msg.id_destino = id_destino;
    strcpy(msg.payload.texto, conteudo);

    // controle de posicoes da fila
    pthread_mutex_lock(&fila_s.mutex);
    
    fila_s.msg[fila_s.next] = msg;
    fila_s.next = (fila_s.next + 1) % 10;

    pthread_mutex_unlock(&fila_s.mutex);
    sem_post(&fila_s.sem);

  }
};
