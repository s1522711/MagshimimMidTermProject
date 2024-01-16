#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define DEBUG false // enables debug stuff for debugging purposes

#define MAX_MENU_INPUT 4 // the maximum input for the menu

// please for the love of god don't change these values, used in a lot of places for calculations (just the minimums)
#define MIN_THOUSAND_DIGIT 1000 // the minimum value for the thousand digit of the secret code
#define MIN_HUNDREDS_DIGIT 100 // the minimum value for the hundreds digit of the secret code
#define MIN_TENS_DIGIT 10 // the minimum value for the tens digit of the secret code
#define MIN_ONES_DIGIT 1 // the minimum value for the ones digit of the secret code

#define MAX_THOUSAND_DIGIT 6000 // the maximum value for the thousand digit of the secret code
#define MAX_HUNDREDS_DIGIT 600 // the maximum value for the hundreds digit of the secret code
#define MAX_TENS_DIGIT 60 // the maximum value for the tens digit of the secret code
#define MAX_ONES_DIGIT 6 // the maximum value for the ones digit of the secret code
#define EASY_GUESSES 20 // the number of guesses for the easy difficulty
#define MODERATE_GUESSES 15 // the number of guesses for the moderate difficulty
#define HARD_GUESSES 10 // the number of guesses for the hard difficulty
#define CRAZY_GUESSES_MIN 5 // the minimum number of guesses for the crazy difficulty
#define CRAZY_GUESSES_MAX 25 // the maximum number of guesses for the crazy difficulty

enum DifficultyEnum {EASY = 1, MODERATE, HARD, CRAZY}; // enum for the difficulty levels
enum DifficultyEnum difficulty = EASY; // global difficulty var, default difficulty is easy, will be changed by the user
enum GameModeEnum {MENU, GAME, WIN, FAIL, EXIT}; // enum for the game modes
enum GameModeEnum gameMode = MENU; // global game mode var, default game mode is menu, will be changed by the program
int secretCode = 0; // the secret code
int guessesLeft = 0; // the number of guesses left
int initialGuessAmount = 0; // the initial number of guesses the user has (depends on the difficulty)


int randr(int min, int max);
void menu();
int createCode();
void playerSetup();
void playerTurn();
void winScreen();
void failScreen();
bool menuInputCheck(int input);
bool codeInputCheck(char digit1, char digit2, char digit3, char digit4);
void replayQuestion();


int main() {
    while (gameMode != EXIT) // while the game mode is not exit
    {
        switch (gameMode) // check the game mode
        {
            case MENU: // if the game mode is menu
                menu(); // call the menu function
                break;
            case GAME: // if the game mode is game
                playerSetup(); // call the player setup function
                break;
            case WIN: // if the game mode is win
                winScreen(); // call the win screen function
                replayQuestion();
                break;
            case FAIL: // if the game mode is fail
                failScreen(); // call the fail screen function
                replayQuestion();
                break;
            default: // if something breaks assume the game mode is menu
                menu();
                break;
        }
    }
    return 0;
}

/*
 * this function returns a random int in the range you provide it
 * input: the minimum and the maximum numbers for the generation
 * output: randomly generated int in the range given
 */
int randr(int min, int max) {
    srand(time(NULL));
    return rand() % (max - min + 1) + min;
}

/*
 * this fuction prints the menu and gets the user's input
 * input: none
 * output: none
 */
void menu()
{
    gameMode = MENU; // set the game mode to menu
    printf("Welcome to \"MAGSHIMIM CODE-BREAKER\"!!!");
    printf("A secret password was chosen to protect the credit card of Pancratius,\nthe descendant of Antiochus.\nYour mission is to stop Pancratius by revealing his secret password.\n\n");
    printf("The rules are as follows:\n");
    printf("1. In each round you try to guess the secret password (4 distinct digits)\n");
    printf("2. After every guess you'll receive two hints about the password\n   HITS:   The number of digits in your guess which were exactly right.\n   MISSES: The number of digits in your guess which belongs to the password but were miss-placed.\n");
    printf("3. If you'll fail to guess the password after a certain number of rounds Pancratius will buy all the gifts for Hanukkah!!!\n");

    printf("Please choose the game level:\n1 - Easy (20 rounds)\n2 - Moderate (15 rounds)\n3 - Hard (10 rounds)\n4 - Crazy (random number of rounds 5-25)\n");
    printf("make a choice: ");
    do
    {
        scanf("%d", &difficulty);
    } while (!menuInputCheck(difficulty));
    gameMode = GAME; // set the game mode to game
}

