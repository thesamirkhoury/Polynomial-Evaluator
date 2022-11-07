/*
 * @author - Samir Khoury.
 * PolEval1 - Polynomial Evaluator using threads.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

// defining macros
#define TRUE 1
#define FALSE 0
#define MAX_LEN 510
#define MAX_POL 3

//global declaration for use in thread related function.
int data[2*MAX_POL];

// methods declaration
void scanPol(const char str[]);
void dataReset();
void* calculate(void *num);


int main() {
    int status;
    pthread_t thread[MAX_POL]; // creating a thread array
    while (TRUE){
        char input[MAX_LEN];
        fgets(input,MAX_LEN,stdin); // input from user
        input[strlen(input)-1]='\0';

        if (strcmp(input,"done")==0){
            exit(EXIT_SUCCESS);
        }

        else{
            // putting numbers from string to data structure.
            scanPol(input);
            int ittNum[MAX_POL];
            int i;
            //creating threads and sending to calculate function.
            for(i=0;i<MAX_POL;i++){
                ittNum[i]=i;
                status=pthread_create(&thread[i],NULL,calculate,(void*)&ittNum[i]);//calculating one of the polynomial's power using threads.
                if (status!=0){
                    perror("thread error!");
                    exit(EXIT_FAILURE);
                }
            }
            // waiting for threads to finish
            for(i=0;i<MAX_POL;i++){
                pthread_join(thread[i],NULL);
            }
            int resultLocation=(2*MAX_POL)-1;

            // calculating final sum with the last factor in the polynomial.
            data[resultLocation]=data[resultLocation]+data[MAX_POL];
            printf("%d\n",data[resultLocation]);
            dataReset();// resetting data for next polynomial.
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
    int i;
    int size=2*MAX_POL;
    for(i=0;i<size;i++){
        data[i]=0;
    }
}
/*
 * Scan Pol
 * takes an input and breaks the numbers in a  special data structure, where it puts the coefficients at indexs from 0-2,
 * at index 3 the last factor (with no X), at index 4 the number to be factored.
 * PARAMETERS: a string from user input.
 * OUTPUT: None.
 */
void scanPol(const char str[]){
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
 * PARAMETERS: number of iterator i (in void format)
 * OUTPUT: None.
 */
void*  calculate(void *num){
    int i= *((int*)num);
    int coef=data[i];
    int eval=data[4];
    int place=3-i;
    int power;
    power = pow(eval, place);
    int calc;
    calc=coef*(power);
    data[5]=data[5]+calc;
    return NULL;
}

