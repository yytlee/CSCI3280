/*
* CSCI3280 Introduction to Multimedia Systems *
* --- Declaration --- *
* I declare that the assignment here submitted is original except for source
* material explicitly acknowledged. I also acknowledge that I am aware of
* University policy and regulations on honesty in academic work, and of the
* disciplinary guidelines and procedures applicable to breaches of such policy
* and regulations, as contained in the website
* http://www.cuhk.edu.hk/policy/academichonesty/ *
* Assignment 3
* Name : Lee Tsz Yan
* Student ID : 1155110177
* Email Addr : 1155110177@link.cuhk.edu.hk
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <iostream>
#include <string>

#define CODE_SIZE  12
#define TRUE 1
#define FALSE 0
#define TABLE_SIZE 4096

using namespace std;

typedef struct entry
{
	unsigned int index;
	char c;
	struct entry *previous;
	struct entry *next;
	struct entry *neighbor;
}Entry;

int table_size = TABLE_SIZE;
Entry* dictionary = (Entry*)malloc(table_size * sizeof(Entry));

/* function prototypes */
unsigned int read_code(FILE*, unsigned int); 
void write_code(FILE*, unsigned int, unsigned int); 
void writefileheader(FILE *,char**,int);
void readfileheader(FILE *,char**,int *);
int compress(FILE*, FILE*, int num_entry);
int decompress(FILE*, FILE*, int num_entry);
void init_table();
int search_dict(char c, int index);
int add_entry(char c, int index, int num);
int search_dict_by_index(int num, int prev_index, int num_entry);
char get_first_char(int index);
int write_file(FILE *, int index);

int main(int argc, char **argv)
{
    int printusage = 0;
    int	no_of_file;
    char **input_file_names;    
	char *output_file_names;
    FILE *lzw_file;

    if (argc >= 3)
    {
		if ( strcmp(argv[1],"-c") == 0)
		{		
			/* compression */
			lzw_file = fopen(argv[2] ,"wb");
        
			/* write the file header */
			input_file_names = argv + 3;
			no_of_file = argc - 3;
			writefileheader(lzw_file,input_file_names,no_of_file);
        	        	
			/* ADD CODES HERE */
			init_table();
			int num_entry = 256;
			for(int i = 0; i < no_of_file; i++){
				FILE* input_file = fopen(input_file_names[i], "rb");
				fseek(input_file, 0, SEEK_SET);
				num_entry = compress(input_file, lzw_file, num_entry);
				cout << num_entry << endl;
				fclose(input_file);
			}
			write_code(lzw_file, TABLE_SIZE, CODE_SIZE);
        	
			fclose(lzw_file);        	
		} else
		if ( strcmp(argv[1],"-d") == 0)
		{	
			/* decompress */
			lzw_file = fopen(argv[2] ,"rb");
			
			/* read the file header */
			no_of_file = 0;			
			readfileheader(lzw_file,&output_file_names,&no_of_file);
			
			/* ADD CODES HERE */
			char *ptr = strtok(output_file_names, " \n");

			init_table();
			int num_entry = 256;
			for(int i = 0; i < no_of_file; i++){
				FILE* output_file = fopen(ptr, "wb");
				cout << ptr << endl;
				num_entry = decompress(lzw_file, output_file, num_entry);
				ptr = strtok(NULL, " \n");
				fclose(output_file);
			}
			
			fclose(lzw_file);		
			free(output_file_names);
		}else
			printusage = 1;
    }else
		printusage = 1;

	if (printusage)
		printf("Usage: %s -<c/d> <lzw filename> <list of files>\n",argv[0]);
 	
	return 0;
}

/*****************************************************************
 *
 * writefileheader() -  write the lzw file header to support multiple files
 *
 ****************************************************************/
void writefileheader(FILE *lzw_file,char** input_file_names,int no_of_files)
{
	int i;
	/* write the file header */
	for ( i = 0 ; i < no_of_files; i++) 
	{
		fprintf(lzw_file,"%s\n",input_file_names[i]);	
			
	}
	fputc('\n',lzw_file);

}

