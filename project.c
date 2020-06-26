#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

struct question
{
    int questionNo;
    char question[500];
    char options[4][40];
    char correctAnswer[40];
} q, flip;

void shuffle(int *array, int n);
void printQuestion(FILE *infile, int *array, int i);
void checkAnswer();
void fiftyfifty();

int main()

{
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
    printf("As long as you have only seen the question and its four possible answers, you can always quit the game.\nat any stage and keep the credits you have already won.\n");
    printf("If you choose to answer a question and answer it incorrectly, you will lose all the credits and will\nonly take away credits you have earned until last milestone question. That is:\n");
    printf(" If incorrect answer was for question from 1 to 5, you will get 0 credits.\n If incorrect answer was for question from 6 to 10, you will get 10,000 credits.\n If incorrect answer was for question from 11 to 15, you will get 3,20,000 credits. \n");
    printf("You have two lifelines:\n First is, 50-50: Two wrong options will be removed from the screen.\n And second is, Flip the question: The question will be replaced by an entire new question .");

    printf("Press Enter to start the quiz!");
    getchar();
    system("clear");

    /* MAIN PART */
    for (int i = 0; i < 15; ++i)
    {
        printQuestion(infile, num, i);
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
    printf("%s\n", q.question);
    printf("A. %s \t\t B. %s\n", q.options[0], q.options[1]);
    printf("C. %s \t\t D. %s\n\n", q.options[2], q.options[3]);

    checkAnswer();
}

void checkAnswer()
{
    char opt;
    int j;

    printf("Enter the option you wish to choose: ");
    scanf("%c", &opt);
    getchar();
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
        /* FIFTY FIFTY LIFELINE */
        fiftyfifty(); goto skip;
    case 'l': break;

    default:
    {
        printf("WRONG OPTION ENTERED. RE ENTER.\n");
        checkAnswer();
    };
    }

    if (strcmp(q.correctAnswer, q.options[j]) == 0)
        printf("CORRECT ANSWER\n\n");
    else
        printf("WRONG\n\n");

    skip: ;
}

void fiftyfifty()
{
    char tempstr[4][40];
    int temparr[4], count = 0;


    for (int k = 0; k < 4; ++k)
    {
        strcpy(tempstr[k], q.options[k]);
        temparr[k] = k;
    }
    shuffle(temparr, 4);

    for (int k = 0; k < 4; k++)
        {
            if (count == 2)
                break;

            if (strcmp(q.correctAnswer, tempstr[temparr[k]]) == 0)
                ;
            else
            {
                count++;
                memset(tempstr[temparr[k]], '\0', 40);
            }
        }
        printf("%s\n", q.question);
        printf("A. %s \t\t B. %s\n", tempstr[0], tempstr[1]);
        printf("C. %s \t\t D. %s\n\n", tempstr[2], tempstr[3]);

        checkAnswer();
}