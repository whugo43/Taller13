#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cc = PTHREAD_COND_INITIALIZER;
pthread_cond_t cp = PTHREAD_COND_INITIALIZER;
//globales de parametros
int tam_cola = 0;
int total_items = 0;
double tiempo_prod = 0;
double tiempo_cons = 0;

int cola = 0;
int producidos = 0;
int consumidos=0;

void* Consumidor(void * arg){ 
  int fin=0;
  while(fin==0){  
    pthread_mutex_lock(&mutex);
      while(cola == 0 &&  producidos<total_items)
        pthread_cond_wait(&cc,&mutex);
      usleep(tiempo_cons);
      if(consumidos<total_items){
      consumidos++;
      --cola;
      printf("Consumidor %d ha consumido 1 item, tamaño cola = %d\n",*((int *)arg),cola);
    }else{
      fin=10 ;
    }
    pthread_cond_signal(&cp);
    pthread_mutex_unlock(&mutex);
    
    
  }
  return (void*)1;  
}


void* Productor(void * arg){ 
  int fin=0;
  while(fin==0){
    pthread_mutex_lock(&mutex);
    while(cola == tam_cola && consumidos<total_items)
      pthread_cond_wait(&cp,&mutex);
     usleep(tiempo_prod);
    if(producidos<total_items){
    cola++;
    producidos++;
    printf("Productor %d ha producido 1 item, tamaño cola = %d\n",*((int *)arg),cola);
  }else{
fin=10;
  }
    pthread_cond_signal(&cc); 
   pthread_mutex_unlock(&mutex);
   
  }
  return (void*)1;
}


int main(int argc, char** argv){
  if (argc==7){
    //obteniendo los parametros 
    int num_hilos_prod = atoi(argv[1]);
    tiempo_prod = atof(argv[2])/1000000;
    int num_hilos_cons = atoi(argv[3]);
    tiempo_cons = atof(argv[4])/1000000;
    tam_cola = atoi(argv[5]);
    total_items = atoi(argv[6]);

    //mostrando informacion
    printf("Numero de productores: %d\n", num_hilos_prod);
    printf("Numero de consumidores: %d\n", num_hilos_cons);
    printf("Tamaño de cola: %d elementos\n", tam_cola);
    printf("Tiempo de consumo: %.2f segundos\n", tiempo_cons*1000000);
    printf("Tiempo de producción: %.2f segundos\n", tiempo_prod*1000000);
    printf("Total de items a producir: %d elementos.\n\n", total_items);

    //creando arreglos para almacenar para almacenar todos los hilos requeridos
    pthread_t *consumidores = (pthread_t *)malloc(sizeof(pthread_t)*num_hilos_cons);
    pthread_t *productores = (pthread_t *)malloc(sizeof(pthread_t)*num_hilos_prod);  

    //determinar la maxima cantidad de hilo a usar
    int hiloMax = 0;
    if(num_hilos_cons > num_hilos_prod)
      hiloMax = num_hilos_cons;
    else
      hiloMax = num_hilos_prod;
    
    int *arreglos=malloc(hiloMax*sizeof(int));
    for(int i=0;i<hiloMax;i++){
      arreglos[i] = i+1;
    }

    //creando los hilos
    for(int i=0;i<hiloMax;i++){
      if(i<num_hilos_prod) pthread_create(&(productores[i]),NULL,Productor,&arreglos[i]);
      if(i<num_hilos_cons) pthread_create(&(consumidores[i]),NULL,Consumidor,&arreglos[i]); 
      
    }
    //esperando hilos
    
    for(int i=0;i<hiloMax;i++){
      if(i<num_hilos_prod) pthread_join(productores[i],NULL);
      if(i<num_hilos_cons) pthread_join(consumidores[i],NULL);
      
    }
    pthread_cond_destroy(&cc);
    pthread_cond_destroy(&cp);

    return 0;
}else{
  printf("Uso del programa <num_hilos_prod> <tiempo_prod> <num_hilos_cons> <tiempo_cons> <tam_cola> <total_items> \n");
  return -1;
  }
}


