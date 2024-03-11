//Fletcher Morton
//Assignment 6

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structs
typedef struct Node Node;
struct Node {
    Node * children[26]; //Children are each letter of the alphabet
    int mangoes;
    int subTreeSum;
};

//Prototypes
Node * createNode();
Node * insert(Node *, char *, int);
Node * deleteName(Node *, char *, int);
Node * renameNode(Node *, char *, char *);
int find(Node *, char *, int);


int main()
{
    Node *root = NULL;

    int numOrangutans;  //(Between 1 and 500,000 inclusive)
    int numEvents;      //(Between 1 and 500,000 inclusive)
    int numMangoes;     //(Between 1 and 100 inclusive)
    int option;         //(Between 1 and 3 inclusive)

    char *name = NULL;
    char *newName = NULL;

    //Take in the number of orangutans and events
    scanf("%d %d ", &numOrangutans, &numEvents);

    //Check that the user entered a value within bounds
    while(numOrangutans < 1 || numOrangutans > 500000 || numEvents < 1 || numEvents > 500000) {
        printf("One of your values was out of range. Remember:\n\tThe number of orangutans, n, must be within the following range: (1 <= n <= 500,000).");
        printf("\n\tThe number of events, e, must be within the following range: (1 <= e <= 500,000).\n\tRe-enter the number of orangutans and events: ");
        scanf("%d %d ", &numOrangutans, &numEvents);
    }

    //Declare variables that will help parse the string that the user is about to input
    char userInput[45];
	char delimiter[] = " \n";
	char *dummyPtr;
	char *subString1;
	char *subString2;
	char *subString3;

	//For loop that terminates once all of the events have happened
    for(int i = 0; i < numEvents; i++) {
        //Get user input and tell strtok where it should split it up
        fgets(userInput, 45, stdin);
        char *snippet = strtok(userInput, delimiter);

        //Assign each snippet of the string to a new string
        subString1 = snippet;
        snippet = strtok(NULL, delimiter);
        subString2 = snippet;
        snippet = strtok(NULL, delimiter);
        subString3 = snippet;

        //The first substring should be the integer corresponding to the user's desired option for the program
        //strtol will convert the string integer into a long int (here casted to an integer).
        //strtol takes the string, a pointer to separate the int and the rest of the string, and the base of the number (here base 10)
        option = (int)strtol(subString1, &dummyPtr, 10);

        //Switch statement that will decide what to do based on the option the user input
       switch(option) {
            //if command 1, the user wants to feed a monkey. The name of the monkey and the number of mangoes fed have been provided
            case 1:
                //The other two strings must have been the orangutan's name and the number of mangoes it was fed
                name = subString2;
                numMangoes = (int)strtol(subString3, &dummyPtr, 10);

                //Call the insert function
                root = insert(root, name, numMangoes);
                break;

            //if command 2, the user wants to change the name of one of the monkeys. The old and name name of the monkey have been provided
            case 2:
                //The other two strings must have been the old name and the new name
                name = subString2;
                newName = subString3;

                //Call the renameNode function
                root = renameNode(root, name, newName);
                break;

            //if command 3, the user wants to know how many mangoes have been eaten by all monkeys that fall under the specified prefix string, which has been provided.
            case 3:
                //The other two strings must have been the orangutan's name, and some empty string
                name = subString2;
                subString3 = NULL;

                //Call the find function and print it to the screen
                printf("\t%d\n", find(root, name, 1));
                break;

            //if the command is not 1, 2, or 3, there must have been an error during the input phase
            default:
                printf("\n\tInvalid input...\n\n");
        }

    }

    return 0;
}

//_________________________________________________________________

Node * createNode()
{
    Node * result = (Node *)malloc(sizeof(Node));
    result->mangoes = 0;
    result->subTreeSum = 0;

    //Initializes all of the node's children to NULL
    for(int i = 0; i < 26; i++) {
        result->children[i] = NULL;
    }

    return result;
}

//_________________________________________________________________

Node * insert(Node *root, char *name, int mangoes)
{
    //If the node does not exist, create it and continue with the insert
    if(root == NULL) root = createNode();

    //If the current letter is the null terminator, we are at the end of the name. Store the information
    if(name[0] == '\0') {
        root->mangoes += mangoes;
        root->subTreeSum += mangoes;

    //If the current letter is a character
    } else {
        //Finds the appropriate index for the current character (0-25)
        int index = name[0] - 'A';
        //Update the current node's subtree sum
        root->subTreeSum += mangoes;
        //name+1 removes the first character of the string, and the recursive call executes with the truncated string as the provided name
        root->children[index] = insert(root->children[index], name+1, mangoes);
    }

    return root;
}

//_________________________________________________________________

Node * deleteName(Node *root, char *name, int mangoesToRemove)
{
    //If the node does not exist, then the job's already done
    if(root == NULL) return root;

    //If the current letter is the null terminator, we are at the end of the name. Delete the information
    if(name[0] == '\0') {
        root->mangoes = 0;
        root->subTreeSum -= mangoesToRemove;

    //If the current letter is a character
    } else {
        //Finds the appropriate index for the current character (0-25)
        int index = name[0] - 'A';
        //Update the current node's subtree sum
        root->subTreeSum -= mangoesToRemove;
        //name+1 removes the first character of the string, and the recursive call executes with the truncated string as the provided name
        root->children[index] = deleteName(root->children[index], name+1, mangoesToRemove);
    }

    return root;
}

//_________________________________________________________________

Node * renameNode(Node *root, char *oldName, char *newName)
{
    //Use find to get the number of mangoes that this monkey has eaten. This will be used for deleting the old name, as well as inserting the new name
    int transferMangoes;
    transferMangoes = find(root, oldName, 0);

    //Delete the instance of the old name
    root = deleteName(root, oldName, transferMangoes);

    //Create an instance of the new name
    root = insert(root, newName, transferMangoes);

    return root;
}

//_________________________________________________________________

int find(Node *root, char *name, int mode)
{
    int result;

    //If the node does not exist, return 0
    if(root == NULL) return 0;

    //If the current letter is the null terminator, return the data
    if(name[0] == '\0') {
        //To give the code more versatility, there are two modes it can be put into. 0 returns the number of mangoes that the monkey at the provided name has eaten; 1 returns the subTreeSum of mangoes
        if(mode == 0) return root->mangoes;
        if(mode == 1) return root->subTreeSum;

    //If the current letter is a character
    } else {
        //Finds the appropriate index for the current character (0-25)
        int index = name[0] - 'A';
        //name+1 removes the first character of the string, and the recursive call that executes with the truncated string as the provided name
        result = find(root->children[index], name+1, mode);
    }

    return result;
}
