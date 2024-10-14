#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mySA.h"

#define MAX_WORD_LENGTH 50
#define SIS_ARRAY_SIZE 10
#define BUFFER_SIZE 2048

int vRead(int argc, char *argv[]); // Forward declaration of vRead
int sentiment(int argc, char *argv[]); // Forward declaration of sentiment

int main(int argc, char *argv[]) {
    // Check # of CLI args
    if (argc == 2) {
        // If 2, run vRead
        return vRead(argc, argv);
    } else if (argc == 3) {
        // If 3, run sentiment
        return sentiment(argc, argv);
    } else {
        // If the number of arguments is neither 2 nor 3, print usage message
        fprintf(stderr, "Usage: %s <path_to_vader_lexicon.txt> [<path_to_sentences.txt>]\n", argv[0]);
        return 1;
    }
}

/*
If argc = 2 then run Vread function (reads the vader_lexicon.txt file)
iF argc = 3 then run sentiment function (returns line by line sentiment analysis)

Reminder == usage = ./mySA <vader_lexicon.txt> [<validation.txt>]
*/

// function reads lexicon file and fills up words structure
words *rLex(const char *filename, int *size) {
    FILE *file = fopen(filename, "r"); // Openfile for read
    if (!file) {
        perror("Sorry, could not open the file"); // file open fails
        return NULL;
    }

    int bucket = 10; // Initial bucket for the array of words
    words *lexi = malloc(bucket * sizeof(words)); // Allocate initial memory
    *size = 0; // Initialize size of array to zero

    char line[512]; // temp location to hold each line of the file
    while (fgets(line, sizeof(line), file)) { // readlines until EOF
        if (*size >= bucket) {
            bucket *= 2; // increase capacity if needed
            lexi = realloc(lexi, bucket * sizeof(words)); // Reallocate memory 
            if (!lexi) {
                perror("Check memory allocation as there is a failure somewhere."); // print error Msg if reallocation fails
                fclose(file);
                return NULL;
            }
        }

        // tkenize line struct parameteres (word,polar,sd,score)
        char *token = strtok(line, "\t");
        lexi[*size].word = dup_str(token); // make copy of word
        token = strtok(NULL, "\t");
        lexi[*size].score = atof(token); // score to float
        token = strtok(NULL, "\t");
        lexi[*size].SD = atof(token); // SD to  float

        // Pars SIscores, assume theyre full
        for (int i = 0; i < SIS_ARRAY_SIZE; ++i) {
            token = strtok(NULL, ", \n"); // Tokenize every score
            if (token != NULL) {
                lexi[*size].SIS_array[i] = atoi(token); // Convert score to int
            }
        }

        (*size)++; // Increment the size of the lexi array
    }

    fclose(file); // Close vader_lexicon file
    return lexi; // Return the populated lexi
}
// We need to free dynamically allocated mem for lexicon
void freeLex(words *lexi, int size) {
    for (int counter = 0; counter < size; ++counter) {
        free(lexi[counter].word); // Free each word string
    }
    free(lexi); // Free lexi array itself
}

int vRead(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path_to_vader_lexicon.txt>\n", argv[0]); // Print if CLI argument is not correct
        return 1;
    }

    int lexSize = 0;
    words *lexi = rLex(argv[1], &lexSize); // Read lexicon from file 
    if (!lexi) {
        return 1; // If reading failed, exit program
    }
 /* 
    for (int loop = 0; loop < lexSize; ++loop) {
        printf("Word: %s\n", lexi[loop].word); // Print word
        printf("Score: %.2f\n", lexi[loop].score); // Print  polar_score
        printf("SD: %.2f\n", lexi[loop].SD); // Print std_dev
        printf("SIS: ");
        for (int j = 0; j < SIS_ARRAY_SIZE; ++j) {
            printf("%d ", lexi[loop].SIS_array[j]); // Print SIS_array
        }
        printf("\n\n");
    }
*/
    freeLex(lexi, lexSize); // Free allocated mem 
    return 0; 
}

// needto calculate the sentiment score of a sentence
float do_sent_score(const char *sentence, words *lexi, int size) {
    float overall = 0.0;       
    int wordC = 0;     
    char *sentCopy = dup_str(sentence);     
    char *token = strtok(sentCopy, " ,.-\n");    
    // Iterate through each token
    while (token != NULL) {
        // Convert token to lowercase
        for (int n = 0; token[n]; n++) {
            token[n] = tolower(token[n]);
        }

        // Compare token with words in lexicon
        int present = 0;
        for (int counter = 0; counter < size; counter++) {
            if (strcmp(lexi[counter].word, token) == 0) {
                overall += lexi[counter].score;       // Add sent score to sum
                present = 1;
                break;
            }
        }

        // If word not found asssign 0 score
        if (!present) {
            overall += 0;
        }

        wordC++;       // Increment word count
        token = strtok(NULL, " ,.-\n");     // next token
    }

    free(sentCopy);        
    return wordC > 0 ? overall / wordC : 0;      // Calculate then return average sentiment score
} 
// duplicating a string
char *dup_str(const char *s) {
    size_t size = strlen(s) + 1;       // Calculate size of string
    char *p = malloc(size);     // Allocate memory for duplicated string
    if (p != NULL) {
        memcpy(p, s, size);     // Copy string
    }
    return p;       // Return pointer to dup string
}



// function does SA  on validation.txt
int sentiment(int argc, char *argv[]) {
    // Check CLI args
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <path_to_vader_lexicon.txt> <path_to_validation.txt>\n", argv[0]);
        return 1; 
    }

    int lexi_size = 0; 
    // Read lexicon from file specified in the CLI arg
    words *lexi = rLex(argv[1], &lexi_size);
    if (!lexi) {
        return 1; //can't read lexi means error
    }

    //open validation.txt in cli arg
    FILE *sentence_file = fopen(argv[2], "r");
    if (!sentence_file) {
        perror("Sorry, can't open file");//file opening file
        freeLex(lexi, lexi_size); // Free alloc mem
        return 1; 
    }

    //format output
    int lineWidth = 80; // typical width is 80chars, just guess
    int scoreLength = 10; //how much spave we have for "score" and numbers
    int statements = lineWidth - scoreLength; // The spae that we'd use for the sentence part
    
    //headers
    printf("%-*s%*s\n", statements, "string sample", scoreLength, "score");
    
   //seperation line
    for (int i = 0; i < lineWidth; i++) {
        putchar('-'); // Print '-' for entire line
    }
    putchar('\n'); //next line

    char buffer[BUFFER_SIZE]; // stores line
    while (fgets(buffer, BUFFER_SIZE, sentence_file)) {
        buffer[strcspn(buffer, "\n")] = 0;

        // sent score for current sentence
        float score = do_sent_score(buffer, lexi, lexi_size);

        // right allign for score
        int sentLen = strlen(buffer);
        int spaces = statements - sentLen; // spacing depends on sent length

        // spacing never negative
        if (spaces < 0) spaces = 0;
        printf("%s%*s%.2f\n", buffer, spaces, "", score);
    }

    // closefile
    fclose(sentence_file);
    // Free allocated mem for lexi
    freeLex(lexi, lexi_size);

    return 0; 
}
