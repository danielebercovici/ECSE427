/*
 ----------------- COMP 310/ECSE 427 Winter 2018 -----------------
 Dimitri Gallos
 Assignment 2 skeleton
 
 -----------------------------------------------------------------
 I declare that the awesomeness below is a genuine piece of work
 and falls under the McGill code of conduct, to the best of my knowledge.
 -----------------------------------------------------------------
 */

//Please enter your name and McGill ID below
//Name: Daniele Bercovici
//McGill ID: 260627845

 

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <semaphore.h>
#include <string.h>


int BUFFER_SIZE = 100; //size of queue
//initialize semaphores
sem_t mutex;
sem_t empty;
sem_t full;


// A structure to represent a queue
struct Queue
{
    int front, rear, size;
    unsigned capacity;
    int* array;
};
 
// function to create a queue of given capacity. 
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->array = (int*) malloc(queue->capacity * sizeof(int));
    return queue;
}
 
// Queue is full when size becomes equal to the capacity 
int isFull(struct Queue* queue)
{
    return ((queue->size ) >= queue->capacity);
}
 
// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}
 
// Function to add an item to the queue.  
// It changes rear and size
void enqueue(struct Queue* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    //printf("%d enqueued to queue\n", item);
}

// Function to remove an item from queue. 
// It changes front and size
int dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to get front of queue
int front(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}
 
// Function to get rear of queue
int rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}

void print(struct Queue* queue){
    if (queue->size == 0){
        return;
    }
    
    for (int i = queue->front; i < queue->front +queue->size; i++){
        
        printf(" Element at position %d is %d \n ", i % (queue->capacity ), queue->array[i % (queue->capacity)]);
    }
    
}

struct Queue* queue;

/*PRODUCER Function: Simulates an Airplane arriving and dumping 5-10 passengers to the taxi platform */
void *FnAirplane(void* cl_id)
{
    
    while(1){
        int numpass = 5+rand()%(6);//number of passengers on plane cl_id
        printf("Airplane %d arrives with %d passengers\n",(int)cl_id,numpass);
 
        int id; //passenger id

        for (int k = 0; k<numpass; k++){
            //format passenger id 1ZZZYYY
            id=1000000 + (1000 * (int)cl_id) + k;

            sem_wait(&empty);
            sem_wait(&mutex); 
            if (isFull(queue)){
                printf("Platform is full: Rest of passengers of plane %d take bus\n",(int)cl_id);
                break;
            }
            else{
                //enqueue passenger
                enqueue(queue,id);
                printf("Passenger %d of airplane %d arrives to platform\n",id, (int)cl_id);
            }
            sem_post(&mutex); 
            sem_post(&full);
        }

    sleep(1);
    }

}

/*CONSUMER Function: simulates a taxi that takes n time to take a passenger home and come back to the airport */
void *FnTaxi(void* pr_id)
{
    while(1){
    printf("Taxi driver %d arrives \n", (int)pr_id);

        //check if empty wait
        if(isEmpty(queue)){
            printf("Taxi driver %d waits for passengers to enter the platform\n", (int)pr_id);
            //wait
            sem_wait(&full);
        }
        sem_wait(&mutex);
        //taxi driver leaves
        int id = dequeue(queue);
        printf("Taxi driver %d picked up client %d from platform\n", (int)pr_id, id);
         
        sem_post(&mutex);
        sem_post(&empty);

        //each taxi rand() from 10-30 min to destination
        float time = (10+rand()%(21))/60.0; //converted to sec
        sleep(time); //driving passengers to destinations
    }
    
  
}


int main(int argc, char *argv[])
{

  int num_airplanes;
  int num_taxis;

  num_airplanes=atoi(argv[1]);
  num_taxis=atoi(argv[2]);
  
  printf("You entered: %d airplanes per hour\n",num_airplanes);
  printf("You entered: %d taxis\n", num_taxis);
  
  
  //initialize queue
  queue = createQueue(BUFFER_SIZE); 

  
  //declare arrays of threads and initialize semaphore(s)
    pthread_t airplanes;
    pthread_t taxis; 
    
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE); //all taxis empty
    sem_init(&empty, 0, 0); //no taxis full

  //create arrays of integer pointers to ids for taxi / airplane threads
  int *taxi_ids[num_taxis];
  int *airplane_ids[num_airplanes];

  //create threads for airplanes
 for (int i=0 ; i< num_airplanes; i++){
    printf("Creating airplane thread %d\n", i);
    pthread_create(&airplanes, NULL, FnAirplane, i);
  }

  //create threads for taxis
 for (int j=0 ; j< num_taxis; j++){
    pthread_create(&taxis, NULL, FnTaxi, j);
  }
  
  pthread_exit(NULL);
}

