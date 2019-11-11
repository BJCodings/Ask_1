/*
 *              Basilis Georgilas A.M:
 * Authors :  &
 *              Ioannis Skoumpas A.M: 2118063
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define SIZE 100000

//Declaration of the procedures
void creation(char *ptr_fname);
void concatenate(char *ptr_fname);
void deletion(char *ptr_fname);
void search(char *ptr_fname);
void export(char *ptr_fname);
void export_file(char *ptr_fname);

int main() {

    int choice =-1;
    char fname[20];
    char *ptr_fname = fname; // pointer for the name of the file
    printf("asdf");

    while(choice != 0){

        printf("\n********************************************************************************************");
        printf("\n1. Give a name for a file"
                        "\n2. Add a new record"
                        "\n3. Delete a record"
                        "\n4. Search a record"
                        "\n5. Export the statistics in the screen"
                        "\n6. Export the statistics in a txt file"
                        "\n0. Exit the program");
        printf("\n********************************************************************************************");
        printf("\nSelect an option:\t");
        scanf("%d",&choice);
        fflush(stdin);

        if(choice == 1){
            creation(ptr_fname);
        }else if(choice == 2){
            concatenate(ptr_fname);
        }else if(choice == 3){
            deletion(ptr_fname);
        }else if(choice == 4){
            search(ptr_fname);
        }else if(choice == 5){
            export(ptr_fname);
        }else if(choice == 6){
            export_file(ptr_fname);
        }else if(choice == 0){
            printf("\nTerminating program...");
            exit(0);
        }
    }
}
// Procedure for the creation of a txt file
void creation(char *ptr_fname){

    printf("Enter a name for the file :\t");
    gets(ptr_fname);

    FILE *file;
    file = fopen(ptr_fname,"a+");
    if (file == NULL)
    {
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
void concatenate(char *ptr_fname){

    FILE *file;
    file = fopen(ptr_fname,"a+");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(-1);
    }

    char input[SIZE];
    printf("Start typing :\t");
    fgets(input, SIZE, stdin); // Get input
    fputs(input, file);

    fclose(file);
}

//Procedure that delete a specific line-entry from the file
void deletion(char *ptr_fname){

    FILE *original_file, *temp_file;
    char ch;
    int delete_line, line = 1;

    //open file in read mode
    original_file = fopen(ptr_fname, "r");
    ch = getc(original_file);

    //Prints the entries of the file
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = getc(original_file);
    }

    rewind(original_file); //rewind the file pointer;
    printf("\nEnter the line number of the line to be deleted:\t");
    scanf("%d", &delete_line); // Get the line that going to be deleted

    //open new file in write mode
    temp_file = fopen("replica.c", "w");
    ch = ' ';  //resetting ch
    while (ch != EOF)
    {
        ch = getc(original_file);
        //except the line to be deleted
        if (line != delete_line)
        {
            //copy all lines in file replica.c
            putc(ch, temp_file);
        }
        if (ch == '\n')
        {
            line++; //increment line
        }
    }
    fclose(original_file);
    fclose(temp_file);
    remove(ptr_fname); //delete the original file

    //rename the file replica.c to original file name
    rename("replica.c", ptr_fname);

    printf("\nThe contents of file after being modified are as follows:\n\t");
    original_file = fopen(ptr_fname, "r");
    ch = getc(original_file);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = getc(original_file);
    }
    fclose(original_file);
}

void search(char *ptr_fname){
    char word[SIZE];
    char ch;
    char str [SIZE]; //
    int line;
    char *pos;
    line = -1;
    int field_counter = 0; // counts the field(commas)
    int field_position[1000]; // hold the pos of every field
    int index = 0;
    int field;
    bool flag = false;

    // Input word to search in file
    printf("Enter a word to search in file: ");
    scanf("%s", word);

    FILE *file;
    file = fopen(ptr_fname,"r");
    if (file == NULL)
    {
        printf("Unable to open file.\n");
        exit(-1);
    }

    //Fixed
    while ((fgets(str, SIZE, file)) != NULL){

        line += 1; // counts the lines

        pos = strstr(str, word); // returns the pos of the founded word

        if (pos != NULL){
            for (ch = getc(file); ch != '\n'; ch = getc(file)) {

                if (ch == ',') {
                    field_counter++; // count how many field there are
                    field_position[field_counter] = index; // saves the pos of each found field
                    flag = true;
                }
                index++;
            }

            field = 1;
            for (int i = 0; i < sizeof(field_position) ; ++i) { // finds the field
                if(field_position[i] <= field && field_position[i + 1] >= field ){
                    break;
                }else{
                    field++;
                }
            }
            if (flag){
                printf("\t\nThe word \"%s\" was found at line %d and in the %d field",word, line, field);
            }
        }
    }
    if(flag == false){
        printf("\nWe couldn't find the word that you looking for");
    }
    fclose(file);
}

void export(char *ptr_fname) {

    char ch;
    int fields_per_line[SIZE];
    int line_counter = 0;
    int field_counter = 0;
    float sum = 0;

    FILE *file;
    file = fopen(ptr_fname, "r");
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

    for (int i = 0; i < line_counter; ++i) {
        sum += fields_per_line[i];  // find the average field per line;
    }

    fclose(file);

    printf("\nThe total number of lines was : %d", line_counter);
    printf("\nThe total number of field was : %d", field_counter);
    printf("\nThe average number of field per line was : %.2f\n", (float) sum / line_counter);

    //Under Construction
    printf("\nUnder Construction");
    char buffer[SIZE];
    char *string[SIZE];
    char *last_token;

    //Opens file in read mode
    file = fopen(ptr_fname, "r");
    if (file == NULL) {
        printf("File not found");
        exit(EXIT_FAILURE);
    }

    //Since, C doesn't provide in-built function,
    //following code will split content of file into words
    int i = 0;
    while (fgets(buffer,SIZE,file) !=NULL){
        last_token = strtok( buffer, " " );
        while( last_token != NULL ){
            string [i] = last_token;
            last_token = strtok( NULL, " " );
            i++;
        }
    }

    for (int j = 0; j < i ; ++j) {
        printf("%s",string[j]);
    }

}

void export_file(char *ptr_fname) {
    char ch;
    char exported_file_name[SIZE];
    int fields_per_line[SIZE];
    int line_counter = 0;
    int field_counter = 0;
    float sum = 0;

    printf("\nEnter the name of the the file of which the contends of the file \"%s\" will be exported to :\t",
           ptr_fname);
    gets(exported_file_name);

    FILE *file;
    file = fopen(ptr_fname, "r");
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

    for (int i = 0; i < line_counter; ++i) {
        sum += fields_per_line[i];
    }

    FILE *export_file;  //Creation of a new file
    export_file = fopen(exported_file_name, "w+");
    if (export_file == NULL) {
        printf("Error opening file!\n");
        exit(-1);
    }
    printf("\nThe file has successfully exported with the name : \"%s\"", exported_file_name);

    fprintf(export_file, "\nThe total number of lines was : %d", line_counter);
    fprintf(export_file, "\nThe total number of field was : %d", field_counter);
    fprintf(export_file, "\nThe average number of field per line was : %.f", (float) sum / line_counter);

    fclose(file);
    fclose(export_file);
}
