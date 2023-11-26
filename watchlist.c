#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 40

typedef struct
{
    char name[MAX]; //can be made variable size??
    float points; 
    int index; 
} Film;

typedef struct
{
    char name[MAX];
    float points; 
    int index; 
} Game;

int censor = 0; //1 = true, 0 = false

void OrderByIndex(FILE *fptr)
{
    Film *films;
    //calculate the number of films in the file with fseek
    fseek(fptr, 0, SEEK_END);
    int numFilms = ftell(fptr) / sizeof(Film);
    films = (Film*)malloc(numFilms * sizeof(Film));
    rewind(fptr);
    fread(films, sizeof(Film), numFilms, fptr);

    //sort the array of films by index
    Film temp;
    for(int i = 0; i < numFilms; i++)
    {
        for(int j = 0; j < numFilms; j++)
        {
            if(films[i].points > films[j].points)
            {
                temp = films[i];
                films[i] = films[j];
                films[j] = temp;
            }
        }
    }

    //write the sorted array back to the file
    rewind(fptr);

    for(int i = 0; i < numFilms; i++)
    {
        films[i].index = i+1;
        fwrite(&films[i], sizeof(Film), 1, fptr);
    }

}

void OrderGamesByIndex(FILE *fptr)
{
    Game *games;
    //calculate the number of films in the file with fseek
    fseek(fptr, 0, SEEK_END);
    int numGames = ftell(fptr) / sizeof(Game);
    games = (Game*)malloc(numGames * sizeof(Game));
    rewind(fptr);
    fread(games, sizeof(Game), numGames, fptr);

    //sort the array of films by index
    Game temp;
    for(int i = 0; i < numGames; i++)
    {
        for(int j = 0; j < numGames; j++)
        {
            if(games[i].points > games[j].points)
            {
                temp = games[i];
                games[i] = games[j];
                games[j] = temp;
            }
        }
    }

    //write the sorted array back to the file
    rewind(fptr);

    for(int i = 0; i < numGames; i++)
    {
        games[i].index = i+1;
        fwrite(&games[i], sizeof(Game), 1, fptr);
    }

}

void Wait() //short function to return to the menu
{
    if(censor != 1) printf("\n[press ENTER to return to the selection menu]\n");
    fflush(stdin);
    getchar(); //Wait for enter key
}

void CheckFilmList(FILE *fptr)
{
    Film film;
    while(fread(&film, sizeof(Film), 1, fptr) == 1)
    {
        printf("%d. %s\nScore: %.1f\n\n", film.index, film.name, film.points);
    }
}

void AddFilmToList(FILE *fptr)
{
    Film film; 
    printf("Enter the name of the film: ");
    fgets(film.name, MAX-1, stdin);
    for(int i = 0; i < MAX; i++)
    {
        if(film.name[i] == '\n')
        {
            film.name[i] = '\0';
            break;
        }
    }
    printf("Enter the score of the film: ");
    scanf("%f", &film.points);
    //fprintf(fptr, "%s %f\n", film.name, film.points);
    fwrite(&film, sizeof(Film), 1, fptr);
}

