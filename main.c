/*
 *              Vasilis Georgoulas A.M:2118147
 * Authors :  &
 *              Ioannis Skoumpas A.M: 2118063
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // for tolower

#define MAX_SIZE 1000
#define MIN_SIZE 100

/* structure holding word frequency information */
typedef struct word {
    char s[MAX_SIZE];	/* the word */
    int	count;		/* number of times word occurs */
} word;

struct File_records {
    char fname[MIN_SIZE]; // name of the file
    int choice; // menu choice
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
  data = (struct File_records *)malloc(sizeof(struct File_records)); // ask
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
        scanf("%d",&data->choice);
        getc(stdin);

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
            free(data);
            exit(0);
        }
    }
}

// Procedure for the creation of a txt file
void creation(struct File_records *data) {

    printf("Enter a name for the file :\t");
    scanf("%s",data->fname);
    getc(stdin);

    FILE *file;
    file = fopen(data->fname, "a+"); //open the file and check if all went good
    if (file == NULL) {
        printf("\n\tError opening file!\n");
        exit(-1);
    }

    char input[MAX_SIZE];
    printf("Start typing :\t");
    fgets(input, MAX_SIZE, stdin);  //get the input from the user and place it in the file
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

    char input[MIN_SIZE];
    printf("Start typing :\t");
    fgets(input, MAX_SIZE, stdin); // Get input
    fputs(input, file);

    fclose(file);
}

//Procedure that delete a specific line-entry from the file
void deletion(struct File_records *data) {

    FILE *original_file, *temp_file;
    char ch;
    int delete_line, line = 0;
    char str[MAX_SIZE];

    //open file in read mode
    original_file = fopen(data->fname, "r");
    if (!original_file) {
        printf("\nError opening data!\n");
        exit(-1);
    }

    temp_file = fopen("replica.c", "w");
    if (!temp_file)
    {
        printf("Unable to open a temporary file to write!!\n");
        fclose(temp_file);

    }
    //Prints the entries of the file in order to choose what we want to delete
    printf("**************************************************\n");
    while (ch != EOF) {
        ch = getc(original_file);
        printf("%c", ch);
    }
    printf("**************************************************\n");

    rewind(original_file); //rewind the file pointer;
    printf("Enter the line number of the line to be deleted:\t");
    scanf("%d", &delete_line); // Get the line that going to be deleted

    // pass all entries to the replica file except from the line that is going to be deleted
    while(!feof(original_file)){
        strcpy(str,"\n");
        fgets(str,MAX_SIZE,original_file);
        if(!feof(original_file)){
            line++;
            if (line != delete_line){
                fprintf(temp_file,"%s",str);
            }

        }
    }
    fclose(original_file);
    fclose(temp_file);
    remove(data->fname); //delete the original file
    rename("replica.c", data->fname); //rename the file replica.c to original file name

    //Print the contents for the file
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
//    char word[MIN_SIZE];
//    char str[MAX_SIZE];
//    int line = 0;
//    int col = -1;
//
//    // Input word to search in file
//    printf("\nEnter a word to search in file :\t");
//    scanf("%s", word);
//
//    FILE *file;
//    file = fopen(data->fname, "r");
//    if (file == NULL) {
//        printf("\nUnable to open file.\n");
//        exit(-1);
//    }
//
//    while ((fgets(str, MAX_SIZE, file) != NULL)){
//        char ch = ' ';
//        int field_pos[MIN_SIZE];
//        int field_counter = 1;
//        int field = 1;
//        char *pos;
//
//        pos = strstr(str,word);
//        printf("%s",pos);
//
//        if(pos!=NULL){
//            // First index of word in str is
//            // Memory address of pos - memory
//            // address of str.
//            col = (pos - str);
//            int i = 0;
//            while (ch != EOF){
//                ch = getc(file);
//                if (ispunct(ch)){
//                    field_pos[field_counter] = i;
//                    field_counter++;
//                }
//                i++;
//            }
//
//            for (int j = 0; j < field_counter ; ++j) {
//                if (field_pos[j] < col && field_pos[j+1] > col){
//                    printf("\nThe word \"%s\" was found at the line %d and in the %d field", word,line,field);
//                }else{
//                    field++;
//                }
//            }
//        }
//    }
}

