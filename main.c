/**@file termfrequency.c
 * @brief A program to find the term frequency and document frequency
 * 
 * This program takes directory name form the user,
 * reads all its text files, takes some terms from user 
 * and finds how many times those terms were repeated and 
 * in how many files those terms appear. It also saves the searches, 
 * display the search history and allows us to delete them
 * 
 * @author: Mohit Shahi
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>


struct frequency{
    char word[20];
    int term;
    int document;
};

struct termData {
    char directory[20];
    int totalFiles;
    int totalTerms;
    struct frequency freq[20];

};

//function prototypes
void path(char ch[]);
int termFreq(char term[], char file[]);
int listOfFiles(char folder[], char files[][20]);
void displayResult(struct termData data);
void findTermFrequency(char argv[]);
void showHistory();
void deleteHistory();

/**@brief The main function
* 
*  This is the entry point of the program
*  This function displays the main menu and calls specific function according to user's choice
*
*  @returns 0
*/

int main(int argc, char* argv[]){
    int choice;
    top:
    system("cls");
    printf("\n\n\t********************** WELCOME **********************\n\n");
    printf("What would you like to do?\n");
    printf("1.  Find term and document frequencies\n");
    printf("2.  Show search history\n");
    printf("3.  Delete search history\n");
    printf("4.  Exit\n");
    printf("Enter the option number:\n");
    printf(">> ");
    scanf("%d%*c", &choice);
    switch(choice){
        case 1:
            system("cls");
            findTermFrequency(argv[0]); 
            fflush(stdin);
            printf("\n\nPress any key to go back to the main menu...\n");
            scanf("%*c");            
            goto top;
        case 2:
            system("cls");
            showHistory();
            printf("\n\nPress any key to go back to the main menu...\n");
            scanf("%*c");
            goto top;
        case 3:
            system("cls");
            deleteHistory();
            printf("\n\nPress any key to go back to the main menu...\n");
            scanf("%*c");
            goto top;
        case 4:
            break;
        default:
            printf("Invalid choice!!\n");
            goto top;
    }
    printf("\n\nThank you for using the program.\n\n");
    return 0;

}
/**@brief 
 * 
 * This function stores a part of path reuqired to read the files from a directory
 * 
 * @param ch - array of characters(a string)
 * 
 */
void path(char ch[]){
    int i = strlen(ch);
    for (i ; i >=0 ; i--) {
        if(ch[i] == '\\'){ //serches for the first appearance of '\' character form the back and puts a null characer after that character 
            ch[i+1] = '\0';
            break;
        } 
    }
}

/**@brief counts how many times a term is repeated in a text file
 * 
 * This function opens a file, reads words one by one, compares with the passed term and increases the
 * count each time the term is repeated in the text file
 * 
 * @param term - term that is searched 
 * @param file - path of text file in which the term is searched 
 * 
 * @returns count i.e. how many times the given term is repeated in given text file
 */

int termFreq(char term[], char file[]){

    FILE *fp = fopen(file, "r"); 

	char c;
    char word[20]; 
	int cnt = 0;
    int i =0; 
	while((c = fgetc(fp)) != EOF){ //reads a character from text file until the end of file
		if(c == ' ' || c == '\n' || c == '.' || c == ',') { 
            word[i] = '\0';
            if(!(strcmp(word, term))){
                cnt++;
            }
            i = 0;
		} 
		else{ 
            word[i++] = c;
		} 
	} 

	fclose(fp);
    return cnt;
}





/**@brief reads a directory and sotres names of all the files in an array
 * 
 * This function opens a folder passed by user, stores each file names of the folder in an array of string passed
 * by user as argument
 * Counts how many files are stored in the arry
 * 
 * @param folder - folder name 
 * @param files - an array of strings where file names are stored 
 * 
 * @returns count i.e. how many files were stored 
 */

int listOfFiles(char folder[],char files[][20]){
    int cnt = 0;
    int i = 0;
    //reading the folder given by the user
    DIR* dir = opendir(folder);
    struct dirent* de;

    // showing error if cannot read the directory
    if(dir == NULL){
        printf("\nCannot open the directory or directory with the name '%s' does not exist.\n", folder);
        exit(-1);
    }
    //reading each file in the directory and counting the files
    while((de = readdir(dir)) != NULL) {
        if((strcmp(de->d_name, ".") == 0) || (strcmp(de->d_name, "..") == 0)) continue;
        strcpy(files[i], de->d_name);
        cnt ++;
        i++;
    }
    closedir(dir);
    return cnt;
}