/*
 * this function checks if the user's input is valid during the menu difficulty selection
 * input: the user's input
 * output: true if the input is valid, false otherwise
 */
bool menuInputCheck(int input)
{
    if (input < 1 || input > MAX_MENU_INPUT ) // if the input is not in the range of 1-4
    {
        printf("Invalid input, please try again: ");
        return false;
    }
    return true; // if the input is valid
}

/*
 * this function generates the secret code
 * input: none
 * output: the secret code, a 4 digit integer with no repeating digits, only digits 1-6
 */
int createCode()
{
    // generate 4 random numbers, only their last digit will be used
    int thousandDigit = randr(MIN_THOUSAND_DIGIT, MAX_THOUSAND_DIGIT);
    int hundredsDigit = randr(MIN_HUNDREDS_DIGIT, MAX_HUNDREDS_DIGIT);
    int tensDigit = randr(MIN_TENS_DIGIT,MAX_TENS_DIGIT);
    int onesDigit = randr(MIN_ONES_DIGIT,MAX_ONES_DIGIT);
    int loadingCounter = 0; // a counter for the loading animation
    printf("Generating secret code");
    // make sure there are no repeating digits
    while (thousandDigit/MIN_THOUSAND_DIGIT == hundredsDigit/MIN_HUNDREDS_DIGIT || thousandDigit/MIN_THOUSAND_DIGIT == tensDigit/MIN_TENS_DIGIT || thousandDigit/MIN_THOUSAND_DIGIT == onesDigit || hundredsDigit/MIN_HUNDREDS_DIGIT == tensDigit/MIN_TENS_DIGIT || hundredsDigit/MIN_HUNDREDS_DIGIT == onesDigit || tensDigit/MIN_TENS_DIGIT == onesDigit)
    {
        if (loadingCounter % 10000000 == 0) // print the loading animation every 60 iterations
        {
            printf(".");
        }
        loadingCounter++;
        thousandDigit = randr(MIN_THOUSAND_DIGIT, MAX_THOUSAND_DIGIT);
        hundredsDigit = randr(MIN_HUNDREDS_DIGIT, MAX_HUNDREDS_DIGIT);
        tensDigit = randr(MIN_TENS_DIGIT,MAX_TENS_DIGIT);
        onesDigit = randr(MIN_ONES_DIGIT,MAX_ONES_DIGIT);
    }
    return (thousandDigit/MIN_THOUSAND_DIGIT*MIN_THOUSAND_DIGIT)+(hundredsDigit/MIN_HUNDREDS_DIGIT*MIN_HUNDREDS_DIGIT)+(tensDigit/MIN_TENS_DIGIT*MIN_TENS_DIGIT)+onesDigit; // return the secret code
}

bool codeInputCheck(char digit1, char digit2, char digit3, char digit4)
{
    if (digit1 < '1' || digit1 > '6' || digit2 < '1' || digit2 > '6' || digit3 < '1' || digit3 > '6' || digit4 < '1' || digit4 > '6') // if the input is not in the range of 1-6
    {
        printf("Only 1-6 are allowed, try again!\nWrite your guess (only 1-6, no ENTER is needed)\n%d guesses left\n", guessesLeft);
        return false;
    }
    if (digit1 == digit2 || digit1 == digit3 || digit1 == digit4 || digit2 == digit3 || digit2 == digit4 || digit3 == digit4) // if the input has repeating digits
    {
        printf("No repeating digits are allowed, try again!\nWrite your guess (only 1-6, no ENTER is needed)\n%d guesses left\n", guessesLeft);
        return false;
    }
    return true; // if the input is valid
}

