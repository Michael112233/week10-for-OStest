#include<cstdio>
#include<iostream>
#include<atomic>
#include<pthread.h>

using namespace std;

class Node {
    public:
        int val;
        Node *next;
        Node(int x) {
            val = x;
            next = NULL;
        }
};

Node* header;
atomic<bool> check(false);

void* append(void* args) {
    while(check) {
    }
    check.exchange(true);
    int x = *(int*)args;
    Node* tmpHeader = header;
    while(tmpHeader -> next != NULL) {
        tmpHeader = tmpHeader -> next;
    }
    tmpHeader -> next = new Node(x);
    check.exchange(false);

    return NULL;
}

void printList() {
    Node* tmpHeader = header -> next;
    while(tmpHeader != NULL) {
        printf("%d ", tmpHeader -> val);
        tmpHeader = tmpHeader -> next;
    }
    printf("\n");
}

void* pop(void* args) {
    Node* tmpHeader = header;
    while(tmpHeader -> next != NULL) {
        if(tmpHeader -> next -> next == NULL) {
            Node* deleteNode = tmpHeader -> next;
            tmpHeader -> next = NULL;
            free(deleteNode);
            break;
        }
        tmpHeader = tmpHeader -> next;
    }
    return NULL;
}

int main() {
    check.exchange(false);
    header = new Node(-1);
    pthread_t thread[10];
    for(int i=0; i<10; i++) {
        int* p = new int(i);
        pthread_create(&thread[i], NULL, append, (void*)p);
    }
    for(int i=0; i<10; i++)
        pthread_join(thread[i], NULL);
    printList();

    check.exchange(false);
    for(int i=0; i<10; i++) {
        int* p = new int(i);
        pthread_create(&thread[i], NULL, pop, (void*)p);
    }
    for(int i=0; i<10; i++)
        pthread_join(thread[i], NULL);
    printList();
}