//Fletcher Morton
//Assignment 5

#include <stdio.h>
#include <stdlib.h>

//Structs
typedef struct Node Node;
struct Node {
    Node * parent;
    Node ** children;
    int id;
    int monkeys;
    int numChildren;
};

//Prototypes
Node * createNode(int id, int monkeys);
Node * addNode(Node *, int, int, int);
int sumMonkeys(Node *, int);
Node * findNode(Node *, int);
void cleanTree(Node *);

//Main
int main()
{
    int exhibits;   //Represents the number of exhibits in the park (between 1 and 200,000)
    int parentID;   //Represents the ID of the parent
    int id;         //Represents the ID of an exhibit (The entrance must be of ID 1, others range from 2 to the number of exhibits)
    int monkeys;    //Represents the number of monkeys hanging around at each exhibit (between 1 and 5,000)
    int visitors;   //Represents the number of visitors to the park (between 1 and 500,000)
    int destination;//Represents the ID of the exhibit that the visitors would like to visit (Value should be between 1 and the number of exhibits)
    int depth = 0;  //Represents the current tree depth

    //Prompt the user to enter the number of exhibits in the park
    scanf("%d", &exhibits);

    //Check that the user entered a value within bounds
    while(exhibits < 1 || exhibits > 200000) {
        printf("The number of exhibits, n, must be within the following range: (1 <= n <= 500,000).\n\tRe-enter the number of exhibits: ");
        scanf("%d", &exhibits);
    }

    Node * root = NULL;

    //Prompt the user to enter the id of, and number of monkeys at, each of the exhibits
    for(int i = 0; i < exhibits;) {
        scanf("%d", &id);

        if(id != -1) {
            //If the user to adding another exhibit
            scanf(" %d", &monkeys);

            //Check that the user entered a value within bounds
            while(monkeys < 1 || monkeys > 5000) {
                printf("The number of monkeys, n, must be within the following range: (1 <= n <= 5,000).\n\tRe-enter the number of monkeys: ");
                scanf("%d", &monkeys);
            }

            //Special cases for if the entrance is being input
            if(i == 0) {
                //Set the parentID to 0 since parent is NULL
                parentID = 0;

                //make sure the proper ID is entered
                if(id != 1) {
                    while(id != 1) {
                        printf("The first exhibit entered must be the entrance, which has ID 1. Enter id and the number of monkeys at the entrance: ");
                        scanf("%d %d", &id, &monkeys);
                    }//while
                }//if id is not 1
            }//if the exhibit is being input

            root = addNode(root, parentID, id, monkeys);

        } else {
            //If the user is backtracking
            monkeys = 0;
            id = parentID;

            parentID = findNode(root, id)->parent->id;
            depth--;
            continue;
        }

        parentID = id;
        i++;
        depth++;
    }

    //Have the user return to the entrance
    while(depth > 1) {
        scanf("%d", &id);
        if(id == -1) {
            depth--;
        } else {
            printf("The maximum number of exhibits have been entered, please input -1 until you leave the park.\n");
        }
    }

    //Prompt the user to enter the number of visitors to the park
    scanf("%d", &visitors);

    //Check that the user entered a value with the correct bounds
    while(visitor < 1 || visitors > 500000) {
        printf("The number of visitors, n, must be within the following range: (1 <= n <= 500,000).\n\tRe-enter the number of visitors: ");
        scanf("%d", &visitors);
    }

    //Prompt the user to enter the ID of the destination exhibit that the current visitor would like to visit
    for(int i = 0; i < visitors; i++) {
        scanf("%d", &destination);
        //Print out the number of monkeys encountered during the trip
        printf("\n%d\n", sumMonkeys(root, destination));
    }

    //Clean the tree
    cleanTree(root);

    return 0;
}

//____________________________________________________
//Create a new node and initialize its data
Node * createNode(int id, int monkeys)
{

    Node * result = (Node *) malloc(sizeof(Node));
    result->parent = NULL;
    result->children = NULL;
    result->id = id;
    result->monkeys = monkeys;
    result->numChildren = 0;

    //Return the newly created Node
    return result;
}

