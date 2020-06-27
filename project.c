#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

struct question
{
    int questionNo;
    char question[500];
    char options[4][40];
    char correctAnswer[40];
} q, flip;

void shuffle(int *array, int n);
void printQuestion(FILE *infile, int *array, int i);
void checkAnswer(struct question ques);
void fiftyfifty(struct question ques);
void flipQuestion();
int calculateGameOver(int roundNumber);
int calculateGameQuit(int roundNumber);

/* Global variables to determine whether a lifeline is used / whether Game is over due to a wrong answer */
int isGameOver = 0, isGameQuit, isFiftyFiftyUsed = 0, isFlipUsed = 0;

int main()

{
    /* Helps in formatting numbers with commas in Indian currency style. */
    setlocale(LC_NUMERIC, "en_IN");

    /* FILE OPENING */
    FILE *infile;
    infile = fopen("questions.txt", "r");
    if (infile == NULL)
    {
        fprintf(stderr, "Error opening file");
        exit(1);
    }

    /* Generating a random array of numbers */
    int num[30];
    for (int i = 0; i < 30; i++)
        num[i] = i;

    shuffle(num, 30);

    /* Introduction Screen */
    char name[50];
    printf("What is your name?\n");
    scanf("%s", name);
    system("clear");
    printf("Welcome to Kaun Banega Crorepati %s.\n", name);
    getchar();

    /* RULES */
    printf("There are total 15 stages (questions) in this game where question no. 5 and 10 are milestone questions.\n");
    printf(" 15) 1,00,00,000\n 14)   50,00,000\n 13)   25,00,000\n 12)   12,50,000\n 11)    6,40,000\n 10)    3,20,000\n 09)    1,60,000\n 08)      80,000\n 07)      40,000\n 06)      20,000\n 05)      10,000\n 04)       5,000\n 03)       3,000\n 02)       2,000\n 01)       1,000\n");
    printf("You have to answer the given question correctly to see and play the next question.\n");
    printf("As long as you have only seen the question and its four possible answers, you can always quit the game by using ' Q ' key,\nat any stage and keep the credits you have already won.\n");
    printf("If you choose to answer a question and answer it incorrectly, you will lose all the credits and will\nonly take away credits you have earned until last milestone question. That is:\n");
    printf(" If incorrect answer was for question from 1 to 5, you will get 0 credits.\n If incorrect answer was for question from 6 to 10, you will get 10,000 credits.\n If incorrect answer was for question from 11 to 15, you will get 3,20,000 credits. \n");
    printf("You have two lifelines:\n First is, 50-50: Two wrong options will be removed from the screen. It can be used by pressing ' F ' key.\n And second is, Flip the question: The question will be replaced by an entire new question. It can be used by pressing ' L ' key.\n");

    printf(" Press Enter to start the quiz!");
    while (getchar() != 10)
    {
        ;
    }
    system("clear");
    /* MAIN PART */
    int i;
    for (i = 0; i < 15; ++i)
    {
        printQuestion(infile, num, i);

        if (isGameOver == 1)
        {
            printf("Your game is over and you have won %'d credits.\n", calculateGameOver(i + 1));
            break;
        }

        if (isGameQuit == 1)
        {
            printf("You have quit the game and won %'d credits. Congrats!\n", calculateGameQuit(i + 1));
            break;
        }
    }

    if (i == 15)
    {
        printf("Congrats, you have won Kaun Banega Crorepati and 1 Crore credits!");
    }

    fclose(infile);
    return 0;
}