void playerSetup()
{
    gameMode = GAME; // set the game mode to game
    secretCode = createCode(); // generate the secret code
    if (DEBUG) // if debug is enabled print the secret code
    {
        printf("secret code: %d\n", secretCode);
    }

    switch (difficulty) // set the number of guesses according to the difficulty
    {
        case EASY:
            guessesLeft = EASY_GUESSES;
            initialGuessAmount = guessesLeft;
            break;
        case MODERATE:
            guessesLeft = MODERATE_GUESSES;
            initialGuessAmount = guessesLeft;
            break;
        case HARD:
            guessesLeft = HARD_GUESSES;
            initialGuessAmount = guessesLeft;
            break;
        case CRAZY:
            guessesLeft = randr(CRAZY_GUESSES_MIN, CRAZY_GUESSES_MAX);
            initialGuessAmount = guessesLeft;
            break;
        default: // if something breaks assume easy difficulty was selected
            guessesLeft = EASY_GUESSES;
            initialGuessAmount = guessesLeft;
            difficulty = EASY;
            break;
    }
    playerTurn(); // start the game
}

void playerTurn()
{
    while (gameMode == GAME) // while the game mode is game (the user didn't win or lose yet)
    {
        char digit1 = 0, digit2 = 0, digit3 = 0, digit4 = 0; // the digits of the user's guess
        int guess = 0; // the user's guess
        int hits = 0, misses = 0; // the number of hits and misses in the user's guess

        printf("\nWrite your guess (only 1-6, no ENTER is needed)\n");

        if (difficulty != CRAZY || DEBUG) // if the difficulty is not crazy/the debug mode isn't enabled print the number of guesses left
        {
            printf("%d guesses left\n", guessesLeft);
        } else // if the difficulty is crazy and the debug mode is enabled print the secret code
        {
            printf("CRAZY MODE!!!\n");
        }

        do
        {
            digit1 = getch(); // get the user's input
            printf("%d", digit1 - '0'); // print the user's input
            digit2 = getch();
            printf("%d", digit2 - '0');
            digit3 = getch();
            printf("%d", digit3 - '0');
            digit4 = getch();
            printf("%d\n", digit4 - '0');
        } while (!codeInputCheck(digit1, digit2, digit3, digit4)); // check if the input is valid
        guess = (digit1 - '0') * MIN_THOUSAND_DIGIT + (digit2 - '0') * MIN_HUNDREDS_DIGIT + (digit3 - '0') * MIN_TENS_DIGIT + (digit4 - '0'); // convert the user's input to an int

        if (guess == secretCode) // if the user guessed the secret code
        {
            gameMode = WIN; // set the game mode to win
            return;
        }
        else // if the user didn't guess the secret code
        {
            guessesLeft--; // decrease the number of guesses left
            if (guessesLeft == 0) // if the user ran out of guesses
            {
                gameMode = FAIL; // set the game mode to fail
                return;
            }
            else // if the user still has guesses left
            {
                // check how many hits and misses the user has
                if (guess / MIN_THOUSAND_DIGIT == secretCode / MIN_THOUSAND_DIGIT)
                {
                    hits++;
                }
                else if ((guess / MIN_THOUSAND_DIGIT) % MIN_TENS_DIGIT == (secretCode / MIN_THOUSAND_DIGIT) % MIN_TENS_DIGIT || (guess / MIN_THOUSAND_DIGIT) % MIN_TENS_DIGIT == (secretCode / MIN_HUNDREDS_DIGIT) % MIN_TENS_DIGIT || (guess / MIN_THOUSAND_DIGIT) % MIN_TENS_DIGIT == (secretCode / MIN_TENS_DIGIT) % MIN_TENS_DIGIT || (guess / MIN_THOUSAND_DIGIT) % MIN_TENS_DIGIT == secretCode % MIN_TENS_DIGIT)
                {
                    misses++;
                }

                if ((guess / MIN_HUNDREDS_DIGIT) % MIN_TENS_DIGIT == (secretCode / MIN_HUNDREDS_DIGIT) % MIN_TENS_DIGIT)
                {
                    hits++;
                }
                else if ((guess / MIN_HUNDREDS_DIGIT) % MIN_TENS_DIGIT == (secretCode / MIN_THOUSAND_DIGIT) % MIN_TENS_DIGIT || (guess / MIN_HUNDREDS_DIGIT) % MIN_TENS_DIGIT == (secretCode / MIN_HUNDREDS_DIGIT) % MIN_TENS_DIGIT || (guess / MIN_HUNDREDS_DIGIT) % MIN_TENS_DIGIT == (secretCode / MIN_TENS_DIGIT) % MIN_TENS_DIGIT || (guess / MIN_HUNDREDS_DIGIT) % MIN_TENS_DIGIT == secretCode % MIN_TENS_DIGIT)
                {
                    misses++;
                }

                if ((guess / MIN_TENS_DIGIT) % MIN_TENS_DIGIT == (secretCode / MIN_TENS_DIGIT) % MIN_TENS_DIGIT)
                {
                    hits++;
                }
                else if ((guess / MIN_TENS_DIGIT) % MIN_TENS_DIGIT == (secretCode / MIN_THOUSAND_DIGIT) % MIN_TENS_DIGIT || (guess / MIN_TENS_DIGIT) % MIN_TENS_DIGIT == (secretCode / MIN_HUNDREDS_DIGIT) % MIN_TENS_DIGIT || (guess / MIN_TENS_DIGIT) % MIN_TENS_DIGIT == (secretCode / MIN_TENS_DIGIT) % MIN_TENS_DIGIT || (guess / MIN_TENS_DIGIT) % MIN_TENS_DIGIT == secretCode % MIN_TENS_DIGIT)
                {
                    misses++;
                }

                if (guess % MIN_TENS_DIGIT == secretCode % MIN_TENS_DIGIT)
                {
                    hits++;
                }
                else if (guess % MIN_TENS_DIGIT == (secretCode / MIN_THOUSAND_DIGIT) % MIN_TENS_DIGIT || guess % MIN_TENS_DIGIT == (secretCode / MIN_HUNDREDS_DIGIT) % MIN_TENS_DIGIT || guess % MIN_TENS_DIGIT == (secretCode / MIN_TENS_DIGIT) % MIN_TENS_DIGIT)
                {
                    misses++;
                }

                printf("You got    %d HITS    %d MISSES.", hits, misses); // print the number of hits and misses
                playerTurn(); // call the function again
            }
        }
    }
}