//____________________________________________________
//Add a node to the tree
Node * addNode(Node *root, int parentalID, int childID, int monkeys)
{
   //If the tree is empty, create a node
    if(root == NULL) return createNode(childID, monkeys);

    //Compare the IDs to the root to see if we're in the correct location
    if(root->id == parentalID) {

        //We are at the parent of the node we want to insert
        if(root->children == NULL) {
            //Root does not have any children
            Node ** newChildrenArray;
            Node ** oldChildrenArray = root->children;

            //Reallocate root's children array to accommodate more children
            newChildrenArray = (Node **) realloc(oldChildrenArray, 1 * sizeof(Node *));

            //Create the child node and adjust pointers
            newChildrenArray[0] = createNode(childID, monkeys);
            newChildrenArray[0]->parent = root;

            root->children = newChildrenArray;
            root->numChildren++;

            return root;

        } else {
            //Root has children
            Node ** newChildrenArray;
            Node ** oldChildrenArray = root->children;

            root->numChildren++;
            int numChildren = root->numChildren;

            //Reallocate root's children array to accommodate more children
            newChildrenArray = (Node **) realloc(oldChildrenArray, numChildren + sizeof(Node *));

            //Create the child node and adjust pointers
            newChildrenArray[(numChildren-1)] = createNode(childID, monkeys);
            newChildrenArray[(numChildren-1)]->parent = root;

            root->children = newChildrenArray;

            return root;
        }

    } else if(root->id == childID) {
        //ID is equal to the root. The childID should not exist
        printf("\n\n\nERROR\n\n\n");
        exit(0);

    } else {
        //We are at the wrong location. Search through the children
        for(int i = 0; i < root->numChildren; i++) {
            if(root->children[i]->id == parentalID) {
                root->children[i] = addNode(root->children[i], parentalID, childID, monkeys);
                return root;

            } else {
                root->children[i] = addNode(root->children[i], parentalID, childID, monkeys);
            }
        }
    }

    return root;
}

//____________________________________________________
//Function to find a node and sum all the monkeys that were encountered along the way
int sumMonkeys(Node *root, int target)
{
    int badID = 0;

   //If the tree is empty
    if(root == NULL) return 0;

    //We found the target id; return the number of monkeys
    if(root->id == target) return root->monkeys;

    //We are not in the correct location
    if(root->id != target) {

        if(root->children == NULL) {
            //Root does not have any children, thus, no moneys
            return 0;

        } else {
            //Root has children to look through
            for(int i = 0; i < root->numChildren; i++) {
                if(root->children[i]->id == target) {
                    //If one of the immediate children of the root is the desired destination, add the child and current root's monkeys together
                    return (root->monkeys + root->children[i]->monkeys);

                } else {
                    //Look at grandchildren
                    if(sumMonkeys(root->children[i], target) == 0) badID = 0;
                    //Recursively sum all the monkeys
                    if(sumMonkeys(root->children[i], target) != 0) return (root->monkeys + sumMonkeys(root->children[i], target));
                }
            }
        }
    }

    return badID;
}

//____________________________________________________
//Function to find a node in a tree and then return that node
Node * findNode(Node *root, int target)
{
    Node * targetNode;

   //If the tree is empty return null
    if(root == NULL) return NULL;

    //We found the desired root, return that root
    if(root->id == target) return root;

    //We are not looking at the correct node
    if(root->id != target) {

        //If the root is a leaf node
        if(root->children == NULL) {
            //The node we're looking for does not exist
            return NULL;

        } else {
            //Root has children to look through
            for(int i = 0; i < root->numChildren; i++) {
                if(root->children[i]->id == target) {
                    //One of the root's children was the node we were looking for
                    return root->children[i];

                } else {
                    //Look at grandchildren
                    targetNode = findNode(root->children[i], target);
                    if(targetNode != NULL) return targetNode;
                }
            }
        }
    }

    return targetNode;
}

//____________________________________________________
//Clean tree method
void cleanTree(Node *root)
{
    //If you are at the bottom
    if(root == NULL) return;

    //If you are at a leaf node, free that node, and then return
    if(root->children == NULL) {
        free(root);
        return;
    }

    //Recursively call cleanTree on all children of each node
    for(int i = 0; i < root->numChildren; i++) {
        cleanTree(root->children[i]);
    }
}
