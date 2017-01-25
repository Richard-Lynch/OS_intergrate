#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 6
#define NUM_STEPS 500.00

#define A_location 10.34
#define B_location 20.12
#define C 1
#define X1 3
#define X2 0
#define X3 4

pthread_mutex_t lock;
pthread_mutex_t lock1;
long double sum = 0;
long double STEP_SIZE;
//double function = 10.0;
int* a;
int* b;

long double function (long double x){
    long double result = 0.00;
    result += C;
    result += (x * X1);
    result += (x*x * X2);
    result += (x*x*x * X3);
    return result;
}

void *intergrate(void *index){
    pthread_mutex_lock(&lock1);
    int in = (int)index;
    long double a = A_location + ((long double)(in)*STEP_SIZE);
    long double b = a + STEP_SIZE;
    printf("Hello thread: %d : %Lf to %Lf\n", in, a, b);
    // if(in > 4995){
    //     printf("B for in %d = %Lf\n", in, b);
    // }

    // double area = function*(int)a[(int)index]+(int)function*b[(int)index];
    // double area =  ( ( function(b) + function(a) )/2 ) * STEP_SIZE;
    long double area =   ( function(a) + ( function(b) - function(a) )*0.5  )* STEP_SIZE;

    pthread_mutex_lock(&lock);
    printf("thread %d Adding Sum:", in);
    sum = sum + area;
    printf(" %Lf\n", sum);
    pthread_mutex_unlock(&lock);

    pthread_mutex_unlock(&lock1);
    
    pthread_exit(NULL);
}

int main(int argc, const char* argv[]){
    // a = malloc(sizeof(int)*NUM_STEPS);
    // b = malloc(sizeof(int)*NUM_STEPS);

    STEP_SIZE = (B_location-A_location)/NUM_STEPS;

    // for(int i = 0; i < NUM_STEPS; i++){
    //     a[i] = i*step_size;
    //     b[i] = a[i]+step_size;
    //     printf("A: %d B: %d\n", a[i],b[i]);
    // }

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\nlock mutex init failed\n");
        return 1;
    }

    //lock1 just ensures that a thread is allows to compeltly execute before another is created, so there is no print errors
    if (pthread_mutex_init(&lock1, NULL) != 0)
    {
        printf("\nlock1 mutex init failed\n");
        return 1;
    }

    printf("hello\n");

    pthread_t threads[(int)NUM_STEPS];
    int rc, t;

    for(t=0; t<(int)NUM_STEPS; t++){
        pthread_mutex_lock(&lock1);
        printf("Creating thread %d.\n", t);
        rc = pthread_create(&threads[t], NULL, intergrate, (void*)t);
        
        if(rc){
            printf("ERROR - return code from pthread_create: %d\n", rc);
            exit(-1);
        }
        pthread_mutex_unlock(&lock1);
    }

    for(t=0; t<(int)NUM_STEPS; t++){
        printf("Joining thread %d.\n", t);
        rc = pthread_join(threads[t], NULL);
        if(rc){
            printf("ERROR - return code from pthread_join: %d\n", rc);
            // exit(-1);
        }
    }

    printf("Result: %Lf\n", sum);

    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&lock1);

    return 0;
}



