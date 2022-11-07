/*
 * @author - Samir Khoury.
 * PolEval2 - Polynomial Evaluator using shared memory.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

// defining macros
#define TRUE 1
#define FALSE 0
#define MAX_LEN 510
#define MAX_POL 3

// methods declaration
void scanPol(const char str[]);
void dataReset();
void calculate(int num);

int main() {
    while (TRUE){
        char input[MAX_LEN];
        fgets(input,MAX_LEN,stdin); // input from user
        input[strlen(input)-1]='\0';

        if (strcmp(input,"done")==0){
            exit(EXIT_SUCCESS);
        }

        else{
            //creating a shared memory.
            int *data;
            key_t key;
            int shm_id;
            // shared memory key generation.
            key=ftok("/tmp",'y');
            if (key==-1){
                perror("shared memory error \n");
                exit(EXIT_FAILURE);
            }
            //shared memory id generation.
            shm_id=shmget(key,(2*MAX_POL)*sizeof(int),IPC_CREAT|0600);
            if (shm_id==-1){
                perror("memory get failed\n");
                exit(EXIT_FAILURE);
            }
            data=(int*)shmat(shm_id, NULL, 0);
            if (data < 0){
                perror("memory get error\n");
                exit(EXIT_FAILURE);
            }
            // putting numbers from string to data structure.
            scanPol(input);
            pid_t pid;
            int j;
            for (j=0;j<MAX_POL;j++){
                pid=fork();
                if(pid == 0){
                    calculate(j);//calculating one of the polynomial's power.
                    exit(0);
                }
                wait(NULL);
            }
            // calculating final sum with the last factor in the polynomial.
            data[5]=data[5]+data[3];
            printf("%d\n",data[5]);
            dataReset();// resetting data for next polynomial.
            shmdt(data);//freeing data pointer.
            shmctl(shm_id,IPC_RMID,NULL);//freeing shared memory pointer.

        }
    }

}
/*
 * Data Reset
 * resets the DATA array structure for the next input.
 * PARAMETERS: None.
 * OUTPUT: None.
 */
void dataReset(){
    key_t key;
    int shm_id;
    key=ftok("/tmp",'y');
    shm_id= shmget(key,0,0600);
    int *data;
    data=(int*) shmat(shm_id,NULL,0);
    int i;
    for(i=0;i<2*MAX_POL;i++){
        data[i]=0;
    }
}
/*
 * Scan Pol
 * takes an input and breaks the numbers in a  special data structure using shared memory, where it puts the coefficients at indexs from 0-2,
 * at index 3 the last factor (with no X), at index 4 the number to be factored.
 * PARAMETERS: a string from user input.
 * OUTPUT: None.
 */
void scanPol(const char str[]){
    key_t key;
    int shm_id;
    key=ftok("/tmp",'y');
    shm_id= shmget(key,0,0600);
    int *data;
    data=(int*) shmat(shm_id,NULL,0);

    int i=0;
    int pow;
    int coef;
    int place;
    int size=strlen(str);
    while (str[i]!='\0'){
        if (str[i]!='x'){
            i++;
        }
        else{
            pow= str[i+2]-'0';

            if(str[i-1]!='+'&&i!=0){
                coef=str[i-2]-'0';
            } //coef
            else{
                coef=1;
            }
            place=3-pow;
            data[place]=coef;
            i++;
        }
    }
    if(str[i-5]=='+'){
        data[3]=str[i-4]-'0';
    }
    else{
        data[3]=0;
    }
    if (size==4){
        data[3]=str[0] - '0';
    }//if there is only a free number un the polynomial.
    data[4]=str[i-1]-'0';
}
/*
 * Calculate
 * takes the current i location and factors the inputted number and updates the result in the data structure at index 5.
 * PARAMETERS: number of iterator i.
 * OUTPUT: None.
 */
void  calculate(int num){
    key_t key;
    int shm_id;
    key=ftok("/tmp",'y');
    shm_id= shmget(key,0,0600);
    int *data;
    data=(int*) shmat(shm_id,NULL,0);

    int i= num;
    int coef=data[i];
    int eval=data[4];
    int place=3-i;
    int power;
    power = pow(eval, place);
    int calc;
    calc=coef*(power);

    data[5]=data[5]+calc;
}
