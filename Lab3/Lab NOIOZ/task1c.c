#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct virus {
    unsigned short SigSize;
    unsigned char* sig;
    char virusName[16];
} virus;


unsigned short virusSignatureLength (FILE* input){
    char virusSizeBuffer[2];
    fread(virusSizeBuffer,1,2,input);
    unsigned short size = (virusSizeBuffer[0] ) | virusSizeBuffer[1] << 8;
    return size;
}

unsigned char* virusSignature (unsigned short virusSignatureSize, FILE* input){
    unsigned char *virusSignature = (unsigned char*)calloc(virusSignatureSize, sizeof(char));
    fread(virusSignature,1, virusSignatureSize,input);
    return virusSignature;
}

char* virusName(FILE* input){
    char* virusName =calloc(16, sizeof(char));
    fread(virusName,1,16,input);
    return virusName;
}

void readVirus(virus* vir, FILE* input){
    vir->SigSize = virusSignatureLength(input);
    vir->sig = virusSignature(vir->SigSize,input);
    strcpy(vir->virusName, virusName(input));
}


void printHex(FILE* output,unsigned char buffer[], int length){
    for (int i=0; i< length; i++){
        fprintf(output,"%02x ",buffer[i]);
    }
    fprintf(output,"\n");
}

void printVirus(virus* vir, FILE* output){
    fprintf(output,"Virus Name:\n");
    fprintf(output,"%s\n", vir->virusName);
    fprintf(output,"Virus Size:\n");
    fprintf(output,"%d\n", vir->SigSize);
    fprintf(output,"Virus Signature:\n");
    printHex(output,vir->sig,vir->SigSize);
}


// linked list
typedef struct link link;

struct link {
    link *nextVirus;
    virus *vir;
};


void list_print(link *virus_list, FILE* output){
    if(virus_list != 0) {
        printVirus((virus *) virus_list->vir, output);
        list_print(virus_list->nextVirus, output);
    }
};


link* list_append(link* virus_list, link* to_add) {
    if(virus_list == NULL)
        return to_add;
    if (to_add != NULL) {
        to_add->nextVirus = virus_list;
    }
    return to_add;
}


void freeLink(link* link){
    free(link->nextVirus);
    free(link->vir);
    link->nextVirus = NULL;
};

void list_free(link *virus_list){
    if(virus_list != NULL){
        list_free(virus_list -> nextVirus);
        free(virus_list);
    }
};


/// menu


void loadSignatures(link** list_viruses){
    char filename[40];
    printf("Enter inputfile name: ");
    scanf("%s", filename);
    FILE* input= fopen( filename ,"r");
    int finished = 0;
    long bytesRead = 0;
    while(!finished){
        link* new_link = calloc(1, sizeof(link));
        virus *new_virus = calloc(1, sizeof (virus));
        readVirus(new_virus , input);
        if(ftell(input) == bytesRead) {
            finished = 1;
            new_virus = NULL;
            new_link = NULL;
        }else {
            new_link->vir = new_virus;
            bytesRead = ftell(input);
            *list_viruses = list_append(*list_viruses, new_link);
        }
    }
}


void printSignatures(link** list_viruses){
    FILE* output = fopen("output.txt", "w+");
    list_print(*list_viruses,output);
    fclose(output);
}

typedef struct func_desc{
    char* name;
    void (*fun)(link**);
} func_desc;

char quit(char c){
    if(c=='q'){
        exit(0);
    }
    return c;
}


int main(int argc, char **argv){
    link* list_viruses =NULL;

    int lower_bound = 0;
    int upper_bound = 2;
    func_desc func_array[3] = {{"Load", loadSignatures},{"Print", printSignatures},{NULL, NULL}};
    printf("Enter a number:\n");

    while(1) {
        for (int i = lower_bound; i < upper_bound; i++) {
            printf("%d) %s\n", i, func_array[i].name);
        }

        char char_func_num[20];
        scanf("%s", char_func_num);
        int func_num = atoi(char_func_num);

        if (func_num < lower_bound || func_num > upper_bound) {
            printf("not within bounds \n");
            quit('q');
        }else{
            func_array[func_num].fun(&list_viruses);
        }
    }
    return 0;
}