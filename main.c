/*
 *              Vasilis Georgoulas A.M:2118147
 * Authors :  &
 *              Ioannis Skoumpas A.M: 2118063
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 1000

/* structure holding word frequency information */
typedef struct word {
    char	s[SIZE];	/* the word */
    int	count;		/* number of times word occurs */
} word;

struct File_records {
    char fname[20];
    int choice;
};

//Declaration of the procedures
void creation(struct File_records *data);

void concatenate(struct File_records *data);

void deletion(struct File_records *data);

void search(struct File_records *data);

int is_alpha(char c);

void remove_non_alpha (char *s);

void make_lowercase (char *s);

void insert_word (word *words, int *n, char *s);

int wordcmp (word *a, word *b);

void export(struct File_records *data);

void export_file(struct File_records *data);

int main() {
    struct File_records *data;
    data->choice = -1;

    while (data->choice != 0) {

        printf("\n********************************************************************************************");
        printf("\n1. Give a name for a data"
               "\n2. Add a new record"
               "\n3. Delete a record"
               "\n4. Search a record"
               "\n5. Export the statistics in the screen"
               "\n6. Export the statistics in a txt data"
               "\n0. Exit the program");
        printf("\n********************************************************************************************");
        printf("\nSelect an option:\t");
        scanf("%d", &data->choice);
        fflush(stdin);

        if (data->choice == 1) {
            creation(data);
        } else if (data->choice == 2) {
            concatenate(data);
        } else if (data->choice == 3) {
            deletion(data);
        } else if (data->choice == 4) {
            search(data);
        } else if (data->choice == 5) {
            export(data);
        } else if (data->choice == 6) {
            export_file(data);
        } else if (data->choice == 0) {
            printf("\nTerminating program...");
            exit(0);
        }
    }
}

// Procedure for the creation of a txt file
void creation(struct File_records *data) {

    printf("Enter a name for the file :\t");
    gets(data->fname);

    FILE *file;
    file = fopen(data->fname, "a+");
    if (file == NULL) {
        printf("\n\tError opening file!\n");
        exit(-1);
    }

    char input[SIZE];
    printf("Start typing :\t");
    fgets(input, SIZE, stdin);
    fputs(input, file); //Place the input into the file

    fclose(file);
}

// Procedure for adding more data to the file
void concatenate(struct File_records *data) {

    FILE *file;
    file = fopen(data->fname, "a+");
    if (file == NULL) {
        printf("\nError opening data!\n");
        exit(-1);
    }

    char input[SIZE];
    printf("Start typing :\t");
    fgets(input, SIZE, stdin); // Get input
    fputs(input, file);

    fclose(file);
}

//Procedure that delete a specific line-entry from the file
void deletion(struct File_records *data) {

    FILE *original_file, *temp_file;
    char ch;
    char str[SIZE];
    int delete_line, line = 0;

    //open file in read mode
    original_file = fopen(data->fname, "r");
    if (!original_file) {
        printf("\nError opening data!\n");
        exit(-1);
    }

    //Prints the entries of the file
    printf("**************************************************\n");
    while (ch != EOF) {
        ch = getc(original_file);
        printf("%c", ch);
    }
    printf("**************************************************\n");

    rewind(original_file); //rewind the file pointer;
    printf("Enter the line number of the line to be deleted:\t");
    scanf("%d", &delete_line); // Get the line that going to be deleted

    //open new file in write mode
    temp_file = fopen("replica.c", "w");
    if (!temp_file) {
        printf("\nError opening data!\n");
        exit(-1);
    }

    // Remember to try to change this
    ch = 'A';  //resetting ch
    while (!feof(original_file)) {
        strcpy(str, original_file);
        fgets(str, SIZE, original_file);
        //except the line to be deleted
        if (!feof(original_file)) {
            line++;
            /* skip the line at given line number */
            if (line != delete_line) {
                fprintf(temp_file, "%s", str);
            }
        }
    }
    fclose(original_file);
    fclose(temp_file);
    remove(data->fname); //delete the original file
    rename("replica.c", data->fname); //rename the file replica.c to original file name

    printf("\nThe contents of file after being modified are as follows:\n\t");
    original_file = fopen(data->fname, "r");
    ch = getc(original_file);
    while (ch != EOF) {
        printf("%c", ch);
        ch = getc(original_file);
    }
    fclose(original_file);
}

void search(struct File_records *data) {
    char word[SIZE];

    // Input word to search in file
    printf("\nEnter a word to search in file :\t");
    scanf("%s", word);

    FILE *file;
    file = fopen(data->fname, "r"); //data->fname : "text.txt"
    if (file == NULL) {
        printf("\nUnable to open file.\n");
        exit(-1);
    }

    //Needs fixing
    char str[SIZE];
    char *pos;
    int line = 0;
    int col = 0;
    char ch;

    while (!feof(file)) {
        fscanf(file,"%s",str);
        printf("%s",str);
        int char_index = 0;
        int field_positions[SIZE];
        int field_counter = 0;
        line++;
        pos = strstr(str, word);

        if (pos != NULL) {
            /* First index of word in str is
            * Memory address of pos - memory
             address of str. */
            col = (pos - str);

            //
            for (ch = getc(file); ch != '\n'; ch = getc(file)) {
                if (ch == ',') {
                    field_counter++;
                    field_positions[field_counter] = char_index;
                }
                char_index++;
            }

            for (int i = 0; i < field_counter; ++i) {
                if (field_positions[i] < col && field_positions[i + 1] > col) {
                    printf("\nWe found \"%s\" in line %d and in the %d field", word, line, i + 1);
                    break;
                }
            }
        }
    }
    fclose(file);
}

