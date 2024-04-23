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
    int ID; //movies with the same ID are part of the same saga
    char name[MAX]; //can be made variable size??
    char genre[MAX]; //genre of the film
    char sagaName[MAX]; //name of the saga the film is part of
    float points; //points of the film
    int index; //index of the film in the list
    bool isSaga; //if the film is part of a saga or not 
    int sagaLength; //number of films in the saga
    float sagaPoints; //average of the points of the films in the saga
} Film;

bool sagaExists = false; //variable to check if the saga exists
int censor = 0; //variable to change the message of the wait function

int CountFilmsInFile(FILE *fptr)
{
    Film film;
    int count = 0;
    while(fread(&film, sizeof(Film), 1, fptr))
    {
        count++;
    }
    return count;
}

Film SearchIfSagaExistsInFile(FILE *fptr, char* sagaName)
{
    Film film;
    while(fread(&film, sizeof(Film), 1, fptr))
    {
        if(strcmp(film.sagaName, sagaName) == 0)
        {
            film.sagaLength++;
            film.sagaPoints += film.points;
            film.points = film.sagaPoints / film.sagaLength;
            fseek(fptr, -sizeof(Film), SEEK_CUR);
            fwrite(&film, sizeof(Film), 1, fptr);
            sagaExists = true;
            return film;
        }
    }

    sagaExists = false;
    return film;
}

void AddFilmToList(FILE *fptr)
{
    Film film; //create a film object
    Film saga; //create a saga object
    char answer; //variable to store the answer of the user
    printf("Enter the name of the film: ");
    fgets(film.name, MAX, stdin); //get the name of the film
    printf("Enter the genre of the film: ");
    fgets(film.genre, MAX, stdin); //get the genre of the film
    printf("Enter the score of the film: ");
    scanf("%f", &film.points); //get the points of the film
    do
    {
        printf("Is this film part of a saga? [y/n]: ");
        scanf(" %c", &answer); //get the answer of the user
        fflush(stdin);
        if(answer == 'y' || answer == 'Y') //if the answer is yes
        {
            film.isSaga = true; //set the film as part of a saga
            printf("Enter the name of the saga: ");
            fgets(film.sagaName, MAX, stdin); //get the name of the saga
            saga = SearchIfSagaExistsInFile(fptr, film.sagaName); 
            if(sagaExists) //search if the saga exists in the file
            {
                printf(COLOR_GREEN "Saga Found\n\nName:" COLOR_RESET " %s\n", saga.sagaName);
                film.ID = saga.ID; //set the ID of the film as the ID of the saga
            }
            else
            {
                printf("Saga Not Found\n\nCreating a new Saga\n\n");
                film.ID = CountFilmsInFile(fptr) + 1; //set the ID of the film as the number of films in the file
            }
        }
        else
        {
            film.isSaga = false; //set the film as not part of a saga
            film.ID = CountFilmsInFile(fptr) + 1; //set the ID of the film as the number of films in the file
            film.sagaPoints = film.points; //set the points of the saga as the points of the film
        }
        
        //error message if the answer is not valid
        if(answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N') printf(COLOR_RED "Please Enter a valid option\n" COLOR_RESET);

    }while(answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N');
    //add the film to the file
    fwrite(&film, sizeof(Film), 1, fptr);
    printf(COLOR_GREEN "Film Added Successfully\n" COLOR_RESET);
}

void Wait() //short function to return to the menu
{
    if(censor != 1) printf(COLOR_GREEN "\n[press ENTER to return to the selection menu]\n" COLOR_RESET);
    fflush(stdin);
    getchar(); //Wait for enter key
    system("cls"); //clear the terminal before starting the program
}

void CreateFileIfNotExisting(char* filename)
{
    FILE *file;
    file = fopen(filename, "rb");
    if(file == NULL)
    {
        file = fopen(filename, "wb");
        fclose(file);
    } 
    else
    {
        fclose(file);
    }   
}

int main()
{
    system("cls"); // clear the terminal before strating the program
    FILE *movies; //file pointer
    char filmfilename[] = "lab_watchlist.txt"; //name of the file

    int choice; //variable to store the choice of the user

    CreateFileIfNotExisting(filmfilename);  //create the file if it does not exist

    do
    {
        printf("What would you like to do?\n\n1. Check Film List\n2. Add Film to watchlist\n3. Modify Film\n\n0. Quit\n");
        scanf("%d", &choice);
        fflush(stdin);
        switch(choice)
        {
            case 0: 
                printf("===================================================\nEXITING PROGRAM NOW\n===================================================\n");
                printf("PRESS ENTER TO TERMINATE EXECUTION\n===================================================\n");
                censor = 1;
                Wait();
            break; 
            case 1: 
                
            break; 
            case 2: 
                system("cls");
                movies = fopen(filmfilename, "ab"); 
                AddFilmToList(movies);
                fclose(movies);
                Wait(); 
            break; 
            case 3: 
                
            break;  
            default:
                system("cls");
                printf("===================================================\n!ERROR!\nPlease Enter a valid option\n===================================================\n");
                Wait();
            break; 
        }
    }while(choice!=0);

    system("cls"); 

    return 0; 
}