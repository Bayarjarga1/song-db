#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*to use strcmp*/
#include <stdbool.h>


struct song{
    char song_title[50];
    char artist[50];
    char album[50];
    int released_year;
    char genre[50];
    int min;
    int sec;
    struct song *next;
};


struct song *loadDatabase();/*to load the song database from a file*/
void addsong(struct song **database);/*1. Add new entry to database.*/
void saveDatabase(struct song *database);/*to save current state of the song to the database*/
void listall(const struct song *database);/*2. List all the entries of the database.*/
void search(const struct song *database);/*3. Search and display songs by entries of the database.*/
void del(struct song **database);/*4. Delete any song from the database.*/
void freeDatabase(struct song *database);/*to free the memory allocated for the song database when the program exits.*/



int main(){
    struct song *songDatabase = loadDatabase();
    int choice;

    do {
        printf("\t\t\t\t================ Song Database Management ===============");
        printf("\n\n\t\t\t\t 1. Add new entry to database.\n");
        printf("\t\t\t\t 2. List all the entries of the database.\n");
        printf("\t\t\t\t 3. Search and display songs by entries of the database.\n");
        printf("\t\t\t\t 4. Delete any song from the database.\n");
        printf("\t\t\t\t 5. Exit.\n");
        printf("\t\t\t\t=========================================================\n\n");
        printf("\t\t\t\t");
        scanf("%d", &choice);
        printf("\n\n\n");

        switch(choice){
            case 1:
                addsong(&songDatabase);
                break;
            case 2:
                listall(songDatabase);
                break;
            case 3:
                search(songDatabase);
                break;
            case 4:
                del(&songDatabase);
                break;
            case 5:
                saveDatabase(songDatabase);
                freeDatabase(songDatabase);
                printf("\t\t\t\t Exiting the program. Goodbyeee!\n\n\n\n");
                break;
            default:
                printf("\t\t\t\t Invalid choice. Please enter a valid option.\n\n\n\n");
        }
    }while(choice!=5);

    return 0;
}


struct song *loadDatabase()
{
    FILE *file = fopen("song_database.txt", "r");
    if (file == NULL){
        return NULL; /*If the file doesnt exist, return an empty database*/
    }

    struct song *head = NULL;
    struct song *current = NULL;

    while (!feof(file)){
        struct song *newsong = (struct song *)malloc(sizeof(struct song));
        if (fscanf(file, "%99[^,], %99[^,], %99[^,], %d, %99[^,], %d, %d\n", newsong->song_title, newsong->artist, newsong->album,
                   &newsong->released_year, newsong->genre, &newsong->min, &newsong->sec)==7){
            newsong -> next = NULL;
            if(head == NULL){
                head = newsong;
                current = newsong;
            }else{
                current->next = newsong;
                current = newsong;
            }
        }else{
            free(newsong); /*if reading fails, free allocated memory*/
        }
    }

    fclose(file);
    return head;
}


void addsong(struct song **database)
{
    char another;
    do{
        struct song *newsong = (struct song *)malloc(sizeof(struct song));
        printf("\t\t\t\t======Add song entries=====\n\n\n");
        printf("\n\t\t\t Enter the title: ");

        getchar();/*to remove white space before enter a string*/

        gets(newsong->song_title);/*to get more that one word*/

        printf("\n\t\t\t Enter the artist: ");

        gets(newsong->artist); /*to get more than one word*/
        printf("\n\t\t\t Enter the album title: ");

        gets(newsong->album);
        printf("\n\t\t\t Enter the released year: ");
        scanf("%d", &newsong->released_year);
        printf("\n\t\t\t Enter the genre: ");
        getchar();
        gets(newsong->genre);
        printf("\n\t\t\t Enter the length of a song: ");
        printf("\n\t\t\t min: ");
        scanf("%d", &newsong->min);
        printf("\n\t\t\t sec: ");
        scanf("%d", &newsong->sec);
        printf("\n\t\t\t====================================\n");

        /*Adjust values if seconds exceed 60*/
        newsong->min += newsong->sec/60;
        newsong->sec = newsong->sec%60;

        newsong->next = *database;
        *database = newsong;

        printf("\n\n\n");

    }while(another=='y'||another=='Y');
}/*Reading entries until first whitespace */