/*****************************************************************
 *
 * readfileheader() - read the fileheader from the lzw file
 *
 ****************************************************************/
void readfileheader(FILE *lzw_file,char** output_filenames,int * no_of_files)
{
	int noofchar;
	char c,lastc;

	noofchar = 0;
	lastc = 0;
	*no_of_files=0;
	/* find where is the end of double newline */
	while((c = fgetc(lzw_file)) != EOF)
	{
		noofchar++;
		if (c =='\n')
		{
			if (lastc == c )
				/* found double newline */
				break;
			(*no_of_files)++;
		}
		lastc = c;
	}

	if (c == EOF)
	{
		/* problem .... file may have corrupted*/
		*no_of_files = 0;
		return;
	
	}
	/* allocate memeory for the filenames */
	*output_filenames = (char *) malloc(sizeof(char)*noofchar);
	/* roll back to start */
	fseek(lzw_file,0,SEEK_SET);

	fread((*output_filenames),1,(size_t)noofchar,lzw_file);
	
	return;
}

/*****************************************************************
 *
 * read_code() - reads a specific-size code from the code file
 *
 ****************************************************************/
unsigned int read_code(FILE *input, unsigned int code_size)
{
    unsigned int return_value;
    static int input_bit_count = 0;
    static unsigned long input_bit_buffer = 0L;

    /* The code file is treated as an input bit-stream. Each     */
    /*   character read is stored in input_bit_buffer, which     */
    /*   is 32-bit wide.                                         */

    /* input_bit_count stores the no. of bits left in the buffer */

    while (input_bit_count <= 24) {
        input_bit_buffer |= (unsigned long) getc(input) << (24-input_bit_count);
        input_bit_count += 8;
    }
    
    return_value = input_bit_buffer >> (32 - code_size);
    input_bit_buffer <<= code_size;
    input_bit_count -= code_size;
    
    return(return_value);
}


/*****************************************************************
 *
 * write_code() - write a code (of specific length) to the file 
 *
 ****************************************************************/
void write_code(FILE *output, unsigned int code, unsigned int code_size)
{
    static int output_bit_count = 0;
    static unsigned long output_bit_buffer = 0L;

    /* Each output code is first stored in output_bit_buffer,    */
    /*   which is 32-bit wide. Content in output_bit_buffer is   */
    /*   written to the output file in bytes.                    */

    /* output_bit_count stores the no. of bits left              */    

    output_bit_buffer |= (unsigned long) code << (32-code_size-output_bit_count);
    output_bit_count += code_size;

    while (output_bit_count >= 8) {
        putc(output_bit_buffer >> 24, output);
        output_bit_buffer <<= 8;
        output_bit_count -= 8;
    }


    /* only < 8 bits left in the buffer                          */    

}

/*****************************************************************
 *
 * compress() - compress the source file and output the coded text
 *
 ****************************************************************/
int compress(FILE *input, FILE *output, int num_entry)
{

	/* ADD CODES HERE */
	char c = fgetc(input);
	int prev_index = -1;
	if(c == EOF){
		cout << "empty file" << endl;
	}
	if(ferror(input)){
		cout << "error" << endl;
	}
	while(c != EOF){
		int search_result = search_dict(c, prev_index);
		if(search_result != -1){
			prev_index = search_result;
		}
		else{
			//cout << "write code: " << prev_index << endl;
			write_code(output, prev_index, CODE_SIZE);
			num_entry = add_entry(c, prev_index, num_entry);
			prev_index = search_dict(c, -1);
		}
		c = fgetc(input);
	}
	if(prev_index != -1){
		//cout << "write code: " << prev_index << endl;
		write_code(output, prev_index, CODE_SIZE);
	}
	write_code(output, TABLE_SIZE - 1, CODE_SIZE);
	cout << "compress" << endl;
	return num_entry;
}


