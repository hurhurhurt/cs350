#include <pthread.h>
#include <stdio.h>


int counter = 10;
void *worker(void *arg) {
  counter--;
  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t p1, p2;
  pthread_create(&p1, NULL, worker, NULL);
  pthread_create(&p2, NULL, worker, NULL);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  printf("%d\n", counter);
  
  return 0;
}
