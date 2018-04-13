#include <stdio.h>  //for printf and scanf
#include <stdlib.h> //for malloc

#define LOW 0
#define HIGH 199
#define START 53

//compare function for qsort
//you might have to sort the request array
//use the qsort function 
// an argument to qsort function is a function that compares 2 quantities
//use this there.
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

//function to swap 2 integers
void swap(int *a, int *b)
{
    if (*a != *b)
    {
        *a = (*a ^ *b);
        *b = (*a ^ *b);
        *a = (*a ^ *b);
        return;
    }
}

//Prints the sequence and the performance metric
void printSeqNPerformance(int *request, int numRequest)
{
    int i, last, acc = 0;
    last = START;
    printf("\n");
    printf("%d", START);
    for (i = 0; i < numRequest; i++)
    {
        printf(" -> %d", request[i]);
        acc += abs(last - request[i]);
        last = request[i];
    }
    printf("\nPerformance : %d\n", acc);
    return;
}

//access the disk location in FCFS
void accessFCFS(int *request, int numRequest)
{
    //simplest part of assignment
    printf("\n----------------\n");
    printf("FCFS :");
    printSeqNPerformance(request, numRequest);
    printf("----------------\n");
    return;
}

//access the disk location in SSTF
void accessSSTF(int *request, int numRequest)
{
    //shortest service time first (closest)
    //START is starting posiiton
    for (int i=0; i<numRequest; i++){
        for(int j = 0; j<numRequest; j++){
            // if(abs(START-request[j])==abs(START-request[i])&& i!=j){//TODO: tie breaker goes to number that came first ----------------------------------
            //     //printf("request i: [%i] %i and request j: [%i] %i\n",request[i],i,request[j],j);

            // }
            if (abs(START-request[j])>abs(START-request[i])){ 
                int tmp=request[i];
                request[i]=request[j];
                request[j]=tmp;
            }
        }   
    }

    printf("\n----------------\n");
    printf("SSTF :");
    printSeqNPerformance(request, numRequest);
    printf("----------------\n");
    return;
}

//access the disk location in SCAN
void accessSCAN(int *request, int numRequest)
{

    //int next = request[0]; //the next value in the order
    //int nextvalue = START-request[0]; //value of the difference of the next one

    int *newRequest = malloc(numRequest * sizeof(int)); //new order
    int newCnt=numRequest;
    //sort
    for (int i=0; i<numRequest; i++){
        for(int j = 0; j<numRequest; j++){

            if (request[j]>request[i]){
                int tmp=request[i];
                request[i]=request[j];
                request[j]=tmp;
            }
        } 
    }
    int direction=0; //0 for up, 1 for down
    //go in direction of closest end
    if(START-LOW < HIGH-START){
        //go down
        direction = 1;
    }
    // //find closest next job and remember the direction
    // for (int i=0; i<numRequest; i++){
    //     //find the closest one to start
    //     int value= START-request[i];
    //     if(abs(value)<abs(nextvalue)){ 
    //         next = request[i];
    //         nextvalue=value;
    //     } 
    // }
    
    //find position
    int m=0; //location of first value to explore
    for(int k=0; k<numRequest; k++){
        if(direction == 0){//up
            if(request[k]>=START){
                newRequest[0]=request[k];
                break;
            }
        }
        else if(direction==1){
            if(request[k]>=START){ //down
                m--;
                newRequest[0]=request[k-1];
                break;
            } 
        }    
        m++;
    }

    if(direction==0){//go up
       for(int i=1; i < numRequest-m;i++){
           newRequest[i]=request[m+i];
       }
       if(m!=0){ //need to go back to start
            newRequest[numRequest-m]=HIGH; //go to end of track
            newCnt++;
            int p=m-1;
            for(int l = numRequest-m+1; l<= numRequest; l++){
                newRequest[l] = request[p];
                p--;
            }

       } 
    }
    if(direction==1){//go down
    if(m == (numRequest)){ //none in up direction no need to go to end
        for (int n =0; n<numRequest;n++){
            newRequest[n]=request[numRequest-n-1];  
        }
    }
    else{//needs to go back
        for (int n =1; n<=m;n++){
            newRequest[n]=request[m-n];  
        }
            newRequest[m+1]=LOW; //go to end of track
            newCnt++;
            int p=1;
            for(int l = m+2; l<= numRequest; l++){
                newRequest[l] = request[m+p];
                p++;
            }
        
    }
  
    }
        

    // if(nextvalue<0){//go up
    //     for (int n =1; n<numRequest-m;n++){
    //         newRequest[n]=request[m+n];
            
    //     }
    //     if(m!=0){//needs to go back
    //         newRequest[numRequest-m]=HIGH; //go to end of track
    //         newCnt++;
    //         int p=m-1;
    //         for(int l = numRequest-m+1; l<= numRequest; l++){
    //             newRequest[l] = request[p];
    //             p--;
    //         }
    //     }
    // }
    // if(nextvalue>=0){//go down
    //     for (int n =1; n<=m;n++){
    //         newRequest[n]=request[m-n];  
    //     }
    //     if(m != (numRequest-1)){//needs to go back
    //         newRequest[m+1]=LOW; //go to end of track
    //         newCnt++;
    //         int p=1;
    //         for(int l = m+2; l<= numRequest; l++){
    //             newRequest[l] = request[m+p];
    //             p++;
    //         }
    //     }
    //}
    
    printf("\n----------------\n");
    printf("SCAN :");
    printSeqNPerformance(newRequest, newCnt);
    printf("----------------\n");
    return;
}

