/*
    Simple udp server
    Silver Moon (m00n.silv3r@gmail.com)
*/
#include "headers/roteador.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 512 // Max length of buffer

void receiver(void *arg) {
  struct sockaddr_in si_me, si_other;

  int s, i, slen = sizeof(si_other), recv_len;
  Mensagem msg;

  // create a UDP socket
  if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    die("socket");
  }

  // zero out the structure
  memset((char *)&si_me, 0, sizeof(si_me));

  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(roteadores[meu_id - 1]->porta);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  // bind socket to port
  if (bind(s, (struct sockaddr *)&si_me, sizeof(si_me)) == -1) {
    die("bind");
  }

  // keep listening for data
  while (1) {
    // printf("Waiting for data...");
    fflush(stdout);
    // receive a reply and print it
    // clear the buffer by filling null, it might have previously received data
    memset(&msg, '\0', sizeof(msg));

    // try to receive some data, this is a blocking call
    if ((recv_len = recvfrom(s, &msg, sizeof(msg), 0,
                             (struct sockaddr *)&si_other, &slen)) == -1) {
      die("recvfrom()");
    }

    // print details of the client/peer and the data received
    // printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr),
    //        ntohs(si_other.sin_port));

    // printf("%d %s", msg.id_origem, msg.payload.texto);
    pthread_mutex_lock(&fila_e.mutex);

    fila_e.msg[fila_e.next] = msg;
    fila_e.next = (fila_e.next + 1) % 10;

    pthread_mutex_unlock(&fila_e.mutex);
    sem_post(&fila_e.sem);
  }

  close(s);
  return;
}
