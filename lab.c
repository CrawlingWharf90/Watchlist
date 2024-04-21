#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define MAX 40
//set colors for the terminal
#define COLOR_RED     "\e[91m"
#define COLOR_GREEN   "\e[32m"
#define COLOR_YELLOW  "\e[33m"
#define COLOR_BLUE    "\e[34m"
#define COLOR_MAGENTA "\e[35m"
#define COLOR_CYAN    "\e[36m"
#define COLOR_RESET   "\e[0m"

typedef struct
{
    char name[MAX]; //can be made variable size??
    char genre[MAX]; //can be made variable size??
    float points; 
    int index; 
} Film;

typedef struct FilmNode 
{
    Film film;
    struct FilmNode* next;
} FilmNode;

typedef struct Saga //a saga is a list of films, a saga can also be made of a single film
{
    FilmNode* films;
    int score; //score of the saga as the average of the scores of the films in the saga
    char name[MAX]; //name of the saga
    int index; //index of the saga in the list
    struct Saga* next;
} Saga;

typedef struct SagaList 
{
    Saga* head;
} SagaList;

int CountSagas(SagaList sagaList) //count how many sagas are in the list
{
    int count = 0;
    Saga* currentSaga = sagaList.head;
    while (currentSaga != NULL) 
    {
        count++;
        currentSaga = currentSaga->next;
    }
    return count;
}

int CountFilmsInSaga(Saga saga) //count how many Films are in each saga
{
    int count = 0;
    FilmNode* currentFilm = saga.films;
    while (currentFilm != NULL) 
    {
        count++;
        currentFilm = currentFilm->next;
    }

    return count;
}

