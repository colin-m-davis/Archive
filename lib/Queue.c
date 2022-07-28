#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Queue as linked list
// For use in breadth-first search of filesystem (e.g. scanning files and subdirs on archive doc)

typedef struct node {
    FILE val;
    struct node *next;
} Node;

typedef struct queue {
    Node *head;
    Node *tail;
} Queue;

Queue* create_q(FILE *data) {
    Queue *q = malloc(sizeof(Queue));
    Node *h = malloc(sizeof(Queue));
    h->val = *data;
    q->head = h;
    q->tail = h;
    return q;
}

 FILE enqueue(Queue *q,  FILE *data) {
    Node *new = malloc(sizeof(Node));
    new->val = *data;
    free(q->tail->next);
    q->tail->next = new;
    q->tail = new;
}

 FILE dequeue(Queue *q) {
    assert(q->head != NULL);
    Node *second = q->head->next;
    free(q->head);
    q->head = second;
}

 FILE delete_q(Queue *q) {
    Node *current = q->head;
    Node *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}
