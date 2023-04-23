#include<cstdio>
#include<iostream>

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

class atomNode {
    public:
        atomic<Node*> node;
        atomNode(int x) {
            Node* tmpNode = new Node(x);
            node.exchange(tmpNode);
        }
        atomNode();
};

atomNode header(-1);

void* append(void* p) {
    int x = *(int*)p;
    atomNode tmpHeader(header.node.load(memory_order_relaxed)->val);
    Node* tmpHeaderNode = tmpHeader.node.load(memory_order_relaxed);
    while(tmpHeaderNode -> next != NULL) {
        tmpHeaderNode = tmpHeaderNode -> next;
    }
    Node* tmpAddNode = tmpHeaderNode -> next;
    tmpAddNode = new Node(x);
    atomNode tmp1;
    tmp1.node.store(tmpAddNode, memory_order_relaxed);
    return NULL;
}

void printList() {
    atomNode tmpHeader(header.node.load(memory_order_relaxed)->val);
    Node* tmpHeaderNode = tmpHeader.node.load(memory_order_relaxed);
    while(tmpHeaderNode -> next != NULL) {
        tmpHeaderNode = tmpHeaderNode -> next;
        printf("%d ", tmpHeaderNode->val);
    }
    printf("\n");
}

void* pop(void* p) {
    int x = *(int*)p;
    atomNode tmpHeader(header.node.load(memory_order_relaxed)->val);
    Node* tmpHeaderNode = tmpHeader.node.load(memory_order_relaxed);
    while(tmpHeaderNode -> next != NULL) {
        if(tmpHeaderNode -> next -> val == x) {
            Node* deleteNode = tmpHeaderNode -> next;
            tmpHeaderNode -> next = deleteNode -> next;
            free(deleteNode);
            break;
        }
        tmpHeaderNode = tmpHeaderNode -> next;
    }
    return NULL;
}

int main() {
    pthread_t thread[10];
    for(int i=0; i<10; i++)
        pthread_create(&thread[i], NULL, append, (void*)(&i));
    for(int i=0; i<10; i++)
        pthread_join(thread[i], NULL);
    printList();
    for(int i=0; i<10; i+=2)
        pthread_create(&thread[i/2], NULL, pop, (void*)(&i));
    for(int i=0; i<5; i++)
        pthread_join(thread[i/2], NULL);
    printList();
}