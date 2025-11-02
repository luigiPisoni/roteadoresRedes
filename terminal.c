#include "headers/roteador.h"
#include <semaphore.h>
#include <stdio.h>
#include <string.h>

void terminal(void *arg) {
  char comando[10];
  int id_destino;
  char conteudo[100];

  while (1) {
    char linha[200];
    printf("Digite um comando do terminal\n::>");
    fgets(linha, sizeof(linha), stdin);

    sscanf(linha, "%9s %d %99s", comando, &id_destino, conteudo);

    Mensagem msg;

    if (strcmp(comando, "enviar") == 0) {
      msg.tipo = DADO;

      msg.id_origem = meu_id;
      msg.id_destino = id_destino;
      strcpy(msg.payload.texto, conteudo);

      // controle de posicoes da fila
      pthread_mutex_lock(&fila_s.mutex);

      fila_s.msg[fila_s.next] = msg;
      fila_s.next = (fila_s.next + 1) % 10;

      pthread_mutex_unlock(&fila_s.mutex);
      sem_post(&fila_s.sem);
    } else if (strcmp(comando, "tabela") == 0) {
      printf("--- TABELA DE DISTANCIA ---\n");
      for (int i = 0; i < MAX_ROTEADORES; i++) {
        printf("R%d: ", (i + 1));
        for (int j = 0; j < MAX_ROTEADORES; j++) {
          printf("(%d, %d)", tabela_distancia[i].v_distancia[j].destino,
                 tabela_distancia[i].v_distancia[j].custo);
        }
        printf("\n");
      }
    } else {
      printf("(!) Comando invalido\n");
    }
  }
};