/*****************************************************************
 *
 * decompress() - decompress a compressed file to the orig. file
 *
 ****************************************************************/
int decompress(FILE *input, FILE *output, int num_entry)
{	
	int pw = read_code(input, CODE_SIZE);
	// cout << pw << " ";
	char c = dictionary[pw].c;
	fprintf(output, "%c", c);
	int cw;
	char s;
	while((cw = read_code(input, CODE_SIZE)) != TABLE_SIZE - 1){
		// cout << cw << " ";
		if(cw < num_entry){
			// cout << "read: " << cw << endl;
			c = get_first_char(cw);
			// c = dictionary[cw].c;
			// s = dictionary[cw].c; //no need - output cw is ok
			//cout << "write " << dictionary[cw].c << endl;
			write_file(output, cw);
		}
		else{
			c = get_first_char(pw);
			// s = pw+c;
			// c = dictionary[pw].c;
			write_file(output, pw);
			fprintf(output, "%c", c);
			cout << c;
			cerr << "no entry! " << c << endl;
		}
		//p = dictionary[pw].c;
		num_entry = add_entry(c, pw, num_entry);
		pw = cw;
	}
	return num_entry;
}

void init_table(){
	for(int i = 0; i < 255; i++){
		dictionary[i].index = i;
		dictionary[i].c = i;
		dictionary[i].previous = NULL;
		dictionary[i].next = NULL;
		dictionary[i].neighbor = NULL;
	}
	for(int i = 256; i < 4095; i++){
		dictionary[i].index = 0;
		dictionary[i].c = '\0';
		dictionary[i].previous = NULL;
		dictionary[i].next = NULL;
		dictionary[i].neighbor = NULL;
	}
	cout << "init dictionary" << endl;
}

int search_dict(char c, int index){
	if(index == -1){
		return c;
	}
	Entry *e = dictionary[index].next;
	while(e != NULL){
		if(e->c == c) return e->index;
		e = e->neighbor;
	}
	return -1;
}

int add_entry(char c, int index, int num){
	//cout << "to add entry" << endl;
	if(num == table_size - 1){
		init_table();
		num = 256;
	}
	dictionary[num].index = num;
	dictionary[num].c = c;
	dictionary[num].previous = &dictionary[index];
	dictionary[num].next = NULL;
	dictionary[num].neighbor = NULL;
	//cout << "add entry: "<< num << ", char = " << dictionary[num].c;
	//cout << ", prev_num: " << dictionary[num].previous->index << ", prev_char: " << dictionary[num].previous->c << endl;

	if(dictionary[index].next == NULL){
		dictionary[index].next = &dictionary[num];
		//cout << "add first link" << endl;
		return num + 1;
	}

	Entry *e = dictionary[index].next;
	while(1){
		if(e->neighbor == NULL){
			e->neighbor = &dictionary[num];
			//cout << "link to neighbor" << endl;
			return num + 1;
		}
		e = e->neighbor;
	}
}

char get_first_char(int index){
	char c = dictionary[index].c;
	Entry *e = dictionary[index].previous;
	while(e != NULL){
		c = e->c;
		e = e->previous;
	}
	//cout << "first char: " << c << endl;
	return c;
}

int search_dict_by_index(int num, int prev_index, int num_entry){
	cout << "to search: num = " << num << endl;
	if(num_entry < num) return -1;
	if(dictionary[num].previous == NULL || dictionary[num].previous->index == prev_index){
		return dictionary[num].c;
	}
	return -1;
}

int write_file(FILE * output, int index){
	//cout << "to write" << endl;
	string out(1, dictionary[index].c);
	char first = '\0';
	Entry *e = dictionary[index].previous;
	while(e != NULL){
		out = e->c + out;
		first = e->c;
		e = e->previous;
	}
	for(int i = 0; i < out.length(); i++){
		fprintf(output, "%c", out[i]);
	}
	
	//cout << "print:                                        " << out << endl;
	return first;
}
