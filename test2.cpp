#include<stdio.h>
#include<pthread.h>
#include<atomic>
#include<ctime>
#include<iostream>

std::atomic<unsigned long> cnt;

void *routine(void *arg) {
    for(int i=0; i<1000000; i++)
        cnt++;
    return NULL;
}

int main() {
    cnt=0;
    const clock_t begin_time = clock();
    pthread_t thread[10000];
    for(int i=0; i<2000; i++){
        pthread_create(&thread[i], NULL, routine, (void*)&i);
    }
    for(int i=0; i<2000; i++){
        pthread_join(thread[i], NULL);
    }
    std::cout << cnt << std::endl;
    std::cout << float( clock () - begin_time ) /  CLOCKS_PER_SEC;
    return 0;
}