//access the disk location in CSCAN
void accessCSCAN(int *request, int numRequest)
{
    int *newRequest = malloc(numRequest * sizeof(int)); //new order
    int newCnt=numRequest;

    //sort
    for (int i=0; i<numRequest; i++){
        for(int j = 0; j<numRequest; j++){

            if (request[j]>request[i]){
                int tmp=request[i];
                request[i]=request[j];
                request[j]=tmp;
            }
        } 
    }
    int direction=0; //0 for up, 1 for down
    //go in direction of closest end
    if(START-LOW < HIGH-START){
        //go down
        direction = 1;
    }    
    //find position
    int m=0; //location of first value to explore
    
    for(int k=0; k<numRequest; k++){
        if(direction == 0){//up
            if(request[k]>=START){
                newRequest[0]=request[k];
                break;
            }
        }
        else if(direction==1){
            if(request[k]>=START){ //down
                newRequest[0]=request[k-1];
                m--;
                break;
            }
        }    
        m++;
    }
if (direction == 0){ //up

    for (int n =1; n<numRequest-m;n++){
        newRequest[n]=request[m+n];
    }
    if(m!=0){//needs to go back
        newRequest[numRequest-m]=HIGH; //go to end of track
        newCnt++;
        newRequest[numRequest-m+1]=LOW; //fast return
        newCnt++;
        
        int p=0;
        for(int l = numRequest-m+2; l<= numRequest+1; l++){
            newRequest[l] = request[p];
            p++;
        }
    }
}
if(direction ==1){ //down
    if(m == (numRequest)){ //none in up direction
        for (int n =0; n<numRequest;n++){
            newRequest[n]=request[numRequest-n-1];  
        }
    }
    else if(m != (numRequest))//needs to go back
    for (int n =1; n<=m;n++){
            newRequest[n]=request[m-n];  
    }

    newRequest[m+1]=LOW; //go to end of track
    newCnt++;
    newRequest[m+2]=HIGH;//fast return
    newCnt++;

    int p=1;
    for(int l = m+3; l<= numRequest+1; l++){
        newRequest[l] = request[numRequest-p];
        p++;
    }


}
//     int next = 0; //the next value in the order
//     int nextvalue = 0; //value of the difference of the next one
//     //find an initial acsending value
//     for(int x =0; x<numRequest;x++){
//         if(START-request[x] <0){
//             next = request[x];
//             nextvalue=START-request[x]; 
//             break;
//         }
//     }

//     int *newRequest = malloc(numRequest * sizeof(int)); //new order
//     int newCnt=numRequest;
    
//     //Assumes only goes forward (ascending) then fast return
//     //sort
//     for (int i=0; i<numRequest; i++){
//         for(int j = 0; j<numRequest; j++){

//             if (request[j]>request[i]){
//                 int tmp=request[i];
//                 request[i]=request[j];
//                 request[j]=tmp;
//             }
//         } 
//     }

//     //find closest next job and that is ascending
//     for (int i=0; i<numRequest; i++){
//         if(next ==0){
//             break;
//         }
//         //find the closest one to start
//         int value= START-request[i];
//         if(abs(value)<abs(nextvalue)){ 
//             next = request[i];
//             nextvalue=value;
//         } 
//     }

//     int done=0;//indicates whether its done when there is no acsending job from start
//     //find position
//     int m=0; //location of first value to explore
//     for(int k=0; k<numRequest; k++){
//         if(next ==0){
//             newRequest[0]=HIGH;
//             newRequest[1]=LOW;
//             newCnt+=2;
//             //go in order
//             for(int n = 2; n<numRequest+2; n++){
//                 newRequest[n]=request[n-2];
//             }
//             done =1;    
//             break;  
//         }
//         if(request[k]==next){
//             newRequest[0]=request[k];
//             break;
//         }
//         m++;
        
//     }

//   if(!done){
//     for (int n =1; n<numRequest-m;n++){
//         newRequest[n]=request[m+n];
//     }
//     if(m!=0){//needs to go back
//         newRequest[numRequest-m]=HIGH; //go to end of track
//         newCnt++;
//         newRequest[numRequest-m+1]=LOW; //fast return
//         newCnt++;
        
//         int p=0;
//         for(int l = numRequest-m+2; l<= numRequest+1; l++){
//             newRequest[l] = request[p];
//             p++;
//         }
//     }
//   }



    printf("\n----------------\n");
    printf("CSCAN :");
    printSeqNPerformance(newRequest, newCnt);
    printf("----------------\n");
    return;
}

