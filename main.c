#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100

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

    while(choice != 0){

        printf("\n1. Give a name for a file"
                        "\n2. Add a new record"
                        "\n3. Delete a record"
                        "\n4. Search a record"
                        "\n5. Export the statistics in the screen"
                        "\n6. Export the statistics in a txt file"
                        "\n0. Exit the program");

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
        }
    }
}
// Procedure for the creation of a txt file
void creation(char *ptr_fname){


    printf("\nEnter a name for the file :\t");
    gets(ptr_fname);


    FILE *file;
    file = fopen(ptr_fname,"a+");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(-1);
    }

    char input[SIZE];
    printf("Start typing :\t");
    fgets(input, SIZE, stdin);
    fputs(input, file); //Place input into the file

    fclose(file);
}


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
    fgets(input, SIZE, stdin);
    fputs(input, file);

    fclose(file);
}

void deletion(char *ptr_fname){

    FILE *main_file, *temp_file;
    char ch;
    int delete_line, temp = 1;

    //open file in read mode
    main_file = fopen(ptr_fname, "r");
    ch = getc(main_file);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = getc(main_file);
    }
    //rewind
    rewind(main_file);
    printf("\nEnter the line number of the line to be deleted:\t");
    scanf("%d", &delete_line);

    //open new file in write mode
    temp_file = fopen("replica.c", "w");
    ch = ' ';       //resetting ch
    while (ch != EOF)
    {
        ch = getc(main_file);
        //except the line to be deleted
        if (temp != delete_line)
        {
            //copy all lines in file replica.c
            putc(ch, temp_file);
        }
        if (ch == '\n')
        {
            temp++;
        }
    }
    fclose(main_file);
    fclose(temp_file);
    remove(ptr_fname);

    //rename the file replica.c to original name
    rename("replica.c", ptr_fname);
    printf("\nThe contents of file after being modified are as follows:\n\t");
    main_file = fopen(ptr_fname, "r");
    ch = getc(main_file);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = getc(main_file);
    }
    fclose(main_file);
}

void search(char *ptr_fname){
    char word[SIZE];

    // Input word to search in file
    printf("Enter word to search in file: ");
    scanf("%s", word);

    FILE *file
    file = fopen(ptr_fname,"r");
    if (file == NULL)
    {
        printf("Unable to open file.\n");
        exit(-1);
    }

    // Find index of word in fptr
    indexOf(fptr, word, &line, &col);

}

void export(char *ptr_fname){

    char ch;
    int fields_per_line[SIZE];
    int line_counter = 0;
    int field_counter = 0;
    float sum = 0;
    
   FILE *file;
   file = fopen(ptr_fname,"r");
   if (file == NULL)
   {
        printf("Error opening file!\n");
        exit(-1);
   }

    for (ch = getc(file); ch != EOF; ch = getc(file)){
        if (ch == '\n'){
            line_counter++;              // Increment count if this character is newline
        }
        if (ch == ','){
            field_counter++;
            fields_per_line[line_counter] += 1;
        }
    }

    for (int i = 0; i < line_counter; ++i) {
        sum += fields_per_line[i];
    }

    printf("\nThe total number of lines was : %d",line_counter);
    printf("\nThe total number of field was : %d",field_counter);
    printf("\nThe average number of field per line was : %.f", (float)sum / line_counter);

    fclose(file);

}

void export_file(char *ptr_fname){
    char ch;
    int fields_per_line[SIZE];
    int line_counter = 0;
    int field_counter = 0;
    float sum = 0;

    FILE *file;
    file = fopen(ptr_fname,"r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(-1);
    }

    for (ch = getc(file); ch != EOF; ch = getc(file)){
        if (ch == '\n'){
            line_counter++;              // Increment count if this character is newline
        }
        if (ch == ','){
            field_counter++;
            fields_per_line[line_counter] += 1;
        }
    }

    for (int i = 0; i < line_counter; ++i) {
        sum += fields_per_line[i];
    }

    FILE *export_file;  //Creation of a new file
    export_file = fopen("statistics.txt", "w+");
    if (export_file == NULL)
    {
        printf("Error opening file!\n");
        exit(-1);
    }
    printf("\nThe file has successfully exported with the name : \"statistics.txt\"");

    fprintf(export_file,"\nThe total number of lines was : %d",line_counter);
    fprintf(export_file,"\nThe total number of field was : %d",field_counter);
    fprintf(export_file,"\nThe average number of field per line was : %.f", (float)sum / line_counter);

    fclose(file);
    fclose(export_file);
}