//Fletcher Morton
//Assignment 1: Organization

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR    1000

//struct cage that contains a string representing the name of the animal within the cage
struct cage
{
char * animal;
};

//struct section which contains an array of cage structs, as well as the number of cages (struct array len)
typedef struct section section;
struct section
{
struct cage * cages;
int numCages;
};

//prototypes
void cageEdit(section *, int, int);
void addAnimal(section *, int, int, char *);
void cageLookUp(section *, int, int);
void freeMem(section *, int);

int main()
{
    int numSections = 0;
    int command = 0;
    int sectionIndex = 0;
    int newCageAmt = 0;
    int cageIndex = 0;
    char *animal;

    //allocate maximum space for animal name
    animal = (char *) malloc((MAX_CHAR + 1) * sizeof(char));

    //prompt the user for the number of sections in their park
    do {
        printf("Please enter the number of sections that are located in the park: ");
        scanf("%d", &numSections);

        //if the value is invalid, reject it and allow the user to enter a correct one
        if(numSections < 1 || numSections > 100000){
            numSections = 0;
            printf("\n\nInvalid number of sections. (1 <= number of sections <= 100,000)\n\n");
        }
    }while(numSections == 0);

    //creates struct array of sections
    struct section *sections;
    sections = (struct section *) calloc(numSections, sizeof(struct section));
    for(int i = 0; i < numSections; i++) {
        sections[i].cages = (struct cage *) calloc(0, sizeof(struct cage));
    }

    //loops until user wants to quit
    while(command != 4) {
        /*only the first number of the potential maximum of 4 is read here. The rest of the digits/strings are read within the case corresponding to the user's command
        this makes it to where the program never reads an excess of digits or a string where an int is attempting to be scanned*/
        scanf("%d", &command);

        switch(command) {
            //if command 1, sectionIndex and newCageAmt are read and passed to the cageEdit function which is called.
            case 1:
                scanf("%d %d", &sectionIndex, &newCageAmt);
                cageEdit(sections, sectionIndex, newCageAmt);
                break;
                //if command 2, sectionIndex, cageIndex, and the name of the animal are read and passed to the addAnimal function which is called.
            case 2:
                scanf("%d %d %s", &sectionIndex, &cageIndex, animal);
                addAnimal(sections, sectionIndex, cageIndex, animal);
                break;
                //if command 3, sectionIndex and cageIndex are read and passed to the cageLookUp function which is called.
            case 3:
                scanf("%d %d", &sectionIndex, &cageIndex);
                cageLookUp(sections, sectionIndex, cageIndex);
                break;
                //if command 4, the program is exited
            case 4:
                //free the dynamically allocated memory first
                freeMem(sections, numSections);
                printf("\n\tGoodbye.\n");
                return 0;
                //if the command is not 1, 2, 3, or 4, there must have been an error during the input phase. The user is looped back to the beginning and prompted to reenter a valid value
            default:
                printf("\n\tInvalid input...\n\n");
        }
    }




    return 0;
}

//______________________________________________________________________________________________________

//Add or Delete elements of the struct cage array
void cageEdit(section *sections, int sectionIndex, int newCageAmt)
{
    //section index is decremented here because the array index begins with zero
    struct cage* newArray;
    sectionIndex--;

    //if the user is attempting to add cages to a section...
    if (sections[sectionIndex].numCages < newCageAmt) {
        //add the current number of existing cages to i, so memory is allocated at the end of the array instead of on top of potentially existing or populated space
        for(int i = 0; i+(sections[sectionIndex].numCages) < newCageAmt; i++) {
            //even if this is the first instance of a cage being created, realloc is ok to use because the number of elements were already initialized at zero
            newArray = (struct cage *) realloc((sections[sectionIndex].cages), (newCageAmt * sizeof(struct cage)));
            sections[sectionIndex].cages = newArray;
            //allocate some memory for the animal name and set it as the null pointer for now
            sections[sectionIndex].cages[i].animal = (char *) malloc((MAX_CHAR + 1) * sizeof(char));
            strcpy(sections[sectionIndex].cages[i].animal, "\0");
        }
    //if the user is attempting to remove cages/truncate the array...
    } else if (sections[sectionIndex].numCages > newCageAmt) {
        //reallocate the array memory to only have the capacity of the newCageAmt * the size of a cage struct
        newArray = (struct cage *) realloc((sections[sectionIndex].cages), (newCageAmt * sizeof(struct cage)));
        sections[sectionIndex].cages = newArray;
    }

    //update the number of cages in the section so our numbers are correct if this command is used again
    sections[sectionIndex].numCages = newCageAmt;
}

//______________________________________________________________________________________________________

//Add an animal String to a cage in the struct cages array
void addAnimal(section *sections, int sectionIndex, int cageIndex, char* name)
{
    //section and cage index are decremented here because the array index begins with zero
    sectionIndex--;
    cageIndex--;
    int len = strlen(name);
    char* realStr;

    //the name of the animal is copied into the animal slot of the cage. Right now, both have the maximum character space allocated
    strcpy(sections[sectionIndex].cages[cageIndex].animal, name);
    //reallocate the memory so the String is only as large as it needs to be
    realStr = (char *) realloc(sections[sectionIndex].cages[cageIndex].animal, ((len + 1) * sizeof(char)));
    sections[sectionIndex].cages[cageIndex].animal = realStr;
}

//______________________________________________________________________________________________________

//print out the animal contained in a specific cage within a specific section
void cageLookUp(section *sections, int sectionIndex, int cageIndex)
{
    //section and cage index are decremented here because the array index begins with zero
    sectionIndex--;
    cageIndex--;

    //first make sure the user is trying to access a cage that actually exists in the memory
    if(cageIndex > (sections[sectionIndex].numCages - 1)) {
        printf("\n\tAnimal not found.\n");
        //return them early if caught by the if statement
        return;
    //then make sure there's actually an animal in that cage
    } else if (strcmp(sections[sectionIndex].cages[cageIndex].animal, "\0") == 0) {
        printf("\n\tAnimal not found.\n");
        //return them early if caught by the if statement
        return;
    }

    //if there was an animal in the specified cage, print out its name
    printf("\n\tSection %d | Cage %d [%s]\n", (sectionIndex+1), (cageIndex+1), sections[sectionIndex].cages[cageIndex].animal);

}

//______________________________________________________________________________________________________

//frees dynamically allocated memory
void freeMem(section *sections, int numSections)
{
    for(int i = 1; i < numSections; i++) {
        free(sections[i].cages);
    }
    free(sections);
}
