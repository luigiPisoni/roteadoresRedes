
#include "headers/roteador.h"
#include <stdio.h>
#include <unistd.h>

void send_distance() {
  while (1) {
    sleep(10);
    int para_enviar[MAX_ROTEADORES], j = 0;
    for (int i = 0; i < MAX_ROTEADORES; i++) {
      if (v_distancia[i].destino > 0) {
        para_enviar[j] = v_distancia[i].destino;
        j++;
      }
    }

    for (int i = 0; i < j; i++) {
      Mensagem msg_controle;
      msg_controle.id_origem = meu_id;
      msg_controle.id_destino = para_enviar[i];
      msg_controle.tipo = CONTROLE;

      // transfere os dados do vetor distancia desse roteador pro payload da
      // mensagem
      for (int i = 0; i < MAX_ROTEADORES; i++) {
        msg_controle.payload.v_distancia[i].destino = v_distancia[i].destino;
        msg_controle.payload.v_distancia[i].custo = v_distancia[i].custo;
      }

      pthread_mutex_lock(&fila_s.mutex);

      fila_s.msg[fila_s.next] = msg_controle;
      fila_s.next = (fila_s.next + 1) % 10;

      pthread_mutex_unlock(&fila_s.mutex);
      sem_post(&fila_s.sem);

      printf("Enviando para R%d: ", msg_controle.id_destino);
      for (int j = 0; j < MAX_ROTEADORES; j++) {
        printf("(%d, %d)", msg_controle.payload.v_distancia[j].destino,
               msg_controle.payload.v_distancia[j].custo);
      }
      printf("\n");
    }
  }
}