/**@brief displays result in format
 * 
 * This void function only prints the result or data by well fomatting
 * 
 * @param data - structure of termData where datas are already stored  
 * 
 */

void displayResult(struct termData data){
    printf("\n\t---------------------------------------------------------------------------");
    printf("\n\tDirectory : %s", data.directory);
    printf("\n\t---------------------------------------------------------------------------");
    printf("\n\tTerm\t\t\tTerm Frequency\t\t\tDocument Frequency\n\n");
    for(int i = 0; i<data.totalTerms; i++) {
        printf("\t%s\t\t\t%d\t\t\t\t%d\n", data.freq[i].word, data.freq[i].term, data.freq[i].document);
    }
    printf("\n\t---------------------------------------------------------------------------");
    printf("\n\tTotal files: %d", data.totalFiles);
    printf("\n\t---------------------------------------------------------------------------");
}

/**@brief This funtion does all the functionality of finding the term and document frequency
 * 
 * This function asks user to enter a directory name to scan, terms that the user want to find
 * the frequencies of 
 * At last it saves the search in data file(history.dat)
 * 
 * @param term - term that is searched 
 * @param file - path of text file in which the term is searched 
 * 
 */

void findTermFrequency(char argv[]){
    int i;
    struct termData data;

    //asking a folder name to read
    printf("Enter a directory to read:\t");
    fgets(data.directory, 20, stdin);
    data.directory[strlen(data.directory)-1] = '\0';


    //array of files 
    char files[30][20];    
    data.totalFiles = listOfFiles(data.directory, files); //getting the total number of files and storing all the file name in array 

    //asking for number of terms
    printf("How many terms would you like to search for?\t");
    scanf("%d", &data.totalTerms);

    //array of terms
    char term[data.totalTerms][20];

    //asking for the terms
    printf("Enter the terms:\n");
    for(int i = 0; i<data.totalTerms; i++) {
        printf("Term %d:\t", i+1);
        scanf("%s", term[i]);
        strcpy(data.freq[i].word, term[i]);
    }
    
    termFreq("hello", "filename");

    //getting the path of the folder 
    path(argv);
    strcat(argv, data.directory);
    strcat(argv,"\\");
    for(int j = 0; j<data.totalTerms; j++) {
        data.freq[j].document = 0;
        data.freq[j].term = 0;
 
        for(i = 0; i<data.totalFiles; i++){
            char fullPath[50];
            strcpy(fullPath, argv);
            strcat(fullPath, files[i]); //full path to the text file
            int n = termFreq(term[j], fullPath);
            if(n>0) data.freq[j].document++; //if the term is repeated atleast once in the text file the document frequency is increased by 1
            data.freq[j].term += n; //adding the total 
        }
    }
    argv[strlen(argv)-1] = '\0';

    //displaying the result
    displayResult(data);

    //storing the data in history.dat file
    FILE *fp = fopen("history.dat", "ab");
    if(fp == NULL) {
        printf("Cannot write the data!\n");
    }
    fwrite(&data, sizeof(struct termData), 1, fp);
    fclose(fp);
}

/**@brief shows all the last searches 
 * 
 * This function reads a data file containing history of last searchese and displays them
 * 
 */ 

void showHistory(){
    struct termData data;
    FILE *fp = fopen("history.dat", "rb");
    if(fp == NULL){
        printf("\n\tError reading the file\n");
        exit(-1);
    }
    int i = 1;
    printf("\n\t\t\t\t\tSEARCH HISTORY\n\n");
    if(!(fread(&data, sizeof(struct termData), 1, fp))) printf("\n\tNo data found!"); // if the files contains nothing
    rewind(fp); // if the file contains some data reading from the beginning 

    while(fread(&data, sizeof(struct termData), 1, fp)){
        printf("\n\t#%d", i++);
        displayResult(data);
        printf("\n\n");
    }
    fclose(fp);
}

/**@brief deletes all the last searches 
 * 
 * This function deletes all the data by creating a new .dat file(history.dat) with no data
 * 
 */ 

void deleteHistory(){
    FILE *fp = fopen("history.dat", "wb");
    if(fp == NULL) {
        printf("\n\tError deleting data.\n");
    }
    else {
        printf("\n\tSuccessfully deleted search history\n");
        
    }
    fclose(fp);
}
