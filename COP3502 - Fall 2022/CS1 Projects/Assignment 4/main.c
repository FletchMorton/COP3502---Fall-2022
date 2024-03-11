//Fletcher Morton
//Assignment 4

#include <stdio.h>
#include <stdlib.h>

#define INVALID -1

//typedefs
typedef struct Queue Queue;
typedef struct Node Node;
typedef struct Group Group;

//Group, which contains the group's arrival time, processing time, and size, all in type long long int
struct Group {
long long int arrival;
long long int size;
long long int processing;
};

//Node in the circular linked list which contains data of type Group, as well as a pointer to the next node in the list
struct Node {
Group data;

Node * next;
};

//Queue to be implemented using a circular Linked List. Contains a tail node for LL operations, and an int to track how many people are in the queue
struct Queue {
int numPeople;

Node * tail;
};

//Group prototypes
void sort(Group **, int);                       //Sort an array of groups in increasing order of arrives
Group * createGroup(int, int, int);             //Create a group with size, arrival time, and processing time as data members
long long int calcGlobalTime(Group **, int);    //Gets the total sum of all processing time

//Circular linked list prototypes
Node * createNode(Group);                       //Create a node with a new group as data
Node * insertTail(Node *, Group);               //Add a group to the tail of a circular linked list and return the resulting tail
Node * removeHead(Node *);                      //Remove the head node of a circular linked list and return the resulting tail

//Queue Prototypes
int front(Queue *);                             //Returns the values of the node at the front of the queue
void dequeue(Queue *);                          //Removes the node at the front of the queue
void enqueue(Queue *, Group);                   //Adds a node to the end of the queue
Queue * createQueue();                          //Creates a new, empty queue
void cleanQueue(Queue *);                       //Repeatedly removes the head of the queue until the queue is empty


