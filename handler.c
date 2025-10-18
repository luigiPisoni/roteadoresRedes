#include "headers/roteador.h"
#include <stdio.h>

// vai verificar as mensagens da fila de entrada e ver se precisa passar pra
// outro roteador. se nao, printa no roteador atual
void *handler(void *arg) {
  while (1) {
    sem_wait(&fila_e.sem);
    pthread_mutex_lock(&fila_e.mutex);

    Mensagem msg = fila_e.msg[fila_e.first];
    if (msg.id_destino == meu_id) {
      if (msg.tipo == DADO) {
        printf("R%d: %s\n", msg.id_origem, msg.payload);
      } else {
        printf("R%d: CONTROLE!!!\n", msg.id_origem);
      }
    } else {
      // tratamento pra quando a mensagem nao e pra esse roteador
    }

    // tratamento pra quando sai uma mensagem da fila
    pthread_mutex_unlock(&fila_e.mutex);
  }
};