/*=============================================================================
| Assignment: HW 02 - Calculating the 8, 16, or 32 bit checksum for a
| given input file
|
| Author: Osman Ali
| Language: C
|
| To Compile: gcc -o -Wall checksum checksum.c
|
| To Execute: ./a.out textfile.txt checksum_size
| where the files in the command line are in the current directory.
|
| The text file contains text is mixed case with spaces, punctuation,
| and is terminated by the hexadecimal '0A', inclusive.
| (The 0x'0A' is included in the checksum calculation.)
|
| The checksum_size contains digit(s) expressing the checksum size
| of either 8, 16, or 32 bits
|
| Class: CIS3360 - Security in Computing - Fall 2020
| Instructor: McAlpin
| Due Date: 11/22
|
+=============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int long bitmask_type;
void compute_checksum(FILE *fp, int size);
void compute_checksum_word(int size, char *input, bitmask_type *check);
bitmask_type bitmask_compute(char *input, int chksum_sz);
void check_valid_checksum_arg(int chksum_sz_arg);
int compute_word_size(int checksum_size);
void format_if_necessary(int length,int word_size);
int pad_with_X(int curr_buf_len, int word_size,char *word_buffer);

#define MAXLEN 500
#define BYTE 8
void check_valid_checksum_arg(int chksum_sz_arg){
     if (chksum_sz_arg != 8 && chksum_sz_arg != 16 && chksum_sz_arg != 32){
         fprintf(stderr,"Invalid checksum value. Valid checksum sizes - 8, 16, or 32. Exiting program\n");
	  		exit(1);
     }
}

int compute_word_size(int checksum_size){
	return (checksum_size / BYTE) + 1;
}
void compute_checksum(FILE *fp, int chksum_sz)
{
      int char_ascii_val = 0;
      int char_count = 0;
      int curr_wrd_buf_len = 0, total_words_so_far = 0, pad_size = 0;
      int word_size = compute_word_size(chksum_sz);
      bitmask_type checksum = 0x0L; 

      char *word_buffer = malloc(sizeof(char) * word_size);

		if(word_buffer == NULL){
		  fprintf(stderr,"Unable to allocate memory");
		  exit(1);
		}

		printf("\n");
      while (char_ascii_val != EOF){
            if (curr_wrd_buf_len == word_size -1)
            {
                  word_buffer[curr_wrd_buf_len] = '\0';
                  printf("%s", word_buffer);
                  compute_checksum_word(chksum_sz, word_buffer, &checksum);
                  total_words_so_far++;
                  curr_wrd_buf_len = 0;
				  format_if_necessary(total_words_so_far,word_size);

            }else{
                  char_ascii_val = fgetc(fp);
                  if (char_ascii_val != EOF){
                      word_buffer[curr_wrd_buf_len] = char_ascii_val;
						}else{
                        if (chksum_sz == 16 || chksum_sz == 32)
                        {
                            if ( (curr_wrd_buf_len  < word_size - 1) && (curr_wrd_buf_len != 0) ){
	   						   pad_size = pad_with_X(curr_wrd_buf_len,word_size,word_buffer);
                               printf("%s", word_buffer);
                               compute_checksum_word(chksum_sz, word_buffer, &checksum);
                               total_words_so_far++;
                               char_count += pad_size - 1;
                               curr_wrd_buf_len = 0;
			       			   format_if_necessary(total_words_so_far,word_size);
                           }
							else
							{
                                break;
									    }

                        }
						   else{
                                break;
								         }

                  }
                  char_count++;
                  curr_wrd_buf_len++;

            }

      }

		if(word_buffer !=NULL){
		  free(word_buffer);
		}
      printf("\n");
      printf("%2d bit checksum is %8lx for all %4d chars\n", chksum_sz, checksum, char_count);
}

int pad_with_X(int curr_buf_len, int word_size,char *word_buffer){
	  int pad_size = 0;
     while (curr_buf_len < word_size -1 ){
     		word_buffer[curr_buf_len] = 'X';
     		curr_buf_len++;
     		pad_size++;
     }
     word_buffer[curr_buf_len] = '\0';
	  return pad_size;
}

bitmask_type bitmask_compute(char *input, int chksum_sz)
{
      int i = 0;
      bitmask_type mask;

      mask = input[i];
      chksum_sz -= BYTE;

      while (chksum_sz != 0){
            mask = (mask << BYTE) + input[i+1];
            chksum_sz -= BYTE;
            i++;
      }
      return mask;
}

void compute_checksum_word(int chksum_sz, char *input, bitmask_type *chksum){
      bitmask_type mask;
      mask = bitmask_compute(input, chksum_sz);
      *chksum = mask + *chksum;
      *chksum = *chksum << (64 - chksum_sz);
      *chksum = *chksum >> (64 - chksum_sz);
}
                  
void format_if_necessary(int length,int word_size){
	if (length * (word_size - 1) == 80){
                        printf("\n");
	}
}
int main(int argc, char *argv[]){
      int chksum_bits_sz;
		FILE *fp = NULL;

      if (argc != 3){
            fprintf(stderr,"Invalid usage.Usage: ./checksum #fname #checksum\n");
				exit(1);
      }

		fp = fopen(argv[1],"r");
		if(fp == NULL){
		  fprintf(stderr,"Unable to open file %s",argv[1]);
		  exit(1);
		}

      chksum_bits_sz = atoi(argv[2]);
		check_valid_checksum_arg(chksum_bits_sz);
      compute_checksum(fp, chksum_bits_sz);
		fclose(fp);
      return 0;
}
/*=============================================================================
| I Osman Ali(3943516) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+=============================================================================*/
