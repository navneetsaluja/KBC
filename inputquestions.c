#include<stdio.h>
#include<stdlib.h>

struct question 
{   
    int questionNo;
    char question[500];
    char options[4][40];
    char correctAnswer[40];
};

int main() 
{
    FILE *outfile;
    int qno;
    outfile = fopen("questions.txt", "a");
    if (outfile == NULL)
    {
		fprintf(stderr, "ERROR IN OPENING FILE");
		exit (1);
    }

    struct question input1 = 
    {
        26, 
        "Which of these is the name of a Zodiac sign as well as a National Award winning Hindi film actor?",
        "Mesh", "Dhanu", "Mithun", "Kumbha",
        "Mithun"
    };

    struct question input2 = 
    {
        27, 
        "Which global retailer bought 77% of Flipkart for over 16 billion US Dollar in 2018?",
        "Amazon", "Rakuten", "Walmart", "Tesco",
        "Walmart"
    };

    struct question input3 = 
    {
        28, 
        "In February 2017, ISRO created a world record by launching how many satellites through a single rocket?",
        "34", "96", "104", "154",
        "104"
    };

    struct question input4 = 
    {
        29, 
        "Which is the largest joint in the human body?",
        "Elbow", "Hip", "Knee", "Shoulder",
        "Knee"
    };

    struct question input5 = 
    {
        30, 
        "Which Indian state had India’s first Muslim woman chief minister?",
        "Kerala", "Jammu & Kashmir", "Goa", "Assam",
        "Assam"
    };


    fwrite(&input1, sizeof(struct question), 1, outfile);
    if(fwrite != 0)  
        printf("contents to file written successfully !\n"); 
    else 
        printf("error writing file !\n");

    fwrite(&input2, sizeof(struct question), 1, outfile);
    if(fwrite != 0)  
        printf("contents to file written successfully !\n"); 
    else 
        printf("error writing file !\n"); 

    fwrite(&input3, sizeof(struct question), 1, outfile);
    if(fwrite != 0)  
        printf("contents to file written successfully !\n"); 
    else 
        printf("error writing file !\n");

    fwrite(&input4, sizeof(struct question), 1, outfile);
    if(fwrite != 0)  
        printf("contents to file written successfully !\n"); 
    else 
        printf("error writing file !\n");
    
    fwrite(&input5, sizeof(struct question), 1, outfile);
    if(fwrite != 0)  
        printf("contents to file written successfully !\n"); 
    else 
        printf("error writing file !\n");
    
    
    
    fclose(outfile);

    return 0;
}