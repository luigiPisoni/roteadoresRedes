#include "pthread.h"
#include <pthread.h>
#include <semaphore.h>

#define MAX_ROTEADORES 10

typedef enum { CONTROLE, DADO } TipoMensagem;

typedef struct {
  int id;
  int porta;
  char ip[16];
} Roteador;

typedef struct {
  TipoMensagem tipo;
  int id_origem;
  int id_destino;
  char payload[100];
} Mensagem;

typedef struct {
  int first;
  int next;
  Mensagem msg[10];
  sem_t sem;
  pthread_mutex_t mutex;
} Fila;

void die(char *s);

extern Fila fila_e;
extern Fila fila_s;
extern int meu_id;
extern Roteador *roteadores[MAX_ROTEADORES];