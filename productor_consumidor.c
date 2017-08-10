#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>


#define MAX_BUFFER 1024 
#define DATOS_A_PRODUCIR 100000 

pthread_mutex_t mutex; 
pthread_cond_t no_lleno=PTHREAD_COND_INITIALIZER; 
pthread_cond_t no_vacio=PTHREAD_COND_INITIALIZER; 
int cola; 
////////////////
int entrada = 0;
int salida = 0;
int contador = 0;
int vacio=DATOS_A_PRODUCIR;


//Producir
int producir() {
    printf("Contador = %dn", contador);
    printf("[+] Produciendo n");
    return 5;
}


void Productor(void *arg) { 
  int num=(int )arg;
  pthread_mutex_lock(&mutex); 

  while (cola == MAX_BUFFER){ /* lleno */
   pthread_cond_wait(&no_lleno, &mutex); /*  bloqueo */
   

 int item;
    item = producir();
    buffer[entrada] = item;
      entrada = (entrada + 1) % DATOS_A_PRODUCIR;
      contador++;
 //pthread_cond_signal(&mutex);
   //    pthread_cond_signal(&lleno);


 }

  printf("Productor i ha producido 1 item, tamaño cola = : %d \n", cola);
  cola ++;



 
 pthread_cond_signal(&&mutex);


  pthread_exit(0);
}



 // Consumir
int consumir(int item) {
    printf("Contador = %dn", contador);
    printf("[-] Consumiendo n");
    return 0;
}




void Consumidor(void){
  int num=(int )arg;
  pthread_mutex_lock(&mutex); 
  while (cola == 0){      
   pthread_cond_wait(&no_vacio, &mutex);

   item = buffer[salida];
   salida = (salida + 1) % DATOS_A_PRODUCIR;
   contador--;
 //pthread_cond_signal(&mutex);
 // pthread_cond_signal(&vacio);
   consumir(item);

  }





  printf("Consumidor j ha consumido 1 item, tamaño cola = ...  \n", cola); /* consume dato */
  cola --;
  pthread_cond_signal(&no_lleno); 
  pthread_mutex_unlock(&mutex);
  pthread_exit(0);
}
 
int main(int argc, char *argv[]){
  if(argc>=7){
    int num_hilos_prod=atoi(argv[1]);
    float tiempo_prod=(float) atoll(argv[2]);
    int num_hilos_cons=atoi(argv[3]);
    float tiempo_cons=(float) atoll(argv[4]);
    int tamcola=atoi(argv[5]);
    int total_items=atoi(argv[6]);

    //creando hilos
    int  hilo;
    for(hilo=0;hilo<num_hilos_cons;hilo++) {
      status = pthread_create(&hilos[hilo], NULL,Consumidor, (void *)hilo);
      if(status < 0){
        fprintf(stderr, "Error al crear el hilo : %d\n");
        exit(-1); 
      }



   //productor
    for(hilo=0;hilo<num_hilos_prod;hilo++) {
      status = pthread_create(&hilos[hilo], NULL,Productor, (void *)hilo);
      if(status < 0){
        fprintf(stderr, "Error al crear el hilo : %d\n");
        exit(-1); 
      }
    }


    pthread_t th1, th2;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&no_lleno, NULL);
    pthread_cond_init(&no_vacio, NULL);
    pthread_create(&th1, NULL, (void *)&Productor, NULL);
    pthread_create(&th2, NULL, (void *)&Consumidor, NULL);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&no_lleno);
    pthread_cond_destroy(&no_vacio);
    exit(0);
}
