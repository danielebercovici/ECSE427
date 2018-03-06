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
//Name: Vittorio Guerrera
//McGill ID: 260636962

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <semaphore.h>

int BUFFER_SIZE = 100; //size of queue

// A structure to represent a queue
struct Queue
{
  int front, rear, size;
  unsigned capacity;
  int *array;
};

// function to create a queue of given capacity.
// It initializes size of queue as 0
struct Queue *createQueue(unsigned capacity)
{
  struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
  queue->capacity = capacity;
  queue->front = queue->size = 0;
  queue->rear = capacity - 1; // This is important, see the enqueue
  queue->array = (int *)malloc(queue->capacity * sizeof(int));
  return queue;
}

// Queue is full when size becomes equal to the capacity
int isFull(struct Queue *queue)
{
  return ((queue->size) >= queue->capacity);
}

// Queue is empty when size is 0
int isEmpty(struct Queue *queue)
{
  return (queue->size == 0);
}

// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue *queue, int item)
{
  if (isFull(queue))
    return;
  queue->rear = (queue->rear + 1) % queue->capacity;
  queue->array[queue->rear] = item;
  queue->size = queue->size + 1;
  printf("%d enqueued to queue\n", item);
}

// Function to remove an item from queue.
// It changes front and size
int dequeue(struct Queue *queue)
{
  if (isEmpty(queue))
    return INT_MIN;
  int item = queue->array[queue->front];
  queue->front = (queue->front + 1) % queue->capacity;
  queue->size = queue->size - 1;
  return item;
}

// Function to get front of queue
int front(struct Queue *queue)
{
  if (isEmpty(queue))
    return INT_MIN;
  return queue->array[queue->front];
}

// Function to get rear of queue
int rear(struct Queue *queue)
{
  if (isEmpty(queue))
    return INT_MIN;
  return queue->array[queue->rear];
}

void print(struct Queue *queue)
{
  if (queue->size == 0)
  {
    return;
  }

  for (int i = queue->front; i < queue->front + queue->size; i++)
  {

    printf(" Element at position %d is %d \n ", i % (queue->capacity), queue->array[i % (queue->capacity)]);
  }
}

struct Queue *queue;
sem_t mutex;
sem_t full;
sem_t empty;
int passengerNumber = 0;

/*Producer Function: Simulates an Airplane arriving and dumping 5-10 passengers to the taxi platform */
void *FnAirplane(void *cl_id)
{
  sleep(1);
  int airplaneID = *(int *)cl_id;
  int airplanePassengers = 5 + (rand() % 5);
  printf("Airplane %d arrives with %d passengers. \n", airplaneID, airplanePassengers);

  for (int i = 0; i < airplanePassengers; i++)
  {
    int passenger;
    passenger = 1000000 + (1000 * airplaneID) + passengerNumber;
    sem_wait(&empty);
    sem_wait(&mutex);
    if (isFull(queue))
    {
      printf("Platform is full. Rest of passengers of plane %d take the bus. \n", airplaneID);
      break;
    }
    else
    {
      enqueue(queue, passenger);
      passengerNumber++;
    }
    sem_post(&mutex);
    sem_post(&full);
  }
}

/* Consumer Function: simulates a taxi that takes n time to take a passenger home and come back to the airport */
void *FnTaxi(void *pr_id)
{
  while (1)
  {
    int taxiID = *(int *)pr_id;
    printf("Taxi driver %d arrives. \n", *(int *)pr_id);

    if (isEmpty(queue) == 1)
    {
      printf("Taxi driver %d waits for passengers to enter the platform. \n", taxiID);
    }
    int passenger;
    sem_wait(&full);
    sem_wait(&mutex);
    passenger = front(queue);
    sem_post(&mutex);
    sem_post(&empty);
    dequeue(queue);
    printf("Taxi driver %d picked up client %d from the platform. \n", taxiID, passenger);
    sleep(10 + (rand() % 21));
  }
}

int main(int argc, char *argv[])
{

  int num_airplanes;
  int num_taxis;

  num_airplanes = atoi(argv[1]);
  num_taxis = atoi(argv[2]);

  printf("You entered: %d airplanes per hour\n", num_airplanes);
  printf("You entered: %d taxis\n", num_taxis);

  //initialize queue
  queue = createQueue(BUFFER_SIZE);

  //declare arrays of threads and initialize semaphore(s)
  pthread_t airplanes[num_airplanes];
  pthread_t taxis[num_taxis];
  sem_init(&mutex, 0, 1);
  sem_init(&full, 0, 0);
  sem_init(&empty, 0, BUFFER_SIZE);

  //create arrays of integer pointers to ids for taxi / airplane threads
  int *taxi_ids[num_taxis];
  int *airplane_ids[num_airplanes];

  //create threads for airplanes
  for (int i = 0; i < num_airplanes; i++)
  {
    printf("Creating airplane Thread %d \n", i);
    int *val = malloc(sizeof(int));
    *val = i;
    airplane_ids[i] = val;
    pthread_create(&airplanes[i], NULL, FnAirplane, airplane_ids[i]);
  }

  //create threads for taxis
  for (int i = 0; i < num_taxis; i++)
  {
    int *val = malloc(sizeof(int));
    *val = i;
    taxi_ids[i] = val;
    pthread_create(&taxis[i], NULL, FnTaxi, taxi_ids[i]);
  }

  pthread_exit(NULL);
}
