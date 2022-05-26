#include <stdio.h>
void printHex(unsigned char buffer[], size_t length){
    for (size_t i = 0; i<length; i++)
        printf("%X ", buffer[i]);
}

int main(int argc, char **argv){
    char* filename = argv[1];
    FILE* filestream;
    unsigned char buffer[2000];
    filestream = fopen(filename, "rb");
    size_t read_len = fread(buffer, sizeof(char), 2000, filestream); 
    printHex(buffer, read_len);





    return 0;
}