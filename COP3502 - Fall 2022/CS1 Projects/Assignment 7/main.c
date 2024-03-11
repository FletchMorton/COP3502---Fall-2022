//Fletcher Morton
//Assignment 7

#include <stdio.h>
#include <stdlib.h>

#define PARENT(x)   (((x)-1)/2)
#define LEFT(x)     ((x)*2+1)
#define RIGHT(x)    (((x)+1)*2)

#define ERROR 999999999

//Structs
typedef struct Shipment Shipment;
struct Shipment {
    int arrival;        //Time at which the shipment arrives
    int expiration;     //Time at which the shipment expires
    int originalSize;   //Size of the shipment as given by the user
    double curSize;     //Size of the shipment as a double for use in the heap
};

typedef struct ArrayList ArrayList;
typedef struct ArrayList Heap;
struct ArrayList {
    Shipment * array;
    int size, cap;
};

// ArrayList Prototypes
ArrayList * createList();
void append(ArrayList *, Shipment);
void deleteList(ArrayList *);

// Heap Prototypes
Heap * createHeap();
void deleteHeap(Heap *);
void heapAdd(Heap *, Shipment);
void cleanHeap(Heap *);
void heapRemove(Heap *);
Shipment front(Heap *);
void percolateUp(Heap *, int);
void percolateDown(Heap *, int);
void swap(Heap *, int, int);

//General Prototypes
void sort(Shipment *, int);
int simulation(Heap *, Shipment *, int, int, double);
double rateSearch(Heap *, Shipment *, int, int, double, double);


int main()
{
    Heap * heap = createHeap();
    Shipment * array;
    int mangoSum = 0;
    int lastExpiration = 0;
    int numShipments = 0;

    //Create an array of all the shipments
    scanf("%d", &numShipments);
    array = (Shipment *) calloc(numShipments, sizeof(Shipment));

    //Scan in all the necessary data, and update the latest expiry time, as well as the sum of all mangoes while going through the list
    for(int i = 0; i < numShipments; i++) {
        scanf("%d %d %d", &array[i].arrival, &array[i].expiration, &array[i].originalSize);
        array[i].curSize = (double)array[i].originalSize;

        if(array[i].expiration > lastExpiration) lastExpiration = array[i].expiration;
        mangoSum += array[i].originalSize;
    }

    //Sort the Shipment array by arrival time
    sort(array, numShipments);

    //Calculate the result and print it out for the user
    double result;
    result = rateSearch(heap, array, lastExpiration, numShipments, 0, mangoSum);

    printf("\n\n\n%.6f", result);

    cleanHeap(heap);
    deleteHeap(heap);
    return 0;
}

//_______________________________________________________________

//---------------------Array List Functions---------------------

//Create an instance of an ArrayList
ArrayList * createList()
{
    ArrayList * result = (ArrayList *) malloc(sizeof(ArrayList));

    result->size = 0;
    result->cap = 10; //Default capacity is 10
    result->array = (Shipment *) calloc(result->cap, sizeof(Shipment)); //Array is of type Shipment

    return result;
}

//_______________________________________________________________

//Add a Shipment to the end of the list
void append(ArrayList * list, Shipment shipment)
{
    //If the ArrayList is full
    if (list->size == list->cap) {
        list->cap *= 2; //Increase the capacity

        //Reallocate the space
        Shipment * temp = list->array;
        list->array = (Shipment *) realloc(temp, list->cap * sizeof(Shipment));
        free(temp);
    }

    //Add the passes shipment into the array and increment the size
    list->array[list->size] = shipment;
    list->size++;

}

//_______________________________________________________________

//Frees memory in the list
void deleteList(ArrayList * list)
{
    free(list->array);
    free(list);
}

//_______________________________________________________________

//------------------------Heap Functions------------------------

//Create an instance of the heap
Heap * createHeap()
{
    //Heap is an ArrayList, so call createList
    return createList();
}

//_______________________________________________________________