/* return 1 if c is alphabetic (a..z or A..Z), 0 otherwise */
int is_alpha (char c) {
    if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') return 1;
    return 0;
}

/* remove the i'th character from the string s */
void remove_char (char *s, int i) {
    while (s[i]) {
        i++;
        s[i-1] = s[i];
    }
    s[i] = 0;
}

/* remove non-alphabetic characters from the string s */
void remove_non_alpha (char *s) {
    int	i;

    for (i=0; s[i]; i++) if (!is_alpha (s[i])) remove_char(s, i);
}
void make_lowercase (char *s) {
    int	i;

    for (i=0; s[i]; i++) s[i] = tolower(s[i]);
}

void insert_word (word *words, int *n, char *s) {
    int	i;

    /* linear search for the word */
    for (i=0; i<*n; i++) {
        if (strcmp(s, words[i].s) == 0) {

            /* found it?  increment and return. */

            words[i].count++;
            return;
        }
    }

    /* copy the word into the structure at the first available slot,
     * i.e., *n
     */

    strcpy (words[*n].s, s);

    /* this word has occured once up to now, so count = 1 */

    words[*n].count = 1;

    /* one more word */

    (*n)++;
}

int wordcmp (word *a, word *b) {
    if (a->count < b->count) return +1;
    if (a->count > b->count) return -1;
    return 0;
}

void export(struct File_records *data) {

    char ch;
    float fields_per_line[SIZE];
    int line_counter = 0;
    int field_counter = 0;
    float sum = 0;


    FILE *file;
    file = fopen(data->fname, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(-1);
    }

    for (ch = getc(file); ch != EOF; ch = getc(file)) {
        if (ch == '\n') {
            line_counter++; // Increment count if this character is newline
        }
        if (ch == ',') {
            field_counter++;           // Find the number of field per line
            fields_per_line[line_counter] += 1; // and store them
        }
    }
    float average;
    for (int i = 0; i < line_counter; ++i) {
        sum += fields_per_line[i];  // find the average field per line;
        average = (float) sum / line_counter;
    }

    fclose(file);

    printf("\nThe total number of lines was : %d", line_counter);
    printf("\nThe total number of field was : %d", field_counter);
    printf("\nThe average number of field per line was : %.2f\n", average);

    // Five more frequent words
    word words[SIZE];
    char s[1000];
    int i, n, m;

    file = fopen(data->fname, "r");

    /* read all the words from the file... */
    while (!feof(file)) {
        fscanf(file, "%s", s);

        /* only insert the word if it's not punctuation */

        if (is_alpha(s[0])) {

            /* get rid of non-letters */

            remove_non_alpha(s);

            /* make all letters lowercase */

            make_lowercase(s);

            /* put this word in the list */

            insert_word(words, &n, s);

        }
    }
    /* sort the list of words by descending frequency */

    qsort((void *) words, n, sizeof(word),
          (int (*)(const void *, const void *)) wordcmp);

    /* print the words with their frequencies */
    printf("\nThe 5 most common words in the file are:\n");
    for (i = 0; i < 5; i++){
        if (words[i].count != 0) {
            printf("\n%s: %d\n", words[i].s, words[i].count);
        }
    }
    fclose(file);
}

void export_file(struct File_records *data) {
    char ch;
    char exported_file_name[SIZE];
    int fields_per_line[SIZE];
    int line_counter = 0;
    int field_counter = 0;
    float sum = 0;

    printf("\nEnter the name of the the file of which the contents of the file \"%s\" will be exported to :\t",
           data->fname);
    gets(exported_file_name);

    FILE *file;
    file = fopen(data->fname, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(-1);
    }

    for (ch = getc(file); ch != EOF; ch = getc(file)) {
        if (ch == '\n') {
            line_counter++;              // Increment count if this character is newline
        }
        if (ch == ',') {
            field_counter++;
            fields_per_line[line_counter] += 1;
        }
    }
    float average;
    for (int i = 0; i < line_counter; ++i) {
        sum += fields_per_line[i];
        average = sum / line_counter;
    }

    FILE *export_file;  //Creation of a new file
    export_file = fopen(exported_file_name, "w");
    if (export_file == NULL) {
        printf("Error opening file!\n");
        exit(-1);
    }

    fprintf(export_file, "\nThe total number of lines was : %d", line_counter);
    fprintf(export_file, "\nThe total number of field was : %d", field_counter);
    fprintf(export_file, "\nThe average number of field per line was : %1.2f", average);

    printf("\nThe file has successfully exported with the name : \"%s\"", exported_file_name);

    // Five more frequent words
    word words[SIZE];
    char s[1000];
    int i, n, m;

    file = fopen(data->fname, "r");

    /* read all the words from the file... */
    while (!feof(file)) {
        fscanf(file, "%s", s);

        /* only insert the word if it's not punctuation */

        if (is_alpha(s[0])) {

            /* get rid of non-letters */

            remove_non_alpha(s);

            /* make all letters lowercase */

            make_lowercase(s);

            /* put this word in the list */

            insert_word(words, &n, s);

        }
    }
    /* sort the list of words by descending frequency */

    qsort((void *) words, n, sizeof(word),
          (int (*)(const void *, const void *)) wordcmp);

    /* print the words with their frequencies */
    fprintf(export_file,"\nThe 5 most common words in the file are:\n");
    for (i = 0; i < 5; i++){
        if (words[i].count != 0) {
            fprintf(export_file,"\n%s: %d\n", words[i].s, words[i].count);
        }
    }

    fclose(file);
    fclose(export_file);
}