void winScreen()
{
    gameMode = WIN; // set the game mode to win
    // ascii art of an award
    printf("                     _ /\\.'|_\n                 _.-| |\\ | / |_\n                / \\ _>-\"\"\"-._.'|_\n               >`-.'         `./ \\\n              /`./             \\-<\n              `-|               |_/\n              /_|               |_\\\n              ) |               | |\n              -<|               |\\/\n              `'_\\             /`<\n               |_/`.         .'\\_/\n                \\_/ >-.._..-'\\_|\n                  `-`_| \\_\\|_/\n                   |   `' |  |\n                   |      |  |\n                   |      |  |\n                   |      |  |\n                   |      |  |\n                   |  /\\  |  |\n                   | /| \\ |\\ |\n                   |/ |/ \\| \\|\n");
    printf("   4 HITS YOU WON!!!\n\n");
    printf("It took you only %d guesses, you are a professional code breaker!\n\n", initialGuessAmount - guessesLeft + 1);

}

void failScreen()
{
    gameMode = FAIL; // set the game mode to fail
    // ascii art of a lion
    printf("                 ___\n       ____  / /     /\n     /  .  __          \\\n   / .    |  /  ____  \\\n /     .  \\    /  __\\\n|  .         /   /__|---.\n|.        \\            \\|\n| .        |            |\n|          \\         __/\n|.           \\_______  |\n|    \\    \\ \\     |  ``'\n|\\         \\  \\    \\\n");
    printf("OOOOHHHH!!! Pancratius won and bought all of Hanukkah's gifts.\n");
    printf("Nothing left for you...\n");
    printf("The secret password was %d\n\n", secretCode);
}

void replayQuestion()
{
    char input = ' ';
    do
    {
        printf("Would you like to play again? (y/n): ");
        input = (char)getch();
        printf("%c\n", input);
    } while (input != 'y' && input != 'n' & input != 'Y' && input != 'N');
    if (input == 'y' || input == 'Y')
    {
        gameMode = MENU;
    }
    else
    {
        gameMode = EXIT;
        printf("Bye Bye!\n");
        printf("Press any key to continue . . . ");
        getch();
    }
}
