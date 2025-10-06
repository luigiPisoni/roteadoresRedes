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
    //   printf("%s %d %s", comando, id_destino, conteudo);
    Mensagem msg;

    msg.tipo = DADO;
    msg.id_origem = meu_id;
    msg.id_destino = id_destino;
    strcpy(msg.payload, conteudo);

    // controle de posicoes da fila
    fila_s.msg[fila_s.first] = msg;
    fila_s.next = (fila_s.next + 1) % 10;

    sem_post(&fila_s.sem);
  }
};
