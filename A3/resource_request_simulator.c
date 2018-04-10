#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

/*
Simulates resource requests by processes 
*/
void request_simulator(int pr_id, int* request_vector){
//call to get a request vector REQj
//call bankers to see if allocation of request vector is safe or not
//if not safe: process continuously calls bankers unitl allocation is safe
//if safe: check if process can termiante and if so, free all resources hekld by process. If you cant terminate, then sleep for 3 seconds and make another request
}
/*
Implementation of isSafe() as described in the slides
*/
int isSafe(){
    //initalize work
    int *work;
    bool *finish;
    int isSafe = 0;
    return isSafe;

}
/*
Implementation of Bankers Algorithm as described in the slides
returns 1 if safe allocation 0 if not safe
*/
int bankers_algorithm(int pr_id, int* request_vector){
//deadlock avoidance bankers algo
//follow class slides

//Holdij initialized to 0, how many resources each process is holding
//Needij how many resources each process needs to finish needij = maxij-holdij
//REQj request vector of resouces "random value between 0 and the aount of resources needed to reminate"
    return 0;

}

/*
Simulates processes running on the system.

*/
void* process_simulator(void* pr_id){
//thread simulating process
//gets random request vector
//calls banker algo
//only gets resource if allocation is safe
//releases resources if it gets its needs met
}

/*
Simulates a fault occuring on the system.

*/
void* fault_simulator(void* pr_id){
//thread running in background removing resources with probability described in the spec
}

/*
Checks for deadlock
*/
void* deadlock_checker(){
//periodically run this
//check if deadlock has occured due to resource fault
//deadlock if Need>Avail for all processes for at least 1 resource type
//process needs checked against current available resource in system
}
void print_array(const int *A, size_t width, size_t height)
{
  for(size_t i = 0; i < height; ++i)
  {
    for(size_t j = 0; j < width; ++j)
      printf("A[%zu][%zu] = %d\n", i, j, A[i * width + j]);
  }
}

int main()
{

    //Initialize all inputs to banker's algorithm
    int numProcesses, numResources, *amountResources, *maxResourceClaim;
    printf("Enter the number of processes : ");
    scanf("%d", &numProcesses);

    printf("Enter the number of district resources : ");
    scanf("%d", &numResources);

    printf("Enter the amount of each resource in the system : "); //Avail, be careful shared mem
    amountResources = malloc(numResources * sizeof(int));
    for (int i = 0; i < numResources; i++)
    {
        scanf("%d", &amountResources[i]);
    } 

    int *max [ numProcesses ][ numResources ];//Maxij
    for (int i = 0; i < numProcesses; i++)
    {
        for(int j=0; j< numResources; j++){
            printf("Enter the max resource claim for process %i and resource %i: \n", i,j);  
            scanf("%d", &max[i][j]);
        }  
    } 
    
    //create threads simulating processes (process_simulator)
    int *requestvector;//??????? REQj?
    pthread_t process;
    pthread_create(&process,NULL,process_simulator,requestvector);

    //create a thread that takes away resources from the available pool (fault_simulator)
    pthread_t resource;
    pthread_create(&resource,NULL,fault_simulator,requestvector);

    //create a thread to check for deadlock (deadlock_checker)
    pthread_t deadlock;
    pthread_create(&deadlock,NULL,deadlock_checker,NULL);
    return 0;
}