
#include "headers/roteador.h"
#include <stdio.h>
#include <unistd.h>

void send_distance() {
  while (1) {
    // printf("alo");
    sleep(5);
    int para_enviar[MAX_ROTEADORES], j = 0;
    for (int i = 0; i < MAX_ROTEADORES; i++) {
      if (v_distancia[i][0] > 0) {
        para_enviar[j] = v_distancia[i][0];
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
        msg_controle.payload.v_distancia[i][0] = v_distancia[i][0];
        msg_controle.payload.v_distancia[i][1] = v_distancia[i][1];
      }

      pthread_mutex_lock(&fila_s.mutex);

      fila_s.msg[fila_s.next] = msg_controle;
      fila_s.next = (fila_s.next + 1) % 10;

      pthread_mutex_unlock(&fila_s.mutex);
      sem_post(&fila_s.sem);
    }
  }
}