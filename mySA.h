//header file, declare functions and structs

#ifndef MYSA_H
#define MYSA_H

#define MAX_WORD_LENGTH 50
#define SIS_ARRAY_SIZE 10
#define BUFFER_SIZE 2048

typedef struct {
    char *word;
    float score;
    float SD;
    int SIS_array[SIS_ARRAY_SIZE];
} words;

words *rLex(const char *filename, int *size);
void freeLex(words *lexi, int size);
int vRead(int argc, char *argv[]);
float do_sent_score(const char *sentence, words *lexi, int size);
int sentiment(int argc, char *argv[]);
char *dup_str(const char *s);

#endif
