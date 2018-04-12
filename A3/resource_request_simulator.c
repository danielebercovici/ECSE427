#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>


//Declare variables
int *avail; //Available resources
int **max;//Maxij
int numProcesses, numResources;
int **need; //how many resources each process needs to finish
int **hold; //how many resources each process is holding

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
                    //must wait --------------------------------------------- return 0;??
                    return 0;
                    //bankers_algorithm(pr_id,request_vector);
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
                    printf("process [%d], resource [%d] new NEED %d \n",i,j, need[i][j]);
                }
            }
            if(isSafe()){ //grant
                printf("GRANTED \n");
                sleep(3); //---------------- is this here???, WHERE DO WE RELIQUISH RESOURCES?
                //reliquish??
                for(int i = 0; i<numProcesses;i++){
                    for(int j =0; j<numResources;j++){
                        avail[j]=avail[j]+hold[i][j];
                        hold[i][j]=0;
                    }
                }
                return 1;
            }
            else{//cancel allocation------------------------ 
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
                //must wait
                //block process until another process finishes --------------------
                //bankers_algorithm(pr_id,request_vector);
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
    int *request_vector;
    request_vector = malloc(numResources * sizeof(int));
    request_simulator((int)pr_id,request_vector);
    //calls banker algo
    bankers_algorithm((int)pr_id,request_vector); //--------------big while(bankers){} ???? or recursive bankers?
    //call bankers to see if allocation of request vector is safe or not
    //if not safe: process continuously calls bankers unitl allocation is safe
    //if safe: check if process can termiante and if so, free all resources held by process. If you cant terminate, then sleep for 3 seconds and make another request
    //only gets resource if allocation is safe
    //releases resources if it gets its needs met

    //check for remaining requests
    bool remaining;
    for(int j=0;j<numResources;j++){ 
        if(need[(int)pr_id][j]!=0){//remaining requests
            remaining=true;
        }
    }
    while(remaining){
        remaining=false;
        request_simulator((int)pr_id,request_vector); 
        bankers_algorithm((int)pr_id,request_vector);
        for(int j=0;j<numResources;j++){ 
            if(need[(int)pr_id][j]!=0){//remaining requests
                remaining=true;
            }
        }
        sleep(3);
    }
    // //check for remaining requests
    // for(int j=0;j<numResources;j++){
        
    //     if(need[(int)pr_id][j]!=0){//remaining requests
    //         sleep(3);
    //         request_simulator((int)pr_id,request_vector); //----------------is this suppose to be here? call bankers again? like while still remaining repeat???
    //         bankers_algorithm((int)pr_id,request_vector);
    //     }
    // }

    //terminate process -------------------------free all resources here???
    pthread_join(pr_id,NULL);

}

/*
Simulates a fault occuring on the system.
*/
void* fault_simulator(void* pr_id){
    //thread running in background removing resources with probability described in the spec
    //rand() % (max_number + 1 - minimum_number) + minimum_number --------------------------------how can it be 50% probability and uniform????
    int resource = rand()%((numResources-1)+1);
    avail[resource]-=1;
}

/*
Checks for deadlock
*/
void* deadlock_checker(){
    bool deadlock = false;
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
        exit(1);//this how you exit???
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
    // pthread_t faulty;
    // while(1){
    //     //pthread_create(&faulty,NULL,fault_simulator,pr_id);
    //     sleep(10);
    // }
    

    // //create a thread to check for deadlock (deadlock_checker)  
    // pthread_t deadlock;
    //  while(1){
    //     pthread_create(&deadlock,NULL,deadlock_checker,NULL);
    //     sleep(10);
    // }
    pthread_exit(NULL);
    return 0;

}