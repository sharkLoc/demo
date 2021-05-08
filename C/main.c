#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "sql.h"

static int usage()
{
	fprintf(stderr,	"\n");
	fprintf(stderr,	"Usage	demo <command> \n\n");
	fprintf(stderr,	"	fq2fa		trans fq to fa seq\n");
	fprintf(stderr,	"	stats		summary of the fastq file stats\n");
	fprintf(stderr,	"	trimfq		filt and cut reads\n");
	fprintf(stderr, "	mergeVcf	merge all vcf files in list\n");
	fprintf(stderr, "\n");
	return 1;
}

int main(int argc, char *argv[])
{
	if(argc==1) return usage();	
	if(!strcmp(argv[1],"fq2fa"))
	{
		fq2fa(argc-1,argv+1);
	}
	else if(!strcmp(argv[1],"stats"))
	{
		summary(argc-1,argv+1);
	}
	else if(!strcmp(argv[1],"trimfq"))
	{
		return trimfq(argc-1,argv+1);
	}
	else if(!strcmp(argv[1],"mergeVcf"))
	{
		return mergeVcf(argc-1,argv+1);
	}
	else
	{
		fprintf(stderr,"error: no command [%s]!\n",argv[1]);
	}

	return 0;
}
© 2021 GitHub, Inc.