/*
 * This function inserts a word into the list of words.  If the word is
 * not yet in the list, a new slot is used with the count set to 1.
 * Otherwise, the count is simply incremented.
 */
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

    if (strlen (s) >= MAX_SIZE) {
        fprintf (stderr, "word too long!\n");
        exit (1);
    }
    if (*n >= MAX_SIZE) {
        fprintf (stderr, "too many words!\n");
        exit (1);
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

/* comparison function for quicksort.  this lets quicksort sort words
 * by descending order of count, i.e., from most to least frequent
 */
int wordcmp (word *a, word *b) {
    if (a->count < b->count) return +1;
    if (a->count > b->count) return -1;
    return 0;
}

/* return 1 if c is alphabetic (a..z or A..Z), 0 otherwise */
int is_alpha (char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return 1;
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

    for (i=0; s[i]; i++){
		 if (!is_alpha (s[i])){
			  remove_char(s, i);
		  }
	  }
}

/* make all the letters in s lowercase */
void make_lowercase (char *s) {
    int	i;

    for (i=0; s[i]; i++){
		 s[i] = tolower(s[i]);
	 }
}


void export(struct File_records *data) {
    char ch;
    float fields_per_line[MIN_SIZE];
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
    for (int i = 0; i < line_counter; i++) {
        sum += fields_per_line[i];  // find the average field per line;
        average = (float) sum / line_counter;
    }

    printf("\nThe total number of lines was : %d", line_counter);
    printf("\nThe total number of field was : %d", field_counter);
    printf("\nThe average number of field per line was : %.2f\n", average);

    fclose(file);

    // Code for the occurrence of the 5 most frequent words
    word words[MAX_SIZE];
    char s[MAX_SIZE];
    int i, n, m;

    n = 0;

    file = fopen(data->fname, "a+");

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

    /* if fewer than 20 words in total, just print up the the
     * first n words
     */
    if (n < 20)
        m = n;
    else
        m = 20;

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
    char exported_file_name[MAX_SIZE];
    float fields_per_line[MAX_SIZE];
    int line_counter = 0;
    int field_counter = 0;
    float sum = 0;

    printf("\nEnter the name of the the file of which the contents of the file \"%s\" will be exported to :\t",
           data->fname);
    scanf("%s",exported_file_name);

    FILE *file;
    file = fopen(data->fname, "a+");
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
        average = (float)sum / line_counter;
    }

    FILE *export_file;  //Creation of a new file
    export_file = fopen(exported_file_name, "w");
    if (export_file == NULL) {
        printf("Error opening file!\n");
        exit(-1);
    }

    fprintf(export_file, "\nThe total number of lines was : %d", line_counter);
    fprintf(export_file, "\nThe total number of field was : %d", field_counter);
    fprintf(export_file, "\nThe average number of field per line was : %.2f", average);

    printf("\nThe file has successfully exported with the name : \"%s\"", exported_file_name);

    //5 most common words
    word words[MAX_SIZE];
    char s[MAX_SIZE];
    int i, n, m;

    n = 0;

    file = fopen(data->fname, "a+");

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

    /* if fewer than 20 words in total, just print up the the
     * first n words
     */
    if (n < 20)
        m = n;
    else
        m = 20;

    /* print the words with their frequencies */
    fprintf(export_file,"\nThe 5 most common words in the file are:\n");
    for (i = 0; i < 5; i++){
        if (words[i].count != 0) {
            fprintf(export_file,"\n%s: %d\n", words[i].s, words[i].count-1);
        }
    }

    fclose(file);
    fclose(export_file);
}
