#include "headers/roteador.h"
#include <stdio.h>
#include <time.h>

// vai verificar as mensagens da fila de entrada e ver se precisa passar pra
// outro roteador. se nao, printa no roteador atual
void *handler(void *arg) {
  while (1) {
    sem_wait(&fila_e.sem);
    pthread_mutex_lock(&fila_e.mutex);

    Mensagem msg = fila_e.msg[fila_e.first];
    fila_e.first = (fila_e.first + 1) % 10;

    if (msg.id_destino == meu_id) {
      if (msg.tipo == DADO) {
        printf("R%d diz: %s\n", msg.id_origem, msg.payload.texto);
        printf("::>");
      } else {
        // roda toda vez que recebe uma mensagem de controle
        tabela_distancia[msg.id_origem - 1].ultima_mensagem = time(NULL);

        // vai percorrer a tabela e verificar quais roteadores ja enviaram
        // mensagens (ultima mensagem > 0) e quais ja se passaram 15 segundos
        for (int i = 0; i < MAX_ROTEADORES; i++) {
          if (tabela_distancia[i].ultima_mensagem > 0 &&
              (tabela_distancia[i].ultima_mensagem + 20) < time(NULL)) {
            // caso caia aqui significa que passaram mais de 15 segundos desde a
            // ultima mensagem, precisando tirar o roteador da tabela de
            // distancia

            for (int j = 0; j < MAX_ROTEADORES; j++) {
              tabela_distancia[i].v_distancia[j].destino = 0;
              tabela_distancia[i].v_distancia[j].custo = 0;
            }
          }
        }

        printf("R%d: ", msg.id_origem);
        for (int j = 0; j < MAX_ROTEADORES; j++) {
          tabela_distancia[msg.id_origem - 1].v_distancia[j].destino =
              msg.payload.v_distancia[j].destino;
          tabela_distancia[msg.id_origem - 1].v_distancia[j].custo =
              msg.payload.v_distancia[j].custo;
          printf("(%d, %d)", msg.payload.v_distancia[j].destino,
                 msg.payload.v_distancia[j].custo);
        }
      }
    } else {
      // tratamento pra quando a mensagem nao e pra esse roteador
    }

    // tratamento pra quando sai uma mensagem da fila
    pthread_mutex_unlock(&fila_e.mutex);
  }
};