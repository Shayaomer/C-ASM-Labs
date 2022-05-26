#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
	FILE* in = stdin;
	FILE* out = stdout;
	FILE* debug_stream = stderr;
	char* file_name = "";
	char* output_file = "";
	int ch;
	int debug = 0;
	int charCount = 0; 
	char  action = '+';
	int key = 0;
	int enc_amount = 0;
	int encrypt = 0;
	char first_l = 'a';
	int first = 1;

	for(int i=1; i<argc; i++){
		if(strcmp(argv[i], "-D") == 0) 
			debug = 1;
		else if(strncmp(argv[i], "-e", 2) == 0 || strncmp(argv[i], "+e", 2) == 0) {
			encrypt = 1;
			if(strncmp(argv[i], "-", 1) == 0)
				action = '-';
			if('0' <= argv[i][2] && '9' >= argv[i][2])
				key = (int)argv[i][2] - 48;
			else if('A' <= argv[i][2] && 'F' >= argv[i][2])
				key = (int)argv[i][2] - 55;
			else if('a' <= argv[i][2] && 'f' >= argv[i][2])
				key = (int)argv[i][2] - 87;
			else {
				printf("Invalid encryption\n");
				return 0;
			}
		}
		else if (strncmp(argv[i], "-i", 2) == 0){
			file_name = argv[i] + 2;
			in = fopen(file_name, "r");
			if (in == NULL) {
				printf("Invalid input file");
				return 0;
			}
		}
		else if (strncmp(argv[i], "-o", 2) == 0){
			output_file = argv[i] + 2;
			out = fopen(output_file, "w");
			if (out == NULL){
				printf("Invalid output file");
				return 0;
			}
		}
	}

	if(encrypt == 1) {
		if (action == '-'){
			enc_amount = key;
			while ((ch = fgetc(in)) != EOF) {
				if(ch != 10) {
					if (enc_amount > 0)
						enc_amount --;
					else 
						fputc(ch, out);
				}
				else{
					enc_amount = key;
					printf("\n");
				}
			}
		}
		else {
			while ((ch = fgetc(in)) != EOF) {
				if (first == 1){
					first_l = ch;
					first = 0;
				}
				if(ch != 10) {
					fputc(ch, out);
				}
				else {
					for (int i = 0; i < key; i++)
						fputc(first_l, out);
					printf("\n");
				}
			}
		}
	}

	else if(debug == 1){ 
		while ((ch = fgetc(in)) != EOF) {
			if(ch != 10) {
				if(ch>='A' && ch<='Z'){
					fprintf(debug_stream, "%d %d\n", ch, 46);
					charCount = charCount + 1;
					fputc(46, out);
				}
				else {
					fprintf(debug_stream, "%d %d\n", ch, ch);
					fputc(ch, out);
				}
			}
			else {
				fprintf(debug_stream, "number of converted letters: %d\n", charCount);
				charCount = 0;
				printf("\n");
			}
		}
	}

	else {
		while ((ch = fgetc(in)) != EOF) {
			if (ch>='A' && ch<='Z')
				fputc(46, out);
			else
				fputc(ch, out);
		}
	}
	


	return 0;
}
