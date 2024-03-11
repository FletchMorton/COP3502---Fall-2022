//Fletcher Morton
//Assignment 2

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Tower struct including information about the tower (height and position)
typedef struct tower tower;
struct tower
{
    int towerLocale;
    double towerHeight;
};

//constructor for our binary search function
double search(double, double, tower *, int, double, double);

int main()
{
    int numTowers;
    double parkLength;
    double percentShade;
    int towerLocale;
    double towerHeight;
    struct tower *towers;

    //Read in basic information about the park layout: Number of towers, length of the park, and percentage of the park to be shaded
    scanf("%d %lf %lf", &numTowers, &parkLength, &percentShade);

    //Check if any of the values entered were out of bounds
    while(numTowers < 2 || numTowers > 500000 || parkLength < 1 || parkLength > 1000000000 || percentShade < 1 || percentShade > 100) {
            printf("\n\t You have gone out of bounds while declaring your values. Ensure that:\n\t\t(2 <= N <= 500,000)\n\t\t(1 <= L <= 1,000,000,000)\n\t\t(1 <= P <= 100)\n\nRe-enter your three values: ");
            scanf("%d %lf %lf", &numTowers, &parkLength, &percentShade);
        }//while

    //Array of tower structs
    towers = (struct tower *) calloc(numTowers, sizeof(struct tower));

    //Tower Description loop
    for(int i = 0; i < numTowers; i++) {
        //read in the tower location and height
        scanf("%d %lf", &towerLocale, &towerHeight);

        //Check if any of the values entered were out of bounds
        while(towerLocale < 0 || towerLocale > parkLength || towerHeight < 1 || towerHeight > 1000000000) {
            printf("\n\t You have gone out of bounds while declaring your values. Ensure that:\n\t\t(0 <= X <= L)\n\t\t(1 <= H <= 1,000,000,000)\n\nRe-enter your two values: ");
            scanf("%d %lf", &towerLocale, &towerHeight);
        }//while

        //Check if the first tower is in the correct position
        while(i == 0 && towerLocale != 0) {
            printf("\n\tThe location of the first tower must be 0. Your specified position for Tower 1: %d has been adjusted to 0.", towerLocale);
            towerLocale = 0;
        }//while

        //Check if the final tower is in the correct position
        while(i == (numTowers - 1) && towerLocale != parkLength) {
            printf("\n\tThe location of the last tower must be %d. Your specified position for Tower %d: %d has been adjusted to %d.", (int)parkLength, numTowers, towerLocale, (int)parkLength);
            towerLocale = parkLength;
        }//while

        //Store the tower information in the tower struct array
        towers[i].towerHeight = towerHeight;
        towers[i].towerLocale = towerLocale;
    }//for

    //Print out the angles of the sun by calling search()
    printf("\n%.5lf %.5lf\n", search(0, 90, towers, numTowers, parkLength, percentShade), search(90, 180, towers, numTowers, parkLength, percentShade));

    //free the dynamically allocated memory
    free(towers);

    //return
    return 0;
}//main


//___________________________________________________________________________________________________________

//Binary search of sun angle values
double search(double lo, double hi, tower * towers, int numTowers, double parkLength, double percentShade) {

    double target = parkLength * (percentShade/100);

    //Binary search of 0-90
    if(lo == 0 && hi == 90){
        //loops for 60 lookups
        for (int i = 0; i < 60; i++) {
            double mid = (lo + hi) / 2;
            double sum = 0;
            double product = 0;

            //A for loop which serves as a summation of all the shade produced by the towers at the current iteration's angle of the sun
            for(int j = 0; j < numTowers; j++) {
                //"Product" is the amount of shade produced. It is calculated by taking the height of the tower, and dividing it by the tangent of the angle of the sun
                product = ( (towers[j].towerHeight) / tan( (mid * (M_PI/180)) )); //mid has to be converted to radians for the tan() function

                //Check if any of the produced shade is overlapping with another tower's shade, or is outside of park boundaries
                if(j == 0) { //The first tower is always at 0. Since we're examining the Eastern portion right now, the first tower's shade will never count
                    product = 0;
                } else if(j == 1 && product > towers[1].towerLocale) { //Here it's checked if any of the second tower's shadow surpasses the first tower. If so, the overlap is not counted
                    product = towers[j].towerLocale;
                } else if((towers[j].towerLocale - product) < towers[j-1].towerLocale) { //Checks if any tower >2 has shade overlapping with the tower before it. If so, overlap is not counted
                    product = towers[j].towerLocale - towers[j-1].towerLocale;
                }

                sum += product;

            }

            if (sum < target) {
                // mid was too large
                hi = mid;
            } else if (sum > target){
                // mid was too low
                lo = mid;
            } else if (sum == target) {
                //mid is correct, return the angle
                return ((lo + hi) / 2);
            }//if
        }//for

        //return here if (sum==target) was never hit
        return ((lo + hi) / 2);
    }//if




    //Binary search of 90-180
    if(lo == 90 && hi == 180){
        // Loops for 60 lookups
        for (int i = 0; i < 60; i++) {
            double mid = (lo + hi) / 2;
            double mirrorMid = 180 - mid; //This is the supplementary angle of mid, used to properly calculate a relevant tangent
            double sum = 0;
            double product = 0;

            //A for loop which serves as a summation of all the shade produced by the towers at the current iteration's angle of the sun
            for(int j = 0; j < numTowers; j++) {
                //"Product" is the amount of shade produced. It is calculated by taking the height of the tower, and dividing it by the tangent of the supplementary angle to the angle of the sun
                product = ( (towers[j].towerHeight) / tan( (mirrorMid * (M_PI/180)) ));

                 //Check if any of the produced shade is overlapping with another tower's shade, or is outside of park boundaries
                if(j == (numTowers - 1)) { //The last tower is always at L. Since we're examining the Western portion right now, the last tower's shade will never count
                    product = 0;
                } else if(j == (numTowers - 2) && (towers[numTowers - 2].towerLocale + product) > parkLength) { //Here it's checked if any of the second-to-last tower's shadow surpasses the final tower. If so, the overlap is not counted
                    product = parkLength - towers[j].towerLocale;
                } else if((towers[j].towerLocale + product) > towers[j+1].towerLocale) { //Checks if any tower < N-1 has shade overlapping with the tower after it. If so, overlap is not counted
                    product = towers[j+1].towerLocale - towers[j].towerLocale;
                }

                sum += product;

            }

            if (sum > target) {
                // mid was too large
                hi = mid;
            } else if (sum < target){
                // mid was too low
                lo = mid;
            } else if (sum == target) {
                //mid was correct, return the angle
                return ((lo + hi) / 2);
            }// if
        }//for

        //return here if (sum==target) was never hit
        return ((lo + hi) / 2);
    }//if


    //end of execution return
    return 0;
}//fn double search()
