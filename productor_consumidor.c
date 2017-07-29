#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t condicionConsumidor = PTHREAD_COND_INITIALIZER;
pthread_cond_t condicionProductor = PTHREAD_COND_INITIALIZER;
int tam_cola = 0;
int cola = 0;
int total_items = 0;
int producidos = 0;
double tiempo_prod = 0;
double tiempo_cons = 0;

void mostrarinfo(int nhilos_prod, int nhilos_cons, double tiempo_cons, double tiempo_prod, int total_items);
void* productor(void * arg);
void* consumidor(void * arg);


void* productor(void * arg)
{ 
  int salir=0;
  while(salir==0)
  {
    pthread_mutex_lock(&mutex);
    while(cola == tam_cola)
      pthread_cond_wait(&condicionProductor,&mutex);
    usleep(tiempo_prod);
    cola++;
    producidos++;
    printf("Productor %d ha producido 1 item, tamaño cola = %d\n",*((int *)arg),cola);
    if(producidos == total_items)
      salir = 1;
    pthread_mutex_unlock(&mutex);
    pthread_cond_broadcast(&condicionConsumidor);
  } 

  return (void*)1;
}

void* consumidor(void * arg)
{ 
  int salir=0;
  while(salir==0)
  {  
    pthread_mutex_lock(&mutex);
    while(cola == 0)
      pthread_cond_wait(&condicionConsumidor,&mutex);
    usleep(tiempo_cons);
    cola--;
    printf("Consumidor %d ha consumido 1 item, tamaño cola = %d\n",*((int *)arg),cola);
    if(producidos == total_items && cola == 0)
      salir = 1;
    pthread_mutex_unlock(&mutex);
    pthread_cond_broadcast(&condicionProductor);
  }
  return (void*)1;  
}


int main(int argc, char** argv)
{
  if (argc!=7)
  {
  	printf("Uso del programa <num_hilos_prod> <tiempo_prod> <num_hilos_cons> <tiempo_cons> <tam_cola> <total_items>\n");
  	return -1;
  }
  
  int nhilos_prod = atoi(argv[1]);
  tiempo_prod = atof(argv[2])/1000000;
  int nhilos_cons = atoi(argv[3]);
  tiempo_cons = atof(argv[4])/1000000;
  tam_cola = atoi(argv[5]);
  int total_items = atoi(argv[6]);

  mostrarinfo(nhilos_prod, nhilos_cons, tiempo_cons, tiempo_prod,total_items);

  int maxHilos = 0;
  if(nhilos_cons > nhilos_prod)
    maxHilos = nhilos_cons;
  else
    maxHilos = nhilos_prod;
  
  pthread_t *listProd = (pthread_t *)malloc(sizeof(pthread_t)*nhilos_prod);  
  pthread_t *listCons = (pthread_t *)malloc(sizeof(pthread_t)*nhilos_cons);

  for(int i = 0; i<maxHilos ; i++ )
  {
    if(i<nhilos_cons)
      pthread_create(&(listCons[i]),NULL,consumidor,&i); 
    if(i<nhilos_prod)     
      pthread_create(&(listProd[i]),NULL,productor,&i);
  }

  for(int i = 0; i<maxHilos ; i++ )
  {
    if(i<nhilos_cons)
      pthread_join(listCons[i],NULL); 
    if(i<nhilos_prod)     
      pthread_join(listProd[i],NULL);
  }


  return 0;
}

void mostrarinfo(int nhilos_prod, int nhilos_cons, double tiempo_cons, double tiempo_prod, int total_items){
	printf("Número de productores: %d\n", nhilos_prod);
	printf("Número de consumidores: %d\n", nhilos_cons);
	printf("Tiempo de consumo: %.2f segundos\n", tiempo_cons*1000000);
	printf("Tiempo de producción: %.2f segundos\n", tiempo_prod*1000000);
	printf("Total de ítems a producir: %d elementos.\n\n", total_items);
}