void ModifyFilmList(FILE *fptr)
{
    //ask for index
    int index, choice;
    printf("Enter the index of the film you want to modify: ");
    scanf("%d", &index);
    fflush(stdin);
    //find the film with that index
    Film film;
    while(fread(&film, sizeof(Film), 1, fptr) == 1)
    {
        if(film.index == index)
        {
            printf("Film found!\n");
            printf("Name: %s\nScore: %.1f\n", film.name, film.points);
            printf("Do you want to modify this film? (y/n): ");
            char confirm;
            scanf("%c", &confirm);
            fflush(stdin);
            if(confirm == 'y')
            {
                printf("1. Modify name\n2. Modify score\n3. Delete film\n");
                scanf("%d", &choice);
                fflush(stdin);
                switch(choice)
                {
                    case 1:
                        printf("Enter the new name: ");
                        fgets(film.name, MAX-1, stdin);
                        for(int i = 0; i < MAX; i++)
                        {
                            if(film.name[i] == '\n')
                            {
                                film.name[i] = '\0';
                                break;
                            }
                        }
                        fseek(fptr, -sizeof(Film), SEEK_CUR);
                        fwrite(&film, sizeof(Film), 1, fptr);
                        printf("Film name modified!\n");
                        break;
                    case 2:
                        printf("Enter the new score: ");
                        scanf("%f", &film.points);
                        fseek(fptr, -sizeof(Film), SEEK_CUR);
                        fwrite(&film, sizeof(Film), 1, fptr);
                        printf("Film score modified!\n");
                        break;
                    case 3:
                        printf("Are you sure you want to delete this film? (y/n): ");
                        scanf(" %c", &confirm);  // Note the space before %c to consume the newline character
                        fflush(stdin);

                        if (confirm == 'y') 
                        {
                            // Open the original file for reading
                            FILE *fptr = fopen("watchlist.txt", "rb");
                            if (fptr == NULL) 
                            {
                                printf("Error opening file!");
                                return;
                            }

                            // Open a new file for writing
                            FILE *new_fptr = fopen("new_watchlist.txt", "wb");
                            if (new_fptr == NULL) 
                            {
                                printf("Error opening file!");
                                fclose(fptr);  // Close the original file before returning
                                return;
                            }

                            // Read each film from the original file and write it to the new file
                            Film film;
                            while (fread(&film, sizeof(Film), 1, fptr) == 1) 
                            {
                                if (film.index != index) 
                                {
                                    fwrite(&film, sizeof(Film), 1, new_fptr);
                                }
                            }

                            // Close both files
                            fclose(fptr);
                            fclose(new_fptr);

                            // Delete the original file
                            if (remove("watchlist.txt") != 0) 
                            {
                                printf("Error deleting the original file!\n");
                                return;
                            }

                            // Rename the new file to the original file name
                            if (rename("new_watchlist.txt", "watchlist.txt") != 0) 
                            {
                                printf("Error renaming the new file!\n");
                                return;
                            }

                            printf("Film deleted successfully!\n");
                        } else {
                            printf("Film not deleted!\n");
                        }
                        break;

                    default:
                        printf("Invalid option!\n");
                        break;

                }
            }
        }
    }
}

void CheckGameList(FILE *fptr)
{
    Game game;
    while(fread(&game, sizeof(Game), 1, fptr) == 1)
    {
        printf("%d. %s\nScore: %.1f\n\n", game.index, game.name, game.points);
    }
}

void AddGameToList(FILE *fptr)
{
    Game game; 
    printf("Enter the name of the game: ");
    fgets(game.name, MAX-1, stdin);
    for(int i = 0; i < MAX; i++)
    {
        if(game.name[i] == '\n')
        {
            game.name[i] = '\0';
            break;
        }
    }
    printf("Enter the score of the game: ");
    scanf("%f", &game.points);
    //fprintf(fptr, "%s %f\n", film.name, film.points);
    fwrite(&game, sizeof(Game), 1, fptr);
}

void ModifyGameList()
{
    printf("FEATURE NOT IMPLEMENTED YET\n");
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
    FILE *filmFile, *gameFile;
    int choice; 
    char confirm;

    CreateFileIfNotExisting("watchlist.txt");
    CreateFileIfNotExisting("gamelist.txt");

    do
    {
        printf("What would you like to do?\n\n1. Check Film List\n2. Add Film to watchlist\n3. Modify Film Score\n\n4. Check Game List\n5. Add Game to game list\n6. Modify Game Score\n\n0. Quit\n");
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
                filmFile = fopen("watchlist.txt", "r+b");
                OrderByIndex(filmFile);
                fclose(filmFile);
                filmFile = fopen("watchlist.txt", "rb");
                CheckFilmList(filmFile); 
                fclose(filmFile);
                Wait(); 
            break; 
            case 2: 
                filmFile = fopen("watchlist.txt", "ab"); 
                AddFilmToList(filmFile);
                fclose(filmFile);
                Wait();  
            break; 
            case 3: 
                filmFile = fopen("watchlist.txt", "r+b");
                ModifyFilmList(filmFile);
                fclose(filmFile);
                Wait(); 
            break;  
            case 4: 
                gameFile = fopen("gamelist.txt", "r+b"); 
                OrderByIndex(gameFile);
                fclose(gameFile);
                gameFile = fopen("gamelist.txt", "rb"); 
                CheckGameList(gameFile); 
                fclose(gameFile); 
                Wait(); 
            break; 
            case 5: 
                gameFile = fopen("gamelist.txt", "ab"); 
                AddGameToList(gameFile);
                fclose(gameFile); 
                Wait();  
            break; 
            case 6: 
                ModifyGameList(); 
                Wait(); 
            break; 
            default:
                printf("===================================================\n!ERROR!\nPlease Enter a valid option\n===================================================\n");
                Wait();
            break; 
        }
    }while(choice!=0);

    return 0; 
}