//access the disk location in LOOK
void accessLOOK(int *request, int numRequest)
{
    //Similar to SCAN except no going to end of track

    //int next = request[0]; 
    //int nextvalue = START-request[0]; 

    int *newRequest = malloc(numRequest * sizeof(int)); //new order
    int newCnt=numRequest;
    //sort
    for (int i=0; i<numRequest; i++){
        for(int j = 0; j<numRequest; j++){

            if (request[j]>request[i]){
                int tmp=request[i];
                request[i]=request[j];
                request[j]=tmp;
            }
        } 
    }
    int direction=0; //0 for up, 1 for down
        //go in direction of closest end
        if(START-LOW < HIGH-START){
            direction = 1;
        } 
    int m=0; //location of first value to explore
    for(int k=0; k<numRequest; k++){
        if(direction == 0){//up
            if(request[k]>=START){
                newRequest[0]=request[k];
                break;
            }
        }
        else if(direction==1){
            if(request[k]>=START){ //down
                m--;
                newRequest[0]=request[k-1];
                break;
            } 
        }    
        m++;
    }

    if(direction==0){//go up
       for(int i=1; i < numRequest-m;i++){
           newRequest[i]=request[m+i];
       }
       if(m!=0){ //need to go back to start
            int p=m-1;
            for(int l = numRequest-m; l<= numRequest; l++){
                newRequest[l] = request[p];
                p--;
            }

       } 
    }
    if(direction==1){//go down
    if(m == (numRequest)){ //none in up direction no need to go to end
        for (int n =0; n<numRequest;n++){
            newRequest[n]=request[numRequest-n-1];  
        }
    }
    else{//needs to go back
        for (int n =1; n<=m;n++){
            newRequest[n]=request[m-n];  
        }
            int p=1;
            for(int l = m+1; l<= numRequest; l++){
                newRequest[l] = request[m+p];
                p++;
            }  
    }
  
    }  

    // for (int i=0; i<numRequest; i++){
    //     //find the closest one to start
    //     int value= START-request[i];
    //     if(abs(value)<abs(nextvalue)){ 
    //         next = request[i];
    //         nextvalue=value;
    //     } 
    // }

    // //find position
    // int m=0; //location of first value to explore
    // for(int k=0; k<numRequest; k++){
    //     if(request[k]==next){
    //         newRequest[0]=request[k];
    //         break;
    //     }
    //     m++;
    // }
    // if(nextvalue<0){//go up
    //     for (int n =1; n<numRequest-m;n++){
    //         newRequest[n]=request[m+n];
            
    //     }
    //     if(m!=0){//needs to go back
          
    //         int p=m-1;
    //         for(int l = numRequest-m; l<= numRequest; l++){
    //             newRequest[l] = request[p];
    //             p--;
    //         }
    //     }
    // }
    // if(nextvalue>=0){//go down
    //     for (int n =1; n<=m;n++){
    //         newRequest[n]=request[m-n];  
    //     }
    //     if(m != (numRequest-1)){//needs to go back
  
    //         int p=1;
    //         for(int l = m+1; l<= numRequest; l++){
    //             newRequest[l] = request[m+p];
    //             p++;
    //         }
    //     }
    // }

    printf("\n----------------\n");
    printf("LOOK :");
    printSeqNPerformance(newRequest, newCnt);
    printf("----------------\n");
    return;
}