void shuffle(int *array, int n)
{
    srand(time(0));
    int i;
    for (i = 0; i < n - 1; i++)
    {
        int j = rand() % n;
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

void printQuestion(FILE *infile, int *array, int i)
{
    int rand = array[i];

    fseek(infile, array[16] * sizeof(struct question), SEEK_SET);
    fread(&flip, sizeof(struct question), 1, infile);

    fseek(infile, rand * sizeof(struct question), SEEK_SET);
    fread(&q, sizeof(struct question), 1, infile);

    /* DISPLAYING A QUESTION */
display:
    printf("Question Number %d\n", i + 1);
    printf("You have %'d credits and you are playing for %'d credits.\n\n", calculateGameQuit(i + 1), calculateGameQuit(i + 2));
    printf("%s\n", q.question);
    printf("A. %s \t\t B. %s\n", q.options[0], q.options[1]);
    printf("C. %s \t\t D. %s\n\n", q.options[2], q.options[3]);

    checkAnswer(q);
}

void checkAnswer(struct question ques)
{
    char opt;
    int j;

    printf("Enter the option you wish to choose: \nA, B, C and D for the respective options. Q to quit the game.\n");

    if (isFiftyFiftyUsed == 0)
    {
        printf("You have the Fifty Fifty lifeline left. Enter F to use the lifeline.\n\n");
    }

    if (isFlipUsed == 0)
    {
        printf("You have Flip the Question lifeline left. Enter L to use the lifeline.\n\n");
    }

    if ((isFlipUsed == 1) && (isFiftyFiftyUsed == 1))
    {
        printf("You have no lifelines left.");
    }

    scanf("%c", &opt);
    while (getchar() != 10)
    {
    }
    switch (tolower(opt))
    {
    case 'a':
        j = 0;
        break;
    case 'b':
        j = 1;
        break;
    case 'c':
        j = 2;
        break;
    case 'd':
        j = 3;
        break;
    case 'f':
        /* Fifty Fifty lifeline. */
        if (isFiftyFiftyUsed == 1)
        {
            printf("Sorry you have already used this lifeline. Please re enter with another option.");
            checkAnswer(ques);
            goto skip;
        }
        else
        {
            fiftyfifty(ques);
            goto skip;
        }

    case 'l':
        /* Flip the Question lifeline. */
        if (isFlipUsed == 1)
        {
            printf("Sorry you have already used this lifeline. Please re enter with another option.");
            checkAnswer(ques);
            goto skip;
        }
        else
        {
            flipQuestion();
            goto skip;
        }

    case 'q':
        isGameQuit = 1;
        goto skip;

    default:
    {
        printf("Wrong option entered. Re enter.\n");
        checkAnswer(ques);
    };
    }

    if (strcmp(ques.correctAnswer, ques.options[j]) == 0)
    {
        system("clear");
        printf("Correct answer. Moving on to the next round.\n\n");
    }
    else
    {
        printf("Wrong answer.\n");
        isGameOver = 1;
    }
skip:;
}

void fiftyfifty(struct question ques)
{
    isFiftyFiftyUsed = 1;
    system("clear");

    char tempstr[4][40];
    int temparr[4], count = 0;

    for (int k = 0; k < 4; ++k)
    {
        strcpy(tempstr[k], ques.options[k]);
        temparr[k] = k;
    }
    shuffle(temparr, 4);

    for (int k = 0; k < 4; k++)
    {
        if (count == 2)
            break;

        if (strcmp(ques.correctAnswer, tempstr[temparr[k]]) == 0)
            ;
        else
        {
            count++;
            memset(tempstr[temparr[k]], '\0', 40);
        }
    }
    printf("%s\n", ques.question);
    printf("A. %s \t\t B. %s\n", tempstr[0], tempstr[1]);
    printf("C. %s \t\t D. %s\n\n", tempstr[2], tempstr[3]);
    checkAnswer(ques);
}

void flipQuestion()
{
    isFlipUsed = 1;
    system("clear");
    printf("%s\n", flip.question);
    printf("A. %s \t\t B. %s\n", flip.options[0], flip.options[1]);
    printf("C. %s \t\t D. %s\n\n", flip.options[2], flip.options[3]);
    checkAnswer(flip);
}

int calculateGameOver(int roundNumber)
{
    if (roundNumber <= 5)
        return 0;
    else if (roundNumber <= 10)
        return 10000;
    else
        return 320000;
}

int calculateGameQuit(int roundNumber)
{
    switch (roundNumber)
    {
    case 1:
        return 0;
        break;

    case 2:
        return 1000;
        break;

    case 3:
        return 2000;
        break;

    case 4:
        return 3000;
        break;

    case 5:
        return 5000;
        break;

    case 6:
        return 10000;
        break;

    case 7:
        return 20000;
        break;

    case 8:
        return 40000;
        break;

    case 9:
        return 80000;
        break;

    case 10:
        return 160000;
        break;

    case 11:
        return 320000;
        break;

    case 12:
        return 640000;
        break;

    case 13:
        return 1250000;
        break;

    case 14:
        return 2500000;
        break;

    case 15:
        return 5000000;
        break;

    case 16:
        return 10000000;
        break;
    default:
        break;
    }
}