//Destroy the entire heap
void deleteHeap(Heap * heap)
{
    //Heap is an ArrayList, so call deleteList
    deleteList(heap);
}

//_______________________________________________________________

//Add a shipment to the heap
void heapAdd(Heap * heap, Shipment shipment)
{
    //Call the append function to add the shipment to the end of the heap, from there
    append(heap, shipment);

    //Percolate the shipment into its correct position in the heap based on expiration time
    percolateUp(heap, heap->size - 1);
}

//_______________________________________________________________

//Remove all the shipments from the heap
void cleanHeap(Heap * heap)
{
    while (heap->size != 0)
        heapRemove(heap);
}

//_______________________________________________________________

//Remove the topmost (highest priority) Shipment of the heap, here based on expiration time
void heapRemove(Heap * heap)
{
    //If the heap is empty, there is nothing to remove, so just return
    if (heap->size == 0) return;

    //Swap the value to remove (should be the root) with the last value in the heap
    swap(heap, 0, heap->size - 1);

    //Decrementing the size will effectively delete the last value in the heap, which is now the value we wanted to delete
    heap->size--;

    //The root is currently not in its proper place because of the swap. Move the value into the correct position
    percolateDown(heap, 0);
}

//_______________________________________________________________

//Return the Shipment at the top/front of the heap
Shipment front(Heap * heap)
{
    return heap->array[0];
}

//_______________________________________________________________

//Swap a value with its parent if the current value has a higher priority than its parent
void percolateUp(Heap * heap, int index)
{
    //If you are at the root, percolating up is impossible
    if (index == 0) return;

    int parentIndex = PARENT(index);

    //If the shipment at the current index expires before the shipment at the parent index, swap the two and check percolate up again
    if (heap->array[index].expiration < heap->array[parentIndex].expiration) {
        swap(heap, parentIndex, index);
        percolateUp(heap, parentIndex);
    }
}

//_______________________________________________________________

//Swap a value with its child if the current value has a lower priority than its child
void percolateDown(Heap * heap, int index)
{
    int bestIndex = index;
    int rightIndex = RIGHT(index);
    int leftIndex = LEFT(index);

    //If the right child is within bounds, and expires before bestIndex does
    if (rightIndex < heap->size && heap->array[bestIndex].expiration > heap->array[rightIndex].expiration)
        bestIndex = rightIndex;

    //If the left child is within bounds, and expires before bestIndex does
    if (leftIndex < heap->size && heap->array[bestIndex].expiration > heap->array[leftIndex].expiration)
        bestIndex = leftIndex;

    //If we adjusted bestIndex (one of the children has a higher priority)
    if (index != bestIndex) {
        //Swap the two values
        swap(heap, bestIndex, index);
        //Check percolate down again
        percolateDown(heap, bestIndex);
    }
}

//_______________________________________________________________

//Swaps two shipments
void swap(Heap * heap, int index, int index2)
{
    Shipment temp = heap->array[index];
    heap->array[index] = heap->array[index2];
    heap->array[index2] = temp;
}

//_______________________________________________________________

//----------------------General Functions----------------------

//Sort an array of shipment with a MergeSort
void sort(Shipment * array, int numShipments)
{
    //If there is only one value in the array, it is already sorted. So just return
    if (numShipments <= 1) return;

    sort(array, numShipments/2);
    sort(array + (numShipments/2), numShipments - (numShipments/2));

    Shipment * temp = calloc(numShipments, sizeof(Shipment));

    //Merge the values in both halves into the temp array
    int frontPointer = 0;
    int backPointer = numShipments/2;
    int mergePointer = 0;

    while (mergePointer < numShipments) {

        //Front is empty
        if (frontPointer == numShipments/2) {
            temp[mergePointer] = array[backPointer];
            mergePointer++;
            backPointer++;

        //If the back is empty
        } else if (backPointer == numShipments) {
            temp[mergePointer] = array[frontPointer];
            mergePointer++;
            frontPointer++;

        //Neither back or front is empty. Compare front and back halves and use the most recent arrival time to sort
        } else {
            if (array[frontPointer].arrival < array[backPointer].arrival) {
                temp[mergePointer] = array[frontPointer];
                mergePointer++;
                frontPointer++;

            } else {
                temp[mergePointer] = array[backPointer];
                mergePointer++;
                backPointer++;

            } //If/Else for smallest value

        } //If/Else for empty checks

    } //While loop


    //Copy each shipment from the merged array into the final array
    for (int i = 0; i < numShipments; i++) {
        array[i] = temp[i];
    }

    //free the temp array
    free(temp);

}

