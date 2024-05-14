#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define MAX 250 //considering that the film with the longest name in the world has 222 characters
//set colors for the terminal
#define COLOR_RED     "\e[91m"
#define COLOR_GREEN   "\e[32m"
#define COLOR_YELLOW  "\e[33m"
#define COLOR_BLUE    "\e[34m"
#define COLOR_MAGENTA "\e[35m"
#define COLOR_CYAN    "\e[36m"
#define COLOR_RESET   "\e[0m"

/*TO FIX:
SAGA LENGTH DOESN'T INDICATE THE ACTUAL SAGA LENGTH
ALL MOVIES HAVE THE SAME ID, ONLY MOVIES IN THE SAME SAGA SHOULD HAVE THE SAME ID, DIFFRENT SAGAS SHOULD HAVE DIFFRENT IDs*/
//saga len is never initialized to 0 that's why it's giving random values

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

int GetSagaLenght(FILE *fptr, Film film)
{
    Film temp;
    int sagaLength = 0; //initialize the length of the saga
    while(fread(&temp, sizeof(Film), 1, fptr))
    {
        if(temp.ID == film.ID) sagaLength++; //increment the length of the saga
    }

    return sagaLength + 1;
}

Film SearchIfSagaExistsInFile(FILE *fptr, char* sagaName)
{
    Film film;
    rewind(fptr);
    printf("Checking if the saga exists\n\n");
    while(!feof(fptr))
    {
        fread(&film, sizeof(Film), 1, fptr);
        //perror("Error: ");
        //printf("%s -> %s\n", film.sagaName, sagaName);
        if(strcmp(film.sagaName, sagaName) == 0)
        {
            rewind(fptr);
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

void ResetSagaCounter(int num, int ID, FILE *fptr)
{
    Film film;
    for(int i=0; i<CountFilmsInFile(fptr); i++)
    {
        fread(&film, sizeof(Film), 1, fptr);
        if(film.ID == ID) film.sagaLength = num;
        fseek(fptr, -sizeof(Film), SEEK_CUR);
        fwrite(&film, sizeof(Film), 1, fptr);
    
    }
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
            fflush(stdin);
            fseek(fptr, 0, SEEK_SET);
            fclose(fptr);
            fptr = fopen("lab_watchlist.txt", "rb+"); //open the file in read mode
            saga = SearchIfSagaExistsInFile(fptr, film.sagaName); 
            fclose(fptr);
            fptr = fopen("lab_watchlist.txt", "ab+"); //open the file in append mode
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

            film.sagaLength = GetSagaLenght(fptr, film); //get the length of the saga
            fclose(fptr); //close the file
            fptr = fopen("lab_watchlist.txt", "ab+");
            ResetSagaCounter(film.sagaLength, film.ID, fptr); //memory leak?
        }
        else
        {
            film.isSaga = false; //set the film as not part of a saga
            for(int i=0; i<strlen(film.name); i++) film.sagaName[i] = film.name[i]; //set the name of the saga as the name of the film
            fclose(fptr);
            fptr = fopen("lab_watchlist.txt", "ab+"); //open the file in append mode
            film.ID = CountFilmsInFile(fptr) + 1; //set the ID of the film as the number of films in the file
            film.sagaLength = GetSagaLenght(fptr, film); //get the length of the saga
            film.sagaPoints = film.points; //set the points of the saga as the points of the film
        }
        
        //error message if the answer is not valid
        if(answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N') printf(COLOR_RED "Please Enter a valid option\n" COLOR_RESET);

    }while(answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N');
    //add the film to the file
    fwrite(&film, sizeof(Film), 1, fptr);
    printf(COLOR_GREEN "Film Added Successfully\n" COLOR_RESET);

    fclose(fptr);
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

void PrintListInTermianl(FILE *fptr)
{
    Film film;
    //get saga length for each movie

    while(fread(&film, sizeof(Film), 1, fptr))
    {
        printf("Name: %s\n", film.name);
        printf("Genre: %s\n", film.genre);
        printf("Points: %.2f\n", film.points);
        if(film.isSaga)
        {
            printf("Saga: %s\n", film.sagaName);
            printf("Saga Points: %.2f\n", film.sagaPoints);
            printf("Number of Films in Saga: %d\n", film.sagaLength);
        }
        printf("ID: %d\n", film.ID);
        printf("===================================================\n");
    }

    fseek(fptr, 0, SEEK_SET);

    Wait();
    return;
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
                system("cls");
                movies = fopen(filmfilename, "rb"); 
                PrintListInTermianl(movies);
                fclose(movies);
            break; 
            case 2: 
                system("cls");
                movies = fopen(filmfilename, "ab"); 
                AddFilmToList(movies);
                Wait(); 
            break; 
            case 3: 
                
            break;  
            case 2303: 
                system("cls");
                printf("===================================================\nWelcome to the Secret Debugging Menu\n===================================================\n");
                int debug;
                do
                {
                    printf("What would you like to do?\n\n1. Delete Film List\n2.Count Films in FIle\n\n0. Go Back\n");
                    scanf("%d", &debug);
                    fflush(stdin);
                    switch(debug)
                    {
                        case 0: 
                            system("cls");
                        break;

                        case 1: 
                            system("cls");
                            movies = fopen(filmfilename, "wb"); 
                            fclose(movies);
                            printf("===================================================\nFilm List Deleted\n===================================================\n");
                            Wait();   
                        break;

                        case 2: 
                            system("cls");
                            movies = fopen(filmfilename, "rb+"); 
                            printf("Number of Films in File: %d\n", CountFilmsInFile(movies));
                            fclose(movies);
                            Wait();
                        break;
                        
                        default:
                            printf("===================================================\n!ERROR!\nPlease Enter a valid option\n===================================================\n");
                        break;
                    }
                }while(debug!=0);
            default:
                system("cls");
                printf("===================================================\n!ERROR!\nPlease Enter a valid option\n===================================================\n");
                Wait(); 
            break; 
        }//end of switch
    }while(choice!=0);

    system("cls"); 

    return 0; 
}