void AddFilm(SagaList* sagaList, char* sagaName) 
{
    system("cls");
    Film film;
    char confirm; //confirmation for adding a film
    // Check if the saga already exists
    Saga* currentSaga = sagaList->head;
    while (currentSaga != NULL) {
        if (strcmp(currentSaga->name, sagaName) == 0) {
            printf("Saga Found\n\nIndex: %d\nName: %s\n", currentSaga->index, currentSaga->name);
            // Saga exists, add the film to it
            printf("Do You Want to Add a Film to this Saga? (Y/N)\n");
            scanf(" %c", &confirm);
            if (confirm != 'Y' && confirm != 'y') {
                return;
            }
            printf("Enter the Film Name: ");
            scanf("%s", film.name);
            printf("Enter the Genre: ");
            scanf("%s", film.genre);
            printf("Enter the Points: ");
            scanf("%f", &film.points);
            film.index = CountFilmsInSaga(*currentSaga) + 1;
            FilmNode* newFilmNode = (FilmNode*)malloc(sizeof(FilmNode));
            newFilmNode->film = film;
            newFilmNode->next = currentSaga->films;
            currentSaga->films = newFilmNode;
            return;
        }
        currentSaga = currentSaga->next;
    }

    // Saga doesn't exist, create a new saga and add it to the saga list
    Saga* newSaga = (Saga*)malloc(sizeof(Saga));
    strcpy(newSaga->name, sagaName);
    newSaga->index = CountSagas(*sagaList) + 1;
    printf("New Saga Created: %d. %s\n", newSaga->index, sagaName);
    newSaga->films = NULL;

    newSaga->next = sagaList->head;
    sagaList->head = newSaga;
    // Add the film to the new saga
    FilmNode* newFilmNode = (FilmNode*)malloc(sizeof(FilmNode));
    printf("Enter the Film Name: ");
    scanf("%s", film.name);
    printf("Enter the Genre: ");
    scanf("%s", film.genre);
    printf("Enter the Points: ");
    scanf("%f", &film.points);
    film.index = 1; //if there is no film in the saga, the index will be 1 since it's the only one
    newFilmNode->film = film;
    newFilmNode->next = newSaga->films;
    newSaga->films = newFilmNode;

    //write all sagas into a file
    FILE* file = fopen("sagas.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    currentSaga = sagaList->head;
    while (currentSaga != NULL) {
        fprintf(file, "%s\n", currentSaga->name);
        FilmNode* currentFilm = currentSaga->films;
        while (currentFilm != NULL) {
            fprintf(file, "%s %f %d\n", currentFilm->film.name, currentFilm->film.genre, currentFilm->film.points, currentFilm->film.index);
            currentFilm = currentFilm->next;
        }
        currentSaga = currentSaga->next;
    }

    fclose(file);
}

void GetSagaScore(SagaList *SagaList) 
{
    Saga* currentSaga = SagaList->head;
    while (currentSaga != NULL) 
    {
        FilmNode* currentFilm = currentSaga->films;
        float totalPoints = 0;
        int filmCount = 0;
        while (currentFilm != NULL) 
        {
            totalPoints += currentFilm->film.points;
            filmCount++;
            currentFilm = currentFilm->next;
        }
        currentSaga->score = totalPoints / filmCount;
        currentSaga = currentSaga->next;
    }
}

void WriteSagasIntoWatchlistFile() 
{
    FILE *file = fopen("lab_watchlist.txt", "wb");
    FILE *sagaFile = fopen("sagas.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    if (sagaFile == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    //order sagas by score
    SagaList sagaList;
    //get saga list from sagas.txt
    char sagaName[MAX];
    while (fscanf(sagaFile, "%s", sagaName) != EOF) 
    {
        AddFilm(&sagaList, sagaName);
    }
    GetSagaScore(&sagaList);
    //sort sagas by score
    Saga* currentSaga = sagaList.head;
    while (currentSaga != NULL) 
    {
        Saga* nextSaga = currentSaga->next;
        while (nextSaga != NULL) 
        {
            if (currentSaga->score < nextSaga->score) 
            {
                Saga temp = *currentSaga;
                *currentSaga = *nextSaga;
                *nextSaga = temp;
            }
            nextSaga = nextSaga->next;
        }
        currentSaga = currentSaga->next;
    }
    //uotput ordered sagas to watchlist file
    currentSaga = sagaList.head;
    if(currentSaga == NULL)
    {
        fprintf(file, "The List Is Empty\nGo Add Some Films\n");
        return;
    }
    while (currentSaga != NULL)  //rember to change formatting based on if the saga has more than one film ---------------------
    {
        fprintf(file, "%s\n", currentSaga->name);
        FilmNode* currentFilm = currentSaga->films;
        while (currentFilm != NULL) 
        {
            fprintf(file, "%d %s %s %f\n", currentFilm->film.index, currentFilm->film.name, currentFilm->film.genre, currentFilm->film.points);
            currentFilm = currentFilm->next;
        }
        currentSaga = currentSaga->next;
    }
    
    fclose(file);
    fclose(sagaFile);

    return; 
}

void ViewWatchlist() 
{
    FILE *file = fopen("lab_watchlist.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    char line[MAX];
    while (fgets(line, MAX, file) != NULL) 
    {
        printf("%s", line);
    }
    fclose(file);
}

int main()
{
    //initialize the saga list
    SagaList* sagaList = (SagaList*)malloc(sizeof(SagaList));
    sagaList->head = NULL;
    int choice = 0;
    char userInput[MAX]; //used to get the name of the saga/film

    printf("What would you like to do?\n\n1. Check Film List\n2. Add a Film\n3. Modify List\n0. Exit\n");
    scanf("%d", &choice);
    do
    {
        switch (choice)
        {
            case 1:
                system("cls");
                ViewWatchlist();
                break;
            case 2:
                system("cls"); 
                printf("Enter The Name of The Saga/Film: \n");
                scanf("%s", userInput);
                AddFilm(sagaList, userInput);
                break;
            case 3:
                printf("Modifying List\n");
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
        printf("What would you like to do?\n\n1. Check Film List\n2. Add a Film\n3. Modify List\n0. Exit\n");
        scanf("%d", &choice);
    }while(choice!=0); 

    return 0; 
}