#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define MAX 40
//set colors for the terminal
/*#define COLOR_RED     "\e[0;31m"
#define COLOR_GREEN   "\e[32m"
#define COLOR_YELLOW  "\e[33m"
#define COLOR_BLUE    "\e[34m"
#define COLOR_MAGENTA "\e[35m"
#define COLOR_CYAN    "\e[36m"
#define COLOR_RESET   "\e[0m"*/

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


int CountElements(FILE *fptr)
{
    Film *films;
    //calculate the number of films in the file with fseek
    fseek(fptr, 0, SEEK_END);
    int numFilms = ftell(fptr) / sizeof(Film);

    return numFilms;
}

int CountGames(FILE *fptr)
{
    Game *game;
    fseek(fptr, 0, SEEK_END);
    int numGames = ftell(fptr) / sizeof(Game);

    return numGames;
}

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

void ScaleAllMovies(FILE *fptr)
{
    int numFilms = CountElements(fptr);

    if (numFilms <= 0)
    {
        printf("No films to scale.\n");
        return;
    }

    Film *films = (Film *)malloc(numFilms * sizeof(Film));
    if (films == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    // Read all films into the array and find the highest score
    float highestScore = 0;
    rewind(fptr);
    for (int i = 0; i < numFilms; i++)
    {
        if (fread(&films[i], sizeof(Film), 1, fptr) == 1)
        {
            if (films[i].points > highestScore)
            {
                highestScore = films[i].points;
            }
        }
    }

    // Scale all scores in the array
    for (int i = 0; i < numFilms; i++)
    {
        films[i].points = (films[i].points / highestScore) * 100;
    }

    // Write the modified array back to the file
    rewind(fptr);
    for (int i = 0; i < numFilms; i++)
    {
        fwrite(&films[i], sizeof(Film), 1, fptr);
    }

    // Free the allocated memory
    free(films);
}

void ScaleAllGames(FILE *fptr)
{
    int numGames = CountGames(fptr);

    if (numGames <= 0)
    {
        printf("No games to scale.\n");
        return;
    }

    Game *games = (Game *)malloc(numGames * sizeof(Game));
    if (games == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    // Read all films into the array and find the highest score
    float highestScore = 0;
    rewind(fptr);
    for (int i = 0; i < numGames; i++)
    {
        if (fread(&games[i], sizeof(Game), 1, fptr) == 1)
        {
            if (games[i].points > highestScore)
            {
                highestScore = games[i].points;
            }
        }
    }

    // Scale all scores in the array
    for (int i = 0; i < numGames; i++)
    {
        games[i].points = (games[i].points / highestScore) * 100;
    }

    // Write the modified array back to the file
    rewind(fptr);
    for (int i = 0; i < numGames; i++)
    {
        fwrite(&games[i], sizeof(Game), 1, fptr);
    }

    // Free the allocated memory
    free(games);
}

void Wait() //short function to return to the menu
{
    if(censor != 1) printf(/*COLOR_GREEN*/ "\n[press ENTER to return to the selection menu]\n" /*COLOR_RESET*/);
    fflush(stdin);
    getchar(); //Wait for enter key
    system("cls"); //clear the terminal before starting the program
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
    int searchBy, chooseToModify, index, choice;
    char name[MAX];
    char c; 
    bool found = false;  
    Film film, chosenFilm; 
    
    do
    {
        printf("1. Search and modify sepcific film\n2. Modify all films\n0. Go back\n");
        scanf(" %d", &chooseToModify);
        switch (chooseToModify)
        {
        case 1: 
            break;
        case 2:
                printf("Scale all movies to be between 0 and 100\n");
                printf("Are you sure you want to modify all films? (y/n):\nYOU CANNOT UNDO THIS ACTION\n");
                scanf(" %c", &c);  // Note the space before %c to consume the newline character
                if(c=='y') ScaleAllMovies(fptr); 
                else printf("Films not modified!\n");
                return; //exit the function
            break; 
        default:
            printf("Please Insert a Valid Option\n\n");
            break;
        }
    }while(chooseToModify != 1); 

    do
    {
        system("cls"); //clear the terminal before starting the program
        printf("1. Search movie by index\n2. Search movie by name\n0. Go back\n"); 
        scanf(" %d", &searchBy); 
        switch (searchBy)
        {
            case 1:
                printf("Enter the index of the movie you want to modify: ");
                scanf("%d", &index);
                fflush(stdin);
                // find the film with that index
                while (fread(&film, sizeof(Film), 1, fptr) == 1)
                {
                    if (film.index == index)
                    {
                        chosenFilm = film;
                        found = true;
                        break; // Exit the loop once the film is found
                    }
                }
                break;

            case 2:
                printf("Enter the name of the movie you want to modify: ");
                fflush(stdin);
                fgets(name, MAX-1, stdin);
                for(int i = 0; i < MAX; i++)
                {
                    if(name[i] == '\n')
                    {
                        name[i] = '\0';
                        break;
                    }
                }
                // find the film with that name
                rewind(fptr); // Reset the file pointer to the beginning
                while (fread(&film, sizeof(Film), 1, fptr) == 1)
                {
                    if (strcmp(film.name, name) == 0)
                    {
                        chosenFilm = film;
                        found = true;
                        break; // Exit the loop once the film is found
                    }
                }
                break;

            case 0:
                Wait();
                return;

            default:
                printf(/*COLOR_RED*/ "Please Insert a Valid Option\n\n" /*COLOR_RESET*/);
                break;
        }
    }while(searchBy < 0 || searchBy > 2);

    if(found == true)
    {
        printf(/*COLOR_GREEN*/ "Film found!\n" /*COLOR_RESET*/);
        printf("Name: %s\nScore: %.1f\n", chosenFilm.name, chosenFilm.points);
        printf("Do you want to modify this film? (y/n): ");
        char confirm;
        scanf("%c", &confirm);
        fflush(stdin);
        if(confirm == 'y')
        {
            system("cls"); //clear the terminal before starting the program
            printf("1. Modify name\n2. Modify score\n3. Delete film\n");
            scanf("%d", &choice);
            fflush(stdin);
            switch(choice)
            {
                case 1:
                    //print chosen film name
                    printf("Chosen film name: %s\n", chosenFilm.name); 
                    printf("Enter the new name: ");
                    fgets(chosenFilm.name, MAX-1, stdin);
                    for(int i = 0; i < MAX; i++)
                    {
                        if(chosenFilm.name[i] == '\n')
                        {
                            chosenFilm.name[i] = '\0';
                            break;
                        }
                    }
                    fseek(fptr, -sizeof(Film), SEEK_CUR); // Move the file pointer back to the correct position
                    fwrite(&chosenFilm, sizeof(Film), 1, fptr);
                    printf(/*COLOR_GREEN*/ "Film name modified!\n" /*COLOR_RESET*/);
                    return; //exit the function
                    break; 
                case 2:
                    printf("Enter the new score: ");
                    scanf("%f", &chosenFilm.points);
                    fseek(fptr, -sizeof(Film), SEEK_CUR); // Move the file pointer back to the correct position
                    fwrite(&chosenFilm, sizeof(Film), 1, fptr);
                    printf(/*COLOR_GREEN*/ "Film score modified!\n" /*COLOR_RESET*/);
                    return; //exit the function
                    break;
                case 3:
                    printf(/*COLOR_YELLOW*/ "Are you sure you want to delete this film? (y/n):" /*COLOR_RED*/ "\nYOU CANNOT UNDO THIS ACTION\n" /*COLOR_RESET*/);
                    scanf(" %c", &confirm);  // Note the space before %c to consume the newline character
                    fflush(stdin);

                    if (confirm == 'y') 
                    {
                        //printf("There are %d elements in the file\n", CountElements(fptr));
                        int numFilms = CountElements(fptr);
                        Film newFilms[numFilms-1];
                        //write all the films except the one to be deleted to a new array
                        rewind(fptr);
                        int i = 0;
                        while(fread(&film, sizeof(Film), 1, fptr) == 1)
                        {
                            if(film.index != chosenFilm.index)
                            {
                                newFilms[i] = film;
                                i++;
                            }
                        }
                        //write the new array to the file
                        fclose(fptr);
                        fptr = fopen("watchlist.txt", "wb");
                        for(int i = 0; i < numFilms-1; i++)
                        {
                            newFilms[i].index = i+1;
                            fwrite(&newFilms[i], sizeof(Film), 1, fptr);
                        }
                        printf(/*COLOR_GREEN*/ "Film deleted!\n" /*COLOR_RESET*/);
                        return; //exit the function
                    } else {
                        printf(/*COLOR_BLUE*/ "Film not deleted!\n" /*COLOR_RESET*/);
                    }
                    break;

                default:
                    printf(/*COLOR_MAGENTA*/ "Invalid option!\n" /*COLOR_RESET*/);
                    break;
            }
        }
    }
    else printf(/*COLOR_RED*/ "Film not found!\n" /*COLOR_RESET*/);
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

void ModifyGameList(FILE *fptr)
{
    int searchBy, chooseToModify, index, choice;
    char name[MAX];
    char c; 
    bool found = false;  
    Game game, chosenGame; 
    
    do
    {
        printf("1. Search and modify sepcific game\n2. Modify all games\n0. Go back\n");
        scanf(" %d", &chooseToModify);
        switch (chooseToModify)
        {
        case 1: 
            break;
        case 2:
                printf("Scale all game scores to be between 0 and 100\n");
                printf("Are you sure you want to modify all games? (y/n):\nYOU CANNOT UNDO THIS ACTION\n");
                scanf(" %c", &c);  // Note the space before %c to consume the newline character
                if(c=='y') ScaleAllGames(fptr); 
                else printf("Games not modified!\n");
                return; //exit the function
            break; 
        default:
            printf("Please Insert a Valid Option\n\n");
            break;
        }
    }while(chooseToModify != 1); 

    do
    {
        printf("1. Search game by index\n2. Search game by name\n0. Go back\n"); 
        scanf(" %d", &searchBy); 
        switch (searchBy)
        {
            case 1:
                printf("Enter the index of the game you want to modify: ");
                scanf("%d", &index);
                fflush(stdin);
                // find the film with that index
                while (fread(&game, sizeof(Game), 1, fptr) == 1)
                {
                    if (game.index == index)
                    {
                        chosenGame = game;
                        found = true;
                        break; // Exit the loop once the film is found
                    }
                }
                break;

            case 2:
                printf("Enter the name of the game you want to modify: ");
                fflush(stdin);
                fgets(name, MAX-1, stdin);
                for(int i = 0; i < MAX; i++)
                {
                    if(name[i] == '\n')
                    {
                        name[i] = '\0';
                        break;
                    }
                }
                // find the film with that name
                rewind(fptr); // Reset the file pointer to the beginning
                while (fread(&game, sizeof(Game), 1, fptr) == 1)
                {
                    if (strcmp(game.name, name) == 0)
                    {
                        chosenGame = game;
                        found = true;
                        break; // Exit the loop once the film is found
                    }
                }
                break;

            case 0:
                Wait();
                return;

            default:
                printf("Please Insert a Valid Option\n\n");
                break;
        }
    }while(searchBy < 0 || searchBy > 2);

    if(found == true)
    {
        printf("Film found!\n");
        printf("Name: %s\nScore: %.1f\n", chosenGame.name, chosenGame.points);
        printf("Do you want to modify this film? (y/n): ");
        char confirm;
        scanf("%c", &confirm);
        fflush(stdin);
        if(confirm == 'y')
        {
            printf("1. Modify name\n2. Modify score\n3. Delete game\n");
            scanf("%d", &choice);
            fflush(stdin);
            switch(choice)
            {
                case 1:
                    //print chosen film name
                    printf("Chosen game name: %s\n", chosenGame.name); 
                    printf("Enter the new name: ");
                    fgets(chosenGame.name, MAX-1, stdin);
                    for(int i = 0; i < MAX; i++)
                    {
                        if(chosenGame.name[i] == '\n')
                        {
                            chosenGame.name[i] = '\0';
                            break;
                        }
                    }
                    fseek(fptr, -sizeof(Game), SEEK_CUR); // Move the file pointer back to the correct position
                    fwrite(&chosenGame, sizeof(Game), 1, fptr);
                    printf("Game name modified!\n");
                    return; //exit the function
                    break; 
                case 2:
                    printf("Enter the new score: ");
                    scanf("%f", &chosenGame.points);
                    fseek(fptr, -sizeof(Game), SEEK_CUR); // Move the file pointer back to the correct position
                    fwrite(&chosenGame, sizeof(Game), 1, fptr);
                    printf("Game score modified!\n");
                    return; //exit the function
                    break;
                case 3:
                    printf("Are you sure you want to delete this film? (y/n):\nYOU CANNOT UNDO THIS ACTION\n");
                    scanf(" %c", &confirm);  // Note the space before %c to consume the newline character
                    fflush(stdin);

                    if (confirm == 'y') 
                    {
                        //printf("There are %d elements in the file\n", CountElements(fptr));
                        int numGames = CountGames(fptr);
                        Game newGames[numGames-1];
                        //write all the films except the one to be deleted to a new array
                        rewind(fptr);
                        int i = 0;
                        while(fread(&game, sizeof(Game), 1, fptr) == 1)
                        {
                            if(game.index != chosenGame.index)
                            {
                                newGames[i] = game;
                                i++;
                            }
                        }
                        //write the new array to the file
                        fclose(fptr);
                        fptr = fopen("gamelist.txt", "wb");
                        for(int i = 0; i < numGames-1; i++)
                        {
                            newGames[i].index = i+1;
                            fwrite(&newGames[i], sizeof(Game), 1, fptr);
                        }
                        printf("Game deleted!\n");
                        return; //exit the function
                    } else {
                        printf("Game not deleted!\n");
                    }
                    break;

                default:
                    printf("Invalid option!\n");
                    break;
            }
        }
    }
    else printf("Game not found!\n");
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
    system("cls"); //clear the terminal before starting the program

    FILE *filmFile, *gameFile;
    int choice; 
    char confirm;

    CreateFileIfNotExisting("watchlist.txt");
    CreateFileIfNotExisting("gamelist.txt");

    do
    {
        printf("What would you like to do?\n\n1. Check Film List\n2. Add Film to watchlist\n3. Modify Film\n\n4. Check Game List\n5. Add Game to game list\n6. Modify Game\n\n0. Quit\n");
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
                system("cls"); //clear the terminal before starting the program
                filmFile = fopen("watchlist.txt", "r+b");
                OrderByIndex(filmFile);
                fclose(filmFile);
                filmFile = fopen("watchlist.txt", "rb");
                CheckFilmList(filmFile); 
                fclose(filmFile);
                Wait(); 
            break; 
            case 2: 
                system("cls"); //clear the terminal before starting the program
                filmFile = fopen("watchlist.txt", "ab"); 
                AddFilmToList(filmFile);
                fclose(filmFile);
                Wait();  
            break; 
            case 3: 
                system("cls"); //clear the terminal before starting the program
                filmFile = fopen("watchlist.txt", "r+b");
                ModifyFilmList(filmFile);
                fclose(filmFile);
                Wait(); 
            break;  
            case 4: 
                system("cls"); //clear the terminal before starting the program
                gameFile = fopen("gamelist.txt", "r+b"); 
                OrderByIndex(gameFile);
                fclose(gameFile);
                gameFile = fopen("gamelist.txt", "rb"); 
                CheckGameList(gameFile); 
                fclose(gameFile); 
                Wait(); 
            break; 
            case 5: 
                system("cls"); //clear the terminal before starting the program
                gameFile = fopen("gamelist.txt", "ab"); 
                AddGameToList(gameFile);
                fclose(gameFile); 
                Wait();  
            break; 
            case 6: 
                system("cls"); //clear the terminal before starting the program
                gameFile = fopen("gamelist.txt", "r+b");
                ModifyGameList(gameFile); 
                fclose(gameFile);
                Wait(); 
            break; 
            default:
                system("cls"); //clear the terminal before starting the program
                printf("===================================================\n!ERROR!\nPlease Enter a valid option\n===================================================\n");
                Wait();
            break; 
        }
    }while(choice!=0);

    return 0; 
}