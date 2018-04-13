#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>


//Declare variables
int *avail; //Available resources
int **max;//Maxij
int numProcesses, numResources;
int **need; //how many resources each process needs to finish
int **hold; //how many resources each process is holding

sem_t mutex;

/*
Simulates resource requests by processes 
*/
void request_simulator(int pr_id, int* request_vector){
//call to get a request vector REQj
    for(int j =0; j<numResources;j++){
        //rand() % (max_number + 1 - minimum_number) + minimum_number
        printf("process [%d] resource [%d] NEED: %d \n",pr_id,j, need[pr_id][j]);
        request_vector[j]=rand()%(need[pr_id][j]+1);
        printf("process [%d] resource [%d] REQUEST: %d \n",pr_id, j,request_vector[j]);
    }
}

/*
Implementation of isSafe() as described in the slides
*/
int isSafe(){
    //initalize work
    int *work;
    int isSafe = 1;

    work = malloc(numResources * sizeof(int));
    for(int j =0;j<numResources;j++){
        work[j]=avail[j];
    }
    bool *finish;
    finish = malloc(numProcesses * sizeof(int));
    for(int i=0;i<numProcesses;i++){
        finish[i]=false;
    }

    for(int i = 0; i<numProcesses;i++){
        for(int j =0; j<numResources;j++){
            if(finish[i]==false && need[i][j]<=work[j]){
                work[j]=work[j]-hold[i][j];
                finish[i]=true;
            }
        }
    } 
   
    for(int i=0;i<numProcesses;i++){
        if(finish[i] == true){}
        else{
            isSafe=0; //not safe
        }  
    }
    return isSafe;

}
/*
Implementation of Bankers Algorithm as described in the slides
returns 1 if safe allocation 0 if not safe
*/
int bankers_algorithm(int pr_id, int* request_vector){
//deadlock avoidance bankers algo

for(int i = 0; i<numProcesses;i++){
    for(int j =0; j<numResources;j++){
        if(request_vector[j]>need[i][j]){
            //impossible
            exit(1);
        }
        else{
            //check if request amount is available
            for(int j=0;j<numResources;j++){
                if(request_vector[j]>avail[j]){
                    return 0;
                }
            }
            //provisional allocation
            for(int j=0;j<numResources;j++){
                avail[j]=avail[j]-request_vector[j];
            }
            for(int i = 0; i<numProcesses;i++){
                for(int j =0; j<numResources;j++){
                    hold[i][j]=hold[i][j]+request_vector[j];
                    need[i][j]=need[i][j]-request_vector[j];
                }
            }
            if(isSafe()){ //grant
                printf("GRANTED \n");
                sleep(3); 
                //reliquish --------------------------------------------------_WHERE TO RELINQUISH?
                for(int i = 0; i<numProcesses;i++){
                    for(int j =0; j<numResources;j++){
                        avail[j]=avail[j]+hold[i][j];
                        hold[i][j]=0;
                    }
                }
                return 1;
            }
            else{//cancel allocation
                for(int j=0;j<numResources;j++){
                    avail[j]=avail[j]+request_vector[j];
                }
                for(int i = 0; i<numProcesses;i++){
                    for(int j =0; j<numResources;j++){
                        hold[i][j]=hold[i][j]-request_vector[j];
                        need[i][j]=need[i][j]+request_vector[j];
                        printf("Cancel allocation \n");
                    }
                }
                return 0;
            }

        }
    }
}
    return 0;
}

/*
Simulates processes running on the system.
*/
void* process_simulator(void* pr_id){

    //thread simulating process
    //gets random request vector
    
    while(1){
        int *request_vector;
        request_vector = malloc(numResources * sizeof(int));
        request_simulator((int)pr_id,request_vector);

        //calls banker algo
        sem_wait(&mutex);
        while(!bankers_algorithm((int)pr_id,request_vector)){} 
        sem_post(&mutex);
        
        sleep(3);
        bool remaining = false;
        for(int j=0;j<numResources;j++){ 
            if(need[(int)pr_id][j]!=0){//remaining requests
                remaining=true;
            }
        }
        if(!remaining){
            break;
        }

    }
    printf("TERMINATING\n");
    //terminate process ----------------------------------------
    pthread_join(pr_id,NULL);
    exit(1);

}

/*
Simulates a fault occuring on the system.
*/
void* fault_simulator(){
    //thread running in background removing resources with probability described in the spec
    //rand() % (max_number + 1 - minimum_number) + minimum_number --------------------------------how can it be 50% probability and uniform????
    while(1){
        int resource = rand()%((numResources-1)+1);
        avail[resource]-=1;
        sleep(10);

    }

    
}

/*
Checks for deadlock
*/
void* deadlock_checker(){
    while(1){
        bool deadlock = false;
        sleep(10);
        //periodically run this
        //check if deadlock has occured due to resource fault
        //process needs checked against current available resource in system
        for(int i = 0; i<numProcesses;i++){
            for(int j =0; j<numResources;j++){
                if(need[i][j]>avail[j]){}
                else{
                    deadlock=true;
                }
            }
        }
        if(deadlock){
            printf("Deadlock will occur as process request more resources, exiting");
            exit(1);//this how you exit???________________________________________________________
        }
    }
    

}

int main()
{
    sem_init(&mutex, 0, 1);

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
    //initialize hold and need
    hold=(int **)malloc(sizeof(int *) * numProcesses);
    for (int k = 0; k < numProcesses; k++) {
        hold[k] = (int *) malloc(sizeof(int) * numResources);
    }
    need=(int **)malloc(sizeof(int *) * numProcesses);
    for (int k = 0; k < numProcesses; k++) {
        need[k] = (int *) malloc(sizeof(int) * numResources);
    }
    for (int i = 0; i < numProcesses; i++){
        for(int j=0; j< numResources; j++){
            hold[i][j]=0;
            need[i][j]=max[i][j];
        }  
    }
    
    //create threads simulating processes (process_simulator)
    int *p_ids[numProcesses];

    pthread_t p_id[numProcesses];
    for (int j=0 ; j< numProcesses; j++){
        int *temp = j;
        p_ids[j] = temp;
        printf("process %d created\n",j);
        pthread_create(&p_id[j], NULL, process_simulator,p_ids[j]);
        
    }

  //create a thread that takes away resources from the available pool (fault_simulator)  ---------------------------why does adding this? make scanf infinite????
    pthread_t faulty;
    pthread_create(&faulty,NULL,fault_simulator,NULL);
 
    

    //create a thread to check for deadlock (deadlock_checker)  
    pthread_t deadlock;
    pthread_create(&deadlock,NULL,deadlock_checker,NULL);

    
    pthread_exit(NULL);
    return 0;

}