//_______________________________________________________________

//Simulates the passing of time and consumption of mangoes
//Returns 0 if a shipment spoils
//Returns 1 if everything was eaten and none of the shipments were allowed to spoil
int simulation(Heap * heap, Shipment * shipment, int end, int numShipments, double consumptionRate)
{
    //Start the simulation at the time of the first shipment
    int curTime = shipment[0].arrival;

    //Keep track of which shipments have come in
    int shipmentIndex = 0;

    //For overflow consumption
    double remainder = 0.000000;


    //Loop from first shipment to expiration of last shipment
    while(curTime <= end) {

        //If there is nothing in the heap, and no shipment is about to arrive
        if(heap->size == 0 && curTime != shipment[shipmentIndex].arrival) {

            //If all the shipments have come through, and there is nothing currently being eaten, done.
            if(shipmentIndex == numShipments) {
                cleanHeap(heap);
                return 1;
            }

            //If there are still shipments, and nothing is currently being eaten, jump to the time of the next shipment
            curTime = shipment[shipmentIndex].arrival;
        }

        //Shipment comes in (while loop to account for simultaneous shipments)
        while(curTime == shipment[shipmentIndex].arrival) {
            //Throw the shipment onto the heap
            heapAdd(heap, shipment[shipmentIndex]);

            //Update the shipment index
            shipmentIndex++;
        }

        //The priority shipment expired before it could be completely eaten
        if(curTime == front(heap).expiration && front(heap).curSize > 0) {
            cleanHeap(heap);
            return 0;
        }

        //Eat the daily amount of mangoes from the priority shipment
        if(heap->size != 0) heap->array[0].curSize -= consumptionRate;

        //Shipment has been completely eaten before it expired (while loop if finish multiple shipments in one day)
        while(front(heap).curSize <= 0 && heap->size != 0) {
            //If we eat more mangoes than are in the shipment, save the overflow in remainder
            remainder = front(heap).curSize;

            //Remove the eaten shipment from the heap
            heapRemove(heap);

            //If there is something in the heap
            if(heap->size != 0) {
                //Apply the overflow consumption to the new shipment
                heap->array[0].curSize = front(heap).originalSize + remainder;
            }
        }

        //Update the time
        curTime++;
    }

    //End of execution return
    cleanHeap(heap);
    return 0;
}

//_______________________________________________________________

//Binary search of consumption rates
double rateSearch(Heap * heap, Shipment * shipments, int end, int numShipments, double lo, double hi)
{
    //Stores the result of running the simulation with the current rate
    int valid;

    //The rate at which mangoes are consumed
    double rate;

    //Loops while lo is less than hi
    while(lo < hi) {
        rate = (double) (lo + hi) / 2;
        valid = simulation(heap, shipments, end, numShipments, rate);

        //When lo = hi, the rate is satisfactory
        if(lo == hi) return rate;

        //Simulation was passed an invalid rate. It was too slow, and something expired
        if (valid == 0) {
            //Shift the search range upwards
            lo = rate + 0.000001;

        //Simulation was passed a valid rate and all shipments were consumed before expiry
        } else if (valid == 1) {
           //Shift the search range downwards to find another rate more precice/low
            hi = rate - 0.000001;
        }

    }

    //End of execution return
    return rate;
}