//access the disk location in CLOOK
void accessCLOOK(int *request, int numRequest)
{
    int *newRequest = malloc(numRequest * sizeof(int)); //new order
    int newCnt=numRequest;

    //sort
    for (int i=0; i<numRequest; i++){
        for(int j = 0; j<numRequest; j++){

            if (request[j]>request[i]){
                int tmp=request[i];
                request[i]=request[j];
                request[j]=tmp;
            }
        } 
    }
    int direction=0; //0 for up, 1 for down
    //go in direction of closest end
    if(START-LOW < HIGH-START){
        //go down
        direction = 1;
    }    
    //find position
    int m=0; //location of first value to explore
    
    for(int k=0; k<numRequest; k++){
        if(direction == 0){//up
            if(request[k]>=START){
                newRequest[0]=request[k];
                break;
            }
        }
        else if(direction==1){
            if(request[k]>=START){ //down
                newRequest[0]=request[k-1];
                m--;
                break;
            }
        }    
        m++;
    }
if (direction == 0){ //up

    for (int n =1; n<numRequest-m;n++){
        newRequest[n]=request[m+n];
    }
    if(m!=0){//needs to go back
        newRequest[numRequest-m]=LOW; //fast return
        newCnt++;
        
        int p=0;
        for(int l = numRequest-m+1; l<= numRequest+1; l++){
            newRequest[l] = request[p];
            p++;
        }
    }
}
if(direction ==1){ //down
    if(m == (numRequest)){ //none in up direction
        for (int n =0; n<numRequest;n++){
            newRequest[n]=request[numRequest-n-1];  
        }
    }
    else if(m != (numRequest))//needs to go back
    for (int n =1; n<=m;n++){
            newRequest[n]=request[m-n];  
    }

    newRequest[m+1]=HIGH;//fast return
    newCnt++;

    int p=1;
    for(int l = m+2; l<= numRequest+1; l++){
        newRequest[l] = request[numRequest-p];
        p++;
    }


}
    //Similar to CScan except no need going to end of tracks

//     int next = 0; 
//     int nextvalue = 0; 

//     //find an initial acsending value
//     for(int x =0; x<numRequest;x++){
//         if(START-request[x] <0){
//             next = request[x];
//             nextvalue=START-request[x]; 
//             break;
//         }
//     }

//     int *newRequest = malloc(numRequest * sizeof(int)); //new order
//     int newCnt=numRequest;
    
//     //Assumes only goes forward (ascending) then fast return
//     //sort
//     for (int i=0; i<numRequest; i++){
//         for(int j = 0; j<numRequest; j++){

//             if (request[j]>request[i]){
//                 int tmp=request[i];
//                 request[i]=request[j];
//                 request[j]=tmp;
//             }
//         } 
//     }

//     //find closest next job and that is ascending
//     for (int i=0; i<numRequest; i++){
//         if(next ==0){
//             break;
//         }
//         //find the closest one to start
//         int value= START-request[i];
//         if(abs(value)<abs(nextvalue)){ 
//             next = request[i];
//             nextvalue=value;
//         } 
//     }

//     int done=0;//indicates whether its done when there is no acsending job from start
//     //find position
//     int m=0; //location of first value to explore
//     for(int k=0; k<numRequest; k++){
//         if(next ==0){
//             newRequest[0]=HIGH;
//             newRequest[1]=LOW;
//             newCnt+=2;
//             //go in order
//             for(int n = 2; n<numRequest+2; n++){
//                 newRequest[n]=request[n-2];
//             }
//             done =1;    
//             break;  
//         }
//         if(request[k]==next){
//             newRequest[0]=request[k];
//             break;
//         }
//         m++;
        
//     }

//   if(!done){
//     for (int n =1; n<numRequest-m;n++){
//         newRequest[n]=request[m+n];
//     }
//     if(m!=0){//needs to go back
//         newRequest[numRequest-m]=LOW; //fast return
//         newCnt++;
//         int p=0;
//         for(int l = numRequest-m+1; l<= numRequest+1; l++){
//             newRequest[l] = request[p];
//             p++;
//         }
//     }
//   }


    printf("\n----------------\n");
    printf("CLOOK :");
    printSeqNPerformance(newRequest,newCnt);
    printf("----------------\n");
    return;
}

int main()
{
    int *request, numRequest, i,ans;

    //allocate memory to store requests
    printf("Enter the number of disk access requests : ");
    scanf("%d", &numRequest);
    request = malloc(numRequest * sizeof(int));

    printf("Enter the requests ranging between %d and %d\n", LOW, HIGH);
    for (i = 0; i < numRequest; i++)
    {
        scanf("%d", &request[i]);
    }

    printf("\nSelect the policy : \n");
    printf("----------------\n");
    printf("1\t FCFS\n");
    printf("2\t SSTF\n");
    printf("3\t SCAN\n");
    printf("4\t CSCAN\n");
    printf("5\t LOOK\n");
    printf("6\t CLOOK\n");
    printf("----------------\n");
    scanf("%d",&ans);

    switch (ans)
    {
    //access the disk location in FCFS
    case 1: accessFCFS(request, numRequest);
        break;

    //access the disk location in SSTF
    case 2: accessSSTF(request, numRequest);
        break;

        //access the disk location in SCAN
     case 3: accessSCAN(request, numRequest);
        break;

        //access the disk location in CSCAN
    case 4: accessCSCAN(request,numRequest);
        break;

    //access the disk location in LOOK
    case 5: accessLOOK(request,numRequest);
        break;

    //access the disk location in CLOOK
    case 6: accessCLOOK(request,numRequest);
        break;

    default:
        break;
    }
    return 0;
}