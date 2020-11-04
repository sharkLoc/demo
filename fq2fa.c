#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RLEN 1024

static void getfq(char *fq, char *ofq);

int main(int argc, char *argv[]){

	if(argc!=3){
		printf("infomation:\n");
		fprintf(stderr,"\tusage: %s <fastq> <fast>\n",argv[0]);
		exit(-1);
	}
	getfq(argv[1],argv[2]);
	exit(0);
}

static void getfq(char *fq, char* ofq){

	FILE *fp1=fopen(fq,"r"); 
	FILE *fp2=fopen(ofq,"w"); 

	char fline[RLEN]; 
	char read[4][RLEN]; 
	char *r=NULL; 

	while(fp1 != NULL && fp2 != NULL){
		
		for(int i=0; i<4; i++){
			r=fgets(fline,RLEN,fp1);
			if(r != NULL){
				strcpy(read[i],fline);
			}
			else{ // EOF
				goto LoopOut;
			}
		}

		for(int i=0; i<4; i++){
			switch(i)
			{
				case 0:
					memcpy(read[i],">",1);
					fprintf(fp2,"%s",read[i]);
					break;
				case 1:
					fprintf(fp2,"%s",read[i]);
					break;
				case 2:
					continue;
					break;
				case 3:
					continue;
					break;
				default:
					break;
			}
		}
	}
	LoopOut:
		free(r); 
		r=NULL;
		free(fp1);
		free(fp2);

	return;
}
