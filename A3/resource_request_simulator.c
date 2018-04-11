#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>



int *avail; //Available resources
int **max;//Maxij
int numProcesses, numResources;
/*
Simulates resource requests by processes 
*/
void request_simulator(int pr_id, int* request_vector){
//call to get a request vector REQj
//requests ij instances of each resource j where i is randomly selected as a value between 0 and amount of remaining resources
for(int i =0; i<request_vector;i++){

}
//check if able to grant safe state
//if so grant
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
int *request_vector;
request_vector = malloc(numResources * sizeof(int));
request_simulator((int)pr_id,request_vector);
//calls banker algo
bankers_algorithm((int)pr_id,request_vector);
//call bankers to see if allocation of request vector is safe or not
//if not safe: process continuously calls bankers unitl allocation is safe
//if safe: check if process can termiante and if so, free all resources hekld by process. If you cant terminate, then sleep for 3 seconds and make another request
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
bool deadlock = false;
//periodically run this
//check if deadlock has occured due to resource fault
//deadlock if Need>Avail for ALL processes for at least 1 resource type
//process needs checked against current available resource in system
if(deadlock){
    printf("Deadlock will occur as process request more resources, exiting");
    exit(1);//?????? HOW EXIT???
}
}
int main()
{

    //Initialize all inputs to banker's algorithm
    
    printf("Enter the number of processes : ");
    scanf("%d", &numProcesses);

    printf("Enter the number of district resources : ");
    scanf("%d", &numResources);

    //Avail, be careful shared mem
    avail = malloc(numResources * sizeof(int));
    for (int i = 0; i < numResources; i++){
        printf("Enter the amount of %d resource in the system : ", i);
        scanf("%d", &avail[i]);
    } 

    max=(int **)malloc(sizeof(int *) * numProcesses);
    for (int k = 0; k < numProcesses; k++) {
        max[k] = (int *) malloc(sizeof(int) * numResources);
    }

    for (int i = 0; i < numProcesses; i++){
        for(int j=0; j< numResources; j++){
            printf("Enter the max resource claim for process %i and resource %i: ", i,j);  
            scanf("%d", &max[i][j]);
        }  
    }
    
    //create threads simulating processes (process_simulator)
    int *p_ids[numProcesses];
    pthread_t p_id[numProcesses];
    for (int j=0 ; j< numProcesses; j++){
        int *temp = j;
        p_ids[j] = temp;
        pthread_create(&p_id[j], NULL, process_simulator,p_ids[j]);
    }

    //create a thread that takes away resources from the available pool (fault_simulator)  //QUESTION 2????? 
    // pthread_t resource;
    // while(1){
    //     //HOW DO YOU DO UNIFORM PROBABILITY 50% for pr_id
    //     //pthread_create(&resource,NULL,fault_simulator,pr_id);
    //     sleep(10);
    // }
    

    // //create a thread to check for deadlock (deadlock_checker)  //QUESTION 2???????? 
    // pthread_t deadlock;
    //  while(1){
    //     pthread_create(&deadlock,NULL,deadlock_checker,NULL);
    //     sleep(10);
    // }
    return 0;

}