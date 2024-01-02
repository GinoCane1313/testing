#include <stdio.h>

#define QUEUE_SIZE 128

/**
Circular queue implementation, based on array
*/
static int queue[QUEUE_SIZE];

/**
Front is the index where we dequeue items from
*/
static int front = -1;

/**
Rear is the index where we enqueue items to
*/
static int rear = -1;

int q_size()
{
    return (front > rear) ? (QUEUE_SIZE - front + rear + 1) : (rear - front + 1);
}

/**
@return 1 if queue is full, 0 otherwise
*/
int q_isFull()
{
    if ((front == rear + 1) || (front == 0 && rear == QUEUE_SIZE - 1))
    {
        return 1;
    }
    return 0;
}

/**
@return 1 if queue is empty, false otherwise
*/
int q_isEmpty()
{
    if (front == -1)
    {
        return 1;
    }
    return 0;
}

/**
Enqueue the given item to the list, if is not full
*/
void q_enqueue(int element)
{
    if (q_isFull())
    {
        printf("\n\n###############################################\n\n");
        printf("           [ERROR] Queue overflow detected!!           ");
        printf("\n\n###############################################\n\n");
        return;
    }

    if (front == -1)
    {
        front = 0;
    }

    rear = (rear + 1) % QUEUE_SIZE;
    queue[rear] = element;
}

/**
Dequeue and return next item (FIFO) from the queue;
if empty, return -1
*/
int q_dequeue()
{
    if (q_isEmpty())
    {
        return -1;
    }

    int element = queue[front];

    if (front == rear)
    {
        // this was the last element, let's reset the queue
        front = -1;
        rear = -1;
    }
    else
    {
        front = (front + 1) % QUEUE_SIZE;
    }

    return element;
}

/**
Display the content of the whole queue
*/
void q_display()
{
    printf("\n-----------------\n");
    if (q_isEmpty())
    {
        printf("Queue is empty\n");
        return;
    }

    printf("%d Items: ", q_size());
    int i;
    for (i = front; i != rear; i = (i + 1) % QUEUE_SIZE)
    {
        printf("%d ", queue[i]);
    }
    printf("%d\n", queue[i]);
    printf("-----------------\n\n");
}