int main() {
    Queue * l = createQueue(); //create left queue
    Queue * r = createQueue(); //create right queue
    int groups;
    long long int size;
    long long int arrival;
    long long int processing;

    //Scan in the number of groups that will enter the park
    scanf("%d", &groups);

    //Check to make sure that the number of groups are within the allowed range, if not, let the user re-enter the value
    while(groups < 1 || groups > 500000) {
        printf("\n\tThis value is out of bounds. Your value, n, should be within the following bounds: (1 <= n <= 500000)");
        printf("\n\tRe-enter your value: ");
        scanf("%d", &groups);
    }

    //Dynamically create an array of Group pointers
    Group ** array;
    array = (Group **) calloc(groups, sizeof(Group *));

    //Scan in the size, arrival time, and processing time for each group, and use that information to call createGroup()
    for(int i = 0; i < groups; i++) {
        scanf("%lld %lld %lld", &size, &arrival, &processing);

        //Check to make sure that the size, arrival time, and processing time are within the allowed range, if not, let the user re-enter the value
        while(size < 1 || arrival < 1 || processing < 1 || size > 1000000 || arrival > 1000000 || groups > 1000000) {
            printf("\n\tOne of your values are out of bounds. Remember that group size, arrival time, and processing time, should be all be within the bounds (1 <= n <= 1000000)");
            printf("\n\tRe-enter your three values: ");
            scanf("%lld %lld %lld", &size, &arrival, &processing);
        }

        array[i] = createGroup(size, arrival, processing);
    }

    //Call sort to order the group array in order of increasing arrival time
    sort(array, groups);


    //------------------------SIMULATION------------------------

    long long int globalTime = calcGlobalTime(array, groups); //Global time is the sum of all processing times

    long long int waitL = 0;
    long long int waitR = 0;

    long long int processL = 0;
    long long int processR = 0;

    long long int sum = 0;

    //This for loop simulates the passing of time
    //Each time i increments, a unit of time has passed
    for(int i = 1, j = 0; i <= (globalTime+1); i++) {

        //Have the queues been processed until empty?
        if(j >= groups && front(l) == INVALID && front(r) == INVALID && processL == 0 && processR == 0 && waitL == 0 && waitR == 0) {
            break; //If so, end the simulation
        }

        //Dequeues a group when they have finished processing
        if(i > 2 && processL == 0 && processR == 0) {       //If the left and right lines have finished processing at the same time
            if(l->tail != NULL) {                           //If there is a group at the front of the left line, remove the group and adjust the number of people
                l->numPeople -= l->tail->data.size;
                dequeue(l);
            }
            if(l->tail != NULL && l->tail->next != NULL) {  //If there is a group waiting at the new head
                processL = l->tail->next->data.processing;  //start processing them
            }

            if(r->tail != NULL) {                           //If there is a group at the front of the right line, remove the group and adjust the number of people
                r->numPeople -= r->tail->data.size;
                dequeue(r);
            }
            if(r->tail != NULL && r->tail->next != NULL) {  //If there is a group waiting at the new head
                processL = r->tail->next->data.processing;  //start processing them
            }

        } else if(i > 1 && processL == 0) {                 //if the left line finished processing a group
            if(l->tail != NULL) {                           //If there is a group at the front of the left line, remove the group and adjust the number of people
                l->numPeople -= l->tail->data.size;
                dequeue(l);
            }
            if(l->tail != NULL && l->tail->next != NULL) {  //If there is a group waiting at the new head
                processL = l->tail->next->data.processing;  //start processing them
            }

        } else if(i > 2 && processR == 0) {                 //if the right line has finished processing a group
            if(r->tail != NULL) {                           //If there is a group at the front of the right line, remove the group and adjust the number of people
                r->numPeople -= r->tail->data.size;
                dequeue(r);
            }
            if(r->tail != NULL && r->tail->next != NULL) {  //If there is a group waiting at the new head
                processL = r->tail->next->data.processing;  //start processing them
            }
        }


        //Enqueues a group when their arrival time has come
        if(i == array[0]->arrival) {                        //Enqueues the first group, putting them in the left line
            enqueue(l, *array[0]);                          //This value is now the tail
            j++;

            l->numPeople += l->tail->data.size;             //adjust the number of people
            waitL += l->tail->data.processing * l->tail->data.size; //Start the wait timer
            processL = l->tail->data.processing * l->tail->data.size; //processing starts immediately
            sum += l->tail->data.processing * l->tail->data.size; //add to the sum

        } else if(i == array[1]->arrival) {                 //Enqueues the second group, putting them in the right line
            enqueue(r, *array[1]);
            j++;

            r->numPeople += l->tail->data.size;
            waitR += r->tail->data.processing * r->tail->data.size;
            processR = r->tail->data.processing * r->tail->data.size; //processing starts immediately
            sum += r->tail->data.processing * r->tail->data.size;

        } else if(j < groups && i == array[j]->arrival) {   //Enqueues any group j > 1, and decides which line they should go in
            if(l->numPeople == r->numPeople || r->numPeople > l->numPeople) { //If both lines have the same amount of people, or if the right line is longer than the left line, send the group to the left line
                enqueue(l, *array[j]);

                l->numPeople += l->tail->data.size;
                sum += (waitL + l->tail->data.processing) * l->tail->data.size;
                waitL += l->tail->data.processing * l->tail->data.size;

            } else if(l->numPeople > r->numPeople) { //If the left line is longer than the right line, send the group to the right line
                enqueue(r, *array[j]);

                r->numPeople += r->tail->data.size;
                sum += (waitR + r->tail->data.processing) * r->tail->data.size;
                waitR += r->tail->data.processing * r->tail->data.size;

            }

            j++;

        }

        //Decrement all of the timers, but only if they are greater than 0
        if (waitL != 0) waitL--;
        if (waitR != 0) waitR--;
        if(processL != 0) processL--;
        if(processR != 0) processR--;

    }

    //----------------------------------------------------------


    //Print out the sum
    printf("\n\n%lld\n\n", sum);

    //Free the dynamically allocated array
    for(int i = 0; i < groups; i++) {
        free(array[i]);
    }

    free(array);

    //Call cleanQueue on r and l just to be safe
    cleanQueue(r);
    cleanQueue(l);

    //End program
    return 0;
}

//_______________________________________________________________

//Queue functions
//Returns the value of arrival time at the front of the queue. Helps other functions see if the list is empty or ordered
int front(Queue * q) {

    //Ensure that there is no attempt to access inappropriate/invalid data
    if (q == NULL) return INVALID;
    if (q->tail == NULL) return INVALID;
    if (q->tail->next == NULL) return INVALID;

    //return that arrival time of the LL head
    return q->tail->next->data.arrival;
}


