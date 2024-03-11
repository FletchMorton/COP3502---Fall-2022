#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Part 1: set up struct that can store information
typedef struct person_t person_t;
typedef struct likes_t likes_t;

struct person_t
{
    char *name;
    person_t *likes;
    person_t *dislikes;
    likes_t *liked_by; //for part 4
};

//part 4:linked list of likes
//
struct likes_t
{
    person_t *person;
    likes_t *next;
};

person_t *make_person(char *name);
void free_person(person_t *person);
person_t *add_like(person_t *person, person_t *liked);
void print_person(person_t *person);

int main()
{
    //part 2: write code to set up the data int he problem
    person_t *greg = make_person("Greg");
    person_t *steve = make_person("Steve");
    person_t *matt = make_person("Matt");

    //steve->likes = matt;
    //matt->likes = greg;

    add_like(steve, matt);
    add_like(matt, greg);

    print_person(greg);
    print_person(steve);
    print_person(matt);

    //part 3: make greg dislike steve only knowing steve's address

    steve->likes->likes->dislikes = steve;

    printf("\n");
    print_person(greg);
    print_person(steve);
    print_person(matt);

    free_person(matt);
    free_person(steve);
    free_person(greg);

    return 0;
}

void free_person(person_t *person)
{

    likes_t *next;
    likes_t *curr = person->liked_by;

    while(curr) {
        next = curr->next;
        free(curr);
        curr = next;
    }

    free(person);
}

person_t *make_person(char *name)
{
    person_t *person = (person_t *) calloc(1, sizeof(person_t));

    person->name - (char *) malloc(strlen(name) + 1);
    person->likes = NULL;
    person->dislikes = NULL;
    strcpy(person->name, name);

    return person;
}

person_t *add_like(person_t *person, person_t *liked)
{
    person->likes = liked;

    //part 4: linked list of likers
    //
    likes_t *like = (likes_t *) malloc(sizeof(likes_t));

    like->person = person;

    like->next = liked->liked_by;
    liked->liked_by = like;
}

void print_person(person_t *person)
{
    printf("%s", person->name);

    if(person->likes) {
        printf(" likes %s", person->likes->name);
    }

    if(person->dislikes) {
        printf(" dislikes %s", person->dislikes->name);
    }

    if(person->liked_by) {
        printf(" liked by");
        likes_t *liker = person->liked_by;
        while(liker) {
            printf(" %s", liker->person->name);
            liker = liker->name;
        }
    }

    printf("\n");
}
