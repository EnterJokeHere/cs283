#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
int reverse_string(char *, int);
int print_words(char *, int);



int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    char *pointer = buff; //pointer to start of buff
    char last_char = '\0'; // last char processed, initially null
    int count = 0; //Char counter

    while (*user_str == ' ' || *user_str == '\t') {     // Skip leading spaces if there are any
        user_str++;
    }

    // Loop through the user-provided string until its end or the buffer limit's
    while (*user_str && count < len) {
        if (*user_str == ' ' || *user_str == '\t') { // Check if the current character is a space or tab.
            if (last_char != ' ') {
                *pointer++ = ' '; // Write a space to the buffer and increment the pointer
                count++;
            }
        
        }
        
        else { // If the character is not a space or tab:
            *pointer++ = *user_str; // Write the current character to the buffer and increment
            count++;
        }
        last_char = *user_str; // Update the last character processed.
        user_str++; // Move to the next character in the user-provided string.
    }

    // If the string is too long, it won't end with \0
    if (*user_str != '\0') { 
        printf("Error -1: The user supplied string is too large");
        return -1;
    }

     // Remove the last space if it's the final character in the buffer
    if (last_char == ' ' && count > 0) {
        pointer--;  // Move pointer back to overwrite the last space
        count--;
    }

    // Fill the remaining buffer space with dots ('.') until the buffer size is reached.
    while (count < len) {
        *pointer++ = '.';
        count++;
    }
    return count;
}

void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]\n");
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int words = 0;
    int in_word = 0;

    for (int i = 0; i < str_len; i++) {
        if (buff[i] != ' ' && buff[i] != '.') { // Count only non-space, non-dot characters
            if (!in_word) {
                in_word = 1;
                words++;
            }
        } else {
            in_word = 0;
        }
    }
    return words;
}


//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int reverse_string(char *buff, int len) {
    char *start = buff, *end = buff + len - 1;
    while (*end == '.') end--; // Ignore dot padding
    
    while (start < end) {
        char temp = *start;
        *start++ = *end;
        *end-- = temp;
    }
    return 0; // Return success
}

int print_words(char *buff, int len) {
    int word_count = 0;
    int char_count = 0;

    printf("Word Print\n----------\n");
    for (int i = 0; i < len && buff[i] != '.'; i++) {
        if (buff[i] != ' ') {
            if (char_count == 0) { // Start of the new word
                printf("%d. ", word_count + 1);
            }
            putchar(buff[i]);
            char_count++;
        } else if (char_count > 0) { // End of the word
            printf("(%d)\n", char_count);
            char_count = 0;
            word_count++;
        }
    }

    // Handle the last word if still counting characters
    if (char_count > 0) {
        printf("(%d)\n", char_count);
        word_count++;
    }

    printf("\nNumber of words returned: %d\n", word_count);
    return word_count;
}




int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    // This is safe because the check ensures that argc is at least 2
    // and verifies that argv[1] starts with a '-' character, which is required 
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    // This checks that the user has given the necessary arguments for the program to function. 
    // `argc < 3` means that the program expects at least 3 arguments: the program name, the option flag, and the user input string. 
    // If fewer than 3 arguments are provided, it is invalid, and we show the usage instructions and exit with error code 1.

    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *) malloc(BUFFER_SZ * sizeof(char));  // Allocate space for the buffer
    if (buff == NULL) {  // Check if malloc fails
        printf("Error: Unable to allocate memory for buffer\n");
        exit(99);  // Exit with code 99 if memory allocation fails
    }


    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement (done)
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        
        case 'r': { // Reverse 
            rc = reverse_string(buff, user_str_len);

            break;
        }
        
        case 'w': { // Print words
            print_words(buff, BUFFER_SZ);
            break;
        }

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
//          Providing both the pointer and the length ensures functions work correctly with variable-sized buffers, 
//          avoid overflows, and handle partially filled buffers, making the program more adaptable overall.
