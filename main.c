/*
 *              Basilis Georgilas A.M:
 * Authors :  &
 *              Ioannis Skoumpas A.M: 2118063
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 1000

struct File_records {
    char fname[20];
    int choice;
};

//Declaration of the procedures
void creation(struct File_records *data);

void concatenate(struct File_records *data);

void deletion(struct File_records *data);

void search(struct File_records *data);

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
    char word[20];


    // Input word to search in file
    printf("\nEnter a word to search in file :\t");
    scanf("%s", word);

    FILE *file;
    file = fopen(data->fname, "r");
    if (file == NULL) {
        printf("\nUnable to open file.\n");
        exit(-1);
    }

    //Fixing
    char str[SIZE];
    char *pos;
    int line = 0;
    int col = 0;
    char ch;

    while ((fgets(str, SIZE, file) != NULL)) {
        int char_index = 0;
        int field_positions[SIZE];
        int field_counter = 0;
        line++;
        printf("\n%d", line);
        pos = strstr(str, word);

        if (pos != NULL) {
            col = (pos - str);

            for (ch = getc(file); ch != '\n'; ch = getc(file)) {
                if (ch == ',') {
                    field_counter++;
                    field_positions[field_counter] = char_index;
                }
                char_index++;
                printf("\n\"%d\"",char_index);
            }

            for (int i = 0; i < field_counter; ++i) {
                if (field_positions[i] < col && field_positions[i + 1] > col) {
                    printf("\nWe found \"%s\" in line %d and in the %d field", word, line, i + 1);
                    break;
                } else {
                    printf("\ninner loop");
                }
            }
        }

    }
    fclose(file);
}

void export(struct File_records *data) {

    char ch;
    int fields_per_line[SIZE];
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
        average = sum / line_counter;
    }

    fclose(file);

    printf("\nThe total number of lines was : %d", line_counter);
    printf("\nThe total number of field was : %d", field_counter);
    printf("\nThe average number of field per line was : %1.2f\n", average);

    //Under Construction
    printf("\nUnder Construction\n");
    char buffer[SIZE];
    char *string[SIZE];
    char *last_token;

    //Opens file in read mode
    file = fopen(data->fname, "r");
    if (file == NULL) {
        printf("Error opening file");
        exit(0);
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

    fclose(file);
    fclose(export_file);
}
