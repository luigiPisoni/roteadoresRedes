#include "pthread.h"
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_ROTEADORES 10

typedef enum { CONTROLE, DADO } TipoMensagem;

typedef struct {
  int id;
  int porta;
  char ip[16];
} Roteador;

typedef struct {
  int destino;
  int custo;
} Distancia;

typedef struct {
  time_t ultima_mensagem;
  Distancia v_distancia[MAX_ROTEADORES];
} TabelaDistancia;

typedef struct {
  TipoMensagem tipo;
  int id_origem;
  int id_destino;
  union {
    char texto[100];
    Distancia v_distancia[MAX_ROTEADORES];
  } payload;
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
extern Distancia v_distancia[MAX_ROTEADORES];
extern TabelaDistancia tabela_distancia[MAX_ROTEADORES];