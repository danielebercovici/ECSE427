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
//Name: <your name>
//McGill ID: <magic number>

 

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <semaphore.h>
#include <string.h>


int BUFFER_SIZE = 100; //size of queue



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
    printf("%d enqueued to queue\n", item);
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
    
    //while(1){
        //produce passenger (w/ planenum+pid) clear every hour
        int numpass = 5+rand()%(6);//number of passengers on plane cl_id
        printf("Airplane %d arrives with %d passengers\n",cl_id,numpass);
        char id1[3];
        char id2[3];
        for (int k = 0; k<numpass; k++){
            sprintf(id1,"%d", (int)cl_id);
            sprintf(id2,"%d", k);
            printf("Passenger %03s%03s of airplane %d arrives to platform\n",id1,id2, (int)cl_id);
        }
        //wait(&empty);
        //wait(&mutex); 

        //check empty or full queue
        if (isFull(queue)){
            printf("Platform is full: Rest of passengers of plane %d take bus",(int)cl_id);
            //ignore other passengers;????? -------how to keep track of passengers>????? Save id in the queue??? but how to save id??
        }
        else{
            //enqueue passenger
        }

        //signal(&mutex); 
        //signal(&full);
       
    }

//}


/*CONSUMER Function: simulates a taxi that takes n time to take a passenger home and come back to the airport */
void *FnTaxi(void* pr_id)
{
    //while(1){
        //wait(&full);
        //wait(&mutex);
        //check if empty wait
        if(isEmpty(queue)){
            //wait
         }
        else{
            dequeue(queue);
            //log passenger and taxi id
            //printf("Taxi driver %d picked up client %d from platform", (int)pr_id, ??);
         }
        //signal(&mutex);
        //signal(&empty);

        //each taxi rand() from 10-30 min to destination
        float time = (10+rand()%(21))/60.0; //converted to sec
        sleep(time); //drivingpassengers to destinations
        //-------COME BACK TO AIRPORT??
    //}
    
  
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
  queue = createQueue(BUFFER_SIZE); //max 100 passengers waiitng for taxi

  
  //declare arrays of threads and initialize semaphore(s) ---?????
    pthread_t airplanes;
    pthread_t taxis; 
    sem_t mutex;
    sem_init(&mutex, 0, 1);
    sem_t empty;
    sem_init(&empty, 0, BUFFER_SIZE); //All taxis empty
    sem_t full;
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

 for (int j=0 ; j< num_airplanes; j++){
    printf("taxi driver %d arrives \n", j);
    pthread_create(&taxis, NULL, FnTaxi, j);
  }
  
  pthread_exit(NULL);
}