//Remove the front of the queue
void dequeue(Queue * q)
{
    q->tail = removeHead(q->tail);

}


//Adds to the back of the queue
void enqueue(Queue * q, Group newGroup)
{
    q->tail = insertTail(q->tail, newGroup);

}


//Creates an empty queue, where the only node is a NULL tail
Queue * createQueue()
{
    Queue * result = (Queue *) malloc(sizeof(Queue));
    result->numPeople = 0;
    result->tail = NULL;
    return result;

}

//Repeatedly removed the head of the queue, until it is detected that the queue is empty
void cleanQueue(Queue * q)
{
    while (front(q) != INVALID) {
        dequeue(q);
    }

}


//_______________________________________________________________

//Circular Linked List functions
//Changes the location of the Linked List's head, and frees the node which had previously been the head
Node * removeHead(Node * tail)
{
    //If the list is empty
    if (tail == NULL) {
        return NULL;
    }

    //If there is only one node in the list
    if (tail == tail->next) {
        free(tail);

        //The list is now empty
        return NULL;
    }

    //If there is more than one node
    Node * oldTail = tail;
    Node * oldHead = tail->next;
    Node * newHead = oldHead->next;

    //Free the memory to remove
    free(oldHead);

    //Adjust pointers
    oldTail->next = newHead;

    //Return the resulting tail
    return oldTail;

}


//Creates a new node and places it after the tail of the Linked List. Then updates pointers so the newly created node is now the tail
Node * insertTail(Node * tail, Group newGroup)
{
    //If the list is empty
    if (tail == NULL) {
        return createNode(newGroup);
    }

    //If the list has at least one node
    Node * oldTail = tail;
    Node * oldHead = oldTail->next;
    Node * newTail = createNode(newGroup);

    //Adjust pointers
    oldTail->next = newTail;
    newTail->next = oldHead;

    //Return the resulting tail
    return newTail;

}


//Generates a node and initializes its data
Node * createNode(Group newGroup)
{
    Node * result = (Node *) malloc(sizeof(Node));

    result->data = newGroup;
    result->next = result;

    return result;

}

//_______________________________________________________________

//Group functions
//Generates a Group and initializes its values
Group * createGroup(int size, int arrival, int processing)
{
    Group * result = (Group *) malloc(sizeof(Group));
    result->size = size;
    result->arrival = arrival;
    result->processing = processing;

    return result;

}


//Calculate the sum of all the processing times
long long int calcGlobalTime(Group **array, int size)
{
    long long int globalTime = 0;

    for(int i = 0; i < size; i++) {
        if(i == 0) {
            globalTime = (array[i]->arrival + array[i]->processing) * array[i]->size;

        } else {
            globalTime += array[i]->processing * array[i]->size;

        }
    }

    return globalTime;

}


//A merge sort which works with Groups. The array of Groups is sorted by increasing arrival time
void sort(Group** arr, int size)
{
    //If there is only one value in the array, it is already sorted. So just return
    if (size <= 1) {
        return;
    }

    sort(arr, size/2);
    sort(arr + size/2, size - size/2);

    Group ** tmp = calloc(size, sizeof(Group *));

    // merge the values in both halves into the tmp array
    int frontPointer = 0;
    int backPointer = size / 2;
    int mergePointer = 0;

    while (mergePointer < size) {
        if (frontPointer == size / 2) {         //if the front is empty
            tmp[mergePointer] = arr[backPointer];
            mergePointer++;
            backPointer++;

        } else if (backPointer == size) {       //if the back is empty
            tmp[mergePointer] = arr[frontPointer];
            mergePointer++;
            frontPointer++;

        } else {
            //if neither the front or back are empty, compare the front of both halves and use smallest arrival time value
            if (arr[frontPointer]->arrival < arr[backPointer]->arrival) {
                tmp[mergePointer] = arr[frontPointer];
                mergePointer++;
                frontPointer++;

            } else {
                tmp[mergePointer] = arr[backPointer];
                mergePointer++;
                backPointer++;

            } //if/else for smallest value

        } //if/else for empty checks

    } //while loop


    // Copy the merged array into the final array
    for (int i = 0; i < size; i++) {
        arr[i] = tmp[i];
    }

    //free the temp array
    free(tmp);

}
