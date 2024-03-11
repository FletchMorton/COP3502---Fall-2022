//Assignment 0
//Fletcher Morton

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100000

int main()
{
    char target[100 + 1];
    char camOutput[SIZE + 1];
    int lines;
    int count = 0;

    //User inputs the target animal pattern and the number of sections to process
    scanf("%s %d", target, &lines);

    //Checks the length of the animal pattern to ensure that 1 <= |S| <= 20
    if (strlen(target) < 1 || strlen(target) > 20) {
        printf("Target is out of bounds.");
        return 0;
    }

    //Checks the amount of lines to ensure that 1 <= |N| <= 100000
    if (lines < 1 || lines > 100000) {
        printf("Number of lines is out of bounds.");
        return 0;
    }

    //Eats remnants of the rest of the first line
    fgets(camOutput, (SIZE + 1), stdin);

    //For loop cycles through the sections of the park
    for(int i = 0; i < lines; i++) {
        //fgets writes the user input into the current section
        fgets(camOutput, (SIZE + 1), stdin);

        //Checks to ensure that there are no more than 100,000 characters in the sequence
        if(strlen(camOutput) < 1 || strlen(camOutput) > SIZE) {
            printf("Invalid amount of characters.");
            exit(0);
        }

        int size = strlen(camOutput);

        //For loop cycles through the characters of the sequence that was just input
        for(int j = 0; j < size; j++) {
            int match = 0;

            //Looks for a match with the target string starting at each character and continuing for the length of 'target' before proceeding to the next character and repeating
            for(int k = 0; k < strlen(target); k++) {
                if(camOutput[j+k] == target[k])
                    match++;
                if(match == strlen(target))
                    count++;
            }
        }
        printf("%d\n", count);
        count = 0;
    }

    return 0;
}