void saveDatabase(struct song *database)
{
    FILE *file = fopen("song_database.txt", "w");
    if (file == NULL){
        printf("Error saving the database.\n");
        return;
    }

    while (database != NULL){
        fprintf(file, "%s, %s, %s, %d, %s, %d, %d\n", database->song_title, database->artist, database->album,
                   database->released_year, database->genre, database->min, database->sec);
        database = database->next;
    }

    fclose(file);
    printf("\t\t\t\tDatabase saved successfully.\n\n\n");
}


void listall(const struct song *database)
{
    printf("\t\t\t\t======================= Song List =======================\n\n");

    while(database != NULL){
        printf("\t\t\t\t Songs\n");
        printf("\t\t\t\t _____________________________________\n");

        printf("\n\t\t\t\t Song Title : %s", database->song_title);
        printf("\n\t\t\t\t Name Of The Artist : %s", database->artist);
        printf("\n\t\t\t\t Album Title : %s", database->album);
        printf("\n\t\t\t\t Released Year : %d", database->released_year);
        printf("\n\t\t\t\t Genre : %s", database->genre);
        printf("\n\t\t\t\t Song Length : %d:%02d", database->min, database->sec);
        printf("\n\t\t\t\t _____________________________________\n");
        database=database->next;
    }
    printf("\n\n\n\n");
    getchar();/*clear the input buffer*/
    /*getchar();wait for user input*/
}


void search(const struct song *database)
{
    char key[50];
    int f = 0; /*to stop printing unsuccessful search result many times*/

    printf("\t\t\t\t======Search Song=====\n\n");

    printf("\t\t\t Enter a key word to search: ");
    getchar();
    gets(key);


    while(database != NULL){
        if(strcmp(database->album, key)==0 || strcmp(database->artist, key)==0 || strcmp(database->genre, key)==0 ||
           strcmp(database->song_title, key)==0){
        printf("\n\t\t\t\t Song Title : %s", database->song_title);
        printf("\n\t\t\t\t Name Of The Artist : %s", database->artist);
        printf("\n\t\t\t\t Album Title : %s", database->album);
        printf("\n\t\t\t\t Released Year : %d", database->released_year);
        printf("\n\t\t\t\t Genre : %s", database->genre);
        printf("\n\t\t\t\t Song Length : %d:%d", database->min, database->sec);
        printf("\n\t\t\t\t _____________________________________\n\n\n\n");
        f = 1; /*if we find a song it will be*/
        }

    database = database->next;
}
        if(f==0){
        printf("\n\t\t\t\t Song has not found\n");
        printf("\t\t\t\t Press any key to exit.\n\n\n\n");
        }
}


void del(struct song **database)
{
    char delsong[50];
    struct song *prev = NULL;
    struct song *current = *database;

    printf("\t\t\t\t======Delete Song=====\n\n");
    printf("\t\t\t Enter the title of the song to delete: ");
    getchar();
    gets(delsong);


    while(current!=NULL){
        if(strcmp(current->song_title, delsong)==0){
            if (prev == NULL){
            *database = current->next;
        }else{
            prev->next = current->next;
            }
        free(current);
        printf("\t\t\t Song deleted successfully.\n\n\n\n");
        return;
    }

    prev = current;
    current = current->next;
    }

    printf("\t\t\t Song not found.\n");
    printf("\t\t\t\t Press any key to exit.\n\n\n\n");
}


void freeDatabase(struct song *database){
    while(database != NULL){
        struct song *temp = database;
        database = database->next;
        free(temp);
    }
}

