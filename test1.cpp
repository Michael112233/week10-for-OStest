#include<iostream>
#include<pthread.h>
#include<atomic>

using namespace std;

std::atomic<bool> ready(false);
std::atomic<bool> winner(false);

void* count(void *arg) {
    int id = *(int*)arg;
    while(!ready);
    for(int i=0; i<1000000; i++);
    if(!winner.exchange(true))
        std::cout<<"Thread #"<<id<<" won!"<<endl;
    return NULL;
}

int main() {
    pthread_t threadIDs[10];
    cout << "10 pthreads start to count to 1000000" << endl;
    for(int i=0; i<10; i++) {
        pthread_create(&threadIDs[i], NULL, count, (void*)&i);
    }
    ready = true;
    for(int i=0; i<10; i++) {
        pthread_join(threadIDs[i], NULL);
    }
}