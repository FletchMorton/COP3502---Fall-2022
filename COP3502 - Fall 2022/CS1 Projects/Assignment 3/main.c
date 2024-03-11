//Fletcher Morton

#include <stdio.h>
#include <stdlib.h>

//Prototypes
void permute(int **, int, int);
void swap(int *, int *);

//global variables
int lowestSum = 999999999;

int main()
{
    int exhibits;
    scanf("%d", &exhibits);

    //Check to make sure that the number of exhibits are within the allowed range, if not, let the user re-enter the value
    while(exhibits < 1 || exhibits > 11) {
        printf("\n\tThis value is out of bounds. Your value, n, should be within the following bounds: (1 <= n <= 11)");
        printf("\n\tRe-enter your value: ");
        scanf("%d", &exhibits);
    }

    //Dynamically allocate a 2D array for animal noises
    int **exhibitArray;
    exhibitArray = (int **) calloc(exhibits, sizeof(int*));

    for(int i = 0; i < exhibits; i++) {
        exhibitArray[i] = (int *) calloc((exhibits+1), sizeof(int));
    }

    //Populate the 2D array with animal noises
    for(int i = 0; i < exhibits; i++) {
        for(int j = 0; j < exhibits; j++) {
            scanf("%d", &exhibitArray[i][j]);

            //Makes sure that an animal does not aggro itself by testing to see if the user entered a zero where they should have, and then correcting the invalid input if they did
            if(i == j && exhibitArray[i][j] != 0) {
                printf("An animal can't aggravate itself. This value (at [%d][%d]) this value has been changed to zero.\nContinue input...\n", i, j);
                exhibitArray[i][j] = 0;
            }
        }
    }


    printf("\n\n\n");
    //permutation loop
    for(int i = 0; i < exhibits; i++) {
        permute(exhibitArray, 0, (exhibits-1));
    }

    printf("%d\n", lowestSum);

    //free dynamically allocated memory
    for(int i = 0; i < exhibits; i++) {
        free(exhibitArray[i]);
    }
    free(exhibitArray);

    return 0;
}

//______________________________________________

//permutes through the 2D array and takes the sums of those permutations as it goes. Those sums are compared to global variable lowestSum as they are generated
void permute(int **nums, int startIndex, int lastIndex)
{
    int i;
    int j;
    int sum = 0;
    int counter;

    //This variable is a boolean which is marked as true when two zeros are placed within the same column. It starts off initialized to false..
    //This prevents the reusing of an animal multiple times in the array
    int animalReused = 0;

    //If start = end...
    if(startIndex == lastIndex) {

        //Summation for loop
        for(i = 0; i <= lastIndex; i++) {
            for(j = 0; j <= lastIndex; j++) {
                counter = 0;

                //Validity checking
                if(i != 0 && nums[i][j] == 0) { //if we are not on the first row of the array and the current index is a zero
                    for(int k = (i-1); k >= 0; k--) {
                        if(nums[k][j] != 0) counter++; //increment a counter for each member of the current index's column that is non-zero. Making sure no animal is reused in the current array
                    }
                }

                //Works out the specifics of how the current row should be summed
                if(nums[i][j] == 0) {
                    if(i == 0 && animalReused == 0) { //If we are in the first row of the 2D array, and no animals have been reused...
                        //only the two values on the right are counted
                        if(j == 0) {
                            sum += (nums[i][j+1] + nums[i][j+2]);
                        //two values on the right and one value on the left are counted
                        } else if(j == 1) {
                            sum += (nums[i][j-1] + nums[i][j+1] + nums[i][j+2]);
                        //only the two values on the left are counted
                        } else if(j == lastIndex) {
                            sum += (nums[i][j-1] + nums[i][j-2]);
                        //two values on the left and one value on the right are counted
                        } else if(j == (lastIndex-1)) {
                            sum += (nums[i][j-1] + nums[i][j-2] + nums[i][j+1]);
                        //the two values on the right and the two values on the left are counted
                        } else {
                            sum += (nums[i][j-1] + nums[i][j-2] + nums[i][j+1] + nums[i][j+2]);
                        }
                        counter = 0;
                    } else if(counter == i && animalReused == 0) { //If there are currently no duplicate zeros in the index's column, and there have been no animals reused so far...
                        //only the two values on the right are counted
                        if(j == 0) {
                            sum += (nums[i][j+1] + nums[i][j+2]);
                        //two values on the right and one value on the left are counted
                        } else if(j == 1) {
                            sum += (nums[i][j-1] + nums[i][j+1] + nums[i][j+2]);
                        //only the two values on the left are counted
                        } else if(j == lastIndex) {
                            sum += (nums[i][j-1] + nums[i][j-2]);
                        //two values on the left and one value on the right are counted
                        } else if(j == (lastIndex-1)) {
                            sum += (nums[i][j-1] + nums[i][j-2] + nums[i][j+1]);
                        //the two values on the right and the two values on the left are counted
                        } else {
                            sum += (nums[i][j-1] + nums[i][j-2] + nums[i][j+1] + nums[i][j+2]);
                        }
                        counter = 0;
                    } else {
                        /* This else statement is triggered if the current index has a duplicate zero in its column. This means that the permutation is invalid and the sum is not counted.
                           animalReused is changed to true here, so even if in the future an index has no duplicate zeros in its column, we know not to bother summing, since we already decided
                           that the array's sum should not be taken into account when trying to find the lowest sum.                                                                            */
                        sum = 0;
                        animalReused = 1;
                        break;
                    }
                }
            }
        }

        //Find which sum out of the calculated array sums are the lowest of all them all
        for(i = 0; i <= lastIndex; i++) {
            if(lowestSum >= sum && sum != 0) { //lowestSum is a global variable, and was initialized to an impossible-to-reach value
                lowestSum = sum; //If a valid sum is less than or equal to lowestSum, it takes its place
            }
        }

    } else { //if we have not reached start = end yet...
        //Swap and permute the array
        for(i = startIndex; i <= lastIndex; i++) {
            for(j = startIndex; j <= lastIndex; j++) {
                swap(&nums[i][startIndex], &nums[i][j]);
                permute(nums, startIndex+1, lastIndex); //run the function again, but now your starting index has increased by one. This continues until start=end
                swap(&nums[i][startIndex], &nums[i][j]); //swap back
            }
        }
    }
}

//______________________________________________

//Swaps the values of two elements
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
