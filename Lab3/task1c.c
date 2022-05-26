#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus {
    unsigned short SigSize;
    unsigned char* sig;
    char virusName[16];
} virus;

typedef struct link link;
 
struct link {
    link *nextVirus;
    virus *vir;
};

void readVirus(virus* vir, FILE* input){
    unsigned char* sig_size = calloc(2, 1);
    if (fread(sig_size, sizeof(char), 2, input) == 0)
        return;
    unsigned short size = (sig_size[0])|sig_size[1]<<8;
    vir->SigSize = size;
    unsigned char* sig = calloc(vir->SigSize, 1);
    fread(sig, sizeof(char), vir->SigSize, input);
    vir->sig = sig;
    char virusName[16];
    fread(virusName, 1, 16, input);
    strcpy(vir->virusName, virusName);
}

void printVirus(virus* vir, FILE* output){
    fprintf(output, "Virus Name: %s\n", vir->virusName);
    fprintf(output, "Virus Size: %d\n", vir->SigSize);
    fprintf(output, "Signature:\n");
    for(unsigned short i = 0; i< vir->SigSize; i++)
        fprintf(output, "%X ", vir->sig[i]);
    fprintf(output, "\n\n");

    
}


 /* Print the data of every link in list to the given stream.
  Each item followed by a newline character. */
void list_print(link *virus_list, FILE* output){
    if (virus_list == NULL)
        return;
    printVirus(virus_list->vir, output);
    list_print(virus_list->nextVirus, output);
}; 

/* Add the given link to the list 
        (either at the end or the beginning, depending on what your TA tells you),
        and return a pointer to the list (i.e., the first link in the list).
        If the list is null - return the given entry. */
link* list_append(link* virus_list, link* to_add){
    if (virus_list == NULL)
        return to_add;
    if (to_add != NULL){
        virus_list->nextVirus = to_add;
        return virus_list;
    }
    return virus_list;
} 
     
/* Free the memory allocated by the list. */
void list_free(link *virus_list){
    if(virus_list->nextVirus != NULL)
        list_free(virus_list->nextVirus);
    free(virus_list->vir);
}

void load_virus_list(link** vir_list, FILE* input){
    
    virus* vir = calloc(1, sizeof(virus));
    link* to_add = calloc(1, sizeof(link));
    readVirus(vir, input);
    to_add->vir = vir;
    *vir_list = list_append(*vir_list, to_add);
    while (1){
        virus* vir = calloc(1, sizeof(virus));
        link* to_add = calloc(1, sizeof(link));
        readVirus(vir, input);
        if(vir->SigSize == 0){
            break;
            free(vir);
            free(to_add);
        }
        to_add->vir = vir;
        *vir_list = list_append(*vir_list, to_add)->nextVirus;
    }
}  

int main(int argc, char **argv){
    // char* filename = argv[1];
    FILE* filestream = NULL;
    // filestream = fopen(filename, "rb");
    // virus* vir = calloc(1, sizeof(virus));
    
    // readVirus(vir, filestream);
    FILE* out_stream = fopen("output.txt", "w");
    link* vir_list = calloc(1, sizeof(link));
    // link* to_add = calloc(1, sizeof(link));
    // to_add->vir = vir;
    // list_append(vir_list, to_add);

    while (1){
        int pressed;
        char to_load[50];
        printf("1) Load signatures \n2) Print signatures \n3) Quit \n");
        scanf("%d", &pressed);

        if (pressed == 1){
            scanf("%s", to_load);
            filestream = fopen(to_load, "rb");
            if (filestream == NULL)
                printf("\nFile not found, try again.\n");
            else 
                printf("\nFile loaded.\n");
        }

        if (pressed == 2){
            if (filestream == NULL)
                printf("\nNO LOADED FILE!\n");
            else {
                load_virus_list(&vir_list, filestream);
                list_print(vir_list, out_stream);
            }
        } 
    }
    


    
    // list_free(vir_list);
    // free(vir);
    return 0;
}