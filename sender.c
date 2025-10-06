/*
    Simple udp client
    Silver Moon (m00n.silv3r@gmail.com)
*/
#include "headers/roteador.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER "127.0.0.1"

int send_msg(Mensagem msg) {
  struct sockaddr_in si_other;
  int s, i, slen = sizeof(si_other);

  if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    die("socket");
  }

  memset((char *)&si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(roteadores[msg.id_destino - 1]
                                ->porta); // pega a porta do roteador de destino

  if (inet_aton(SERVER, &si_other.sin_addr) == 0) {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }

  if (sendto(s, &msg, sizeof(msg), 0, (struct sockaddr *)&si_other, slen) ==
      -1) {
    die("sendto()");
  }

  close(s);
  return 0;
}

void sender(void *arg) {
  while (1) {
    sem_wait(&fila_s.sem);
    pthread_mutex_lock(&fila_s.mutex);

    Mensagem msg = fila_s.msg[fila_s.first];
    send_msg(msg);

    // tratamento pra quando a fila de entrada de outro roteador estiver cheia
    pthread_mutex_unlock(&fila_s.mutex);
  }
}
