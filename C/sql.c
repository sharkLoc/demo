#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <math.h>

char *readline(gzFile file)
{
	size_t baselen = 256;
	char *line = (char *)calloc(baselen,sizeof(char));
	if(!line) exit(1);
	
	int ch;
	size_t index = 0;
	while((ch=gzgetc(file)) != -1 && ch != 10)
	{
		line[index] = ch;
		index++;
		if(index == baselen)
		{
			baselen += 128;
			line=(char *)realloc(line,baselen);
			if(!line)
			{
				free(line);
				exit(1);
			}
		}
	}

	line[index] = '\0';  // chomp "\n"
	if(ch == -1)  return NULL;// end of file
	return line; 
}	

void fq2fa(int argc, char *argv[])
{
	gzFile fp=gzopen(argv[1],"r");
	if(!fp) exit(1);
	int flag=1;
	while(flag)
	{
		for(int i=0; i<4; i++)
		{
			char *read=readline(fp);
			if(read)
			{
				if(i==0)
				{
					memcpy(read,">",1);
					fprintf(stdout,"%s\n",read);
				}
				if(i==1)
					fprintf(stdout,"%s\n",read);
			}
			else
			{
				flag=0;
			}
			free(read);
		}
	}
	gzclose(fp);
	return;
}
uint64_t quality20(char *line)
{
	int i=0; uint64_t qCount=0;
	while(line[i]!='\0')
	{
		int q=line[i]-33;
		//if(q>20 && q<30) qCount++;
		if(q>20) qCount++;
		i++;
	}	
	return qCount;
}

uint64_t quality30(char *line)
{
	int i=0; uint64_t qCount=0;	
	while(line[i]!='\0')
	{
		int q=line[i]-33;
		if(q>=30) qCount++;
		i++;
	}
	return qCount;
}

float lowQvrate(int q,char *read)
{
	size_t len=strlen(read);
	size_t lowCount=0;
	for(int i=0; read[i]!='\0'; i++)
	{
		if(read[i]-33<q) lowCount++;
	}
	return lowCount / len ;
}

int trimfq(int argc, char *argv[])
{
	int ch,R,L,q=20,s=30; float n=0.05,l=0.3;	
	while((ch=getopt(argc,argv,"n:R:L:q:l:s:")) != -1)
	{
		switch(ch)
		{
			case 'n':
				n=atof(optarg); break; // for gap 
			case 'R':
				R=atoi(optarg); break;
			case 'L':
				L=atoi(optarg); break;
			case 'q':
				q=atoi(optarg); break; 
			case 'l':
				l=atof(optarg); break; // low qvalue rate
			case 's':
				s=atoi(optarg); break; // min len read
		}
	}

	if(optind == argc)
	{
		fprintf(stderr,"\n");
		fprintf(stderr,"usage: demo trimfq [options] <in.fq>\n\n");
		fprintf(stderr,"	-n [float]	max gap N rate in read [%.2f]\n",n);
		fprintf(stderr,"	-q [int]	low quality threshold set with -l [%d]\n",q);
		fprintf(stderr,"	-l [float]	low quality rate threshold set with -q [%.2f]\n",l);
		fprintf(stderr,"	-s [int]	min read length threshold [%d]\n",s);
		fprintf(stderr,"	-R [int]	trimfq frome Right side\n");
		fprintf(stderr,"	-L [int]	trimfq frome Left side\n");
		return 1;
	}
	gzFile fp=gzopen(argv[optind],"r");
	if(!fp) exit(1);
	gzclose(fp);
	return 0;
}

void summary(int argc, char *argv[])
{
	gzFile fp=gzopen(argv[1],"r");
	gzFile fd=gzopen(argv[2],"r");
	if(!fp || !fd) exit(1);
	float averageLen,gc,averageLen1,gc1;
	uint64_t readCount=0,baseCount=0,nCount=0,aCount=0,tCount=0,gCount=0,cCount=0;
	uint64_t readCount1=0,baseCount1=0,nCount1=0,aCount1=0,tCount1=0,gCount1=0,cCount1=0;
	uint64_t q=0,q1=0,p=0,p1=0;
	int flag=1;
	while(flag)
	{
		for(int i=0; i<4; i++)
		{
			char *read=readline(fp);
			char *read1=readline(fd);
			if(read && read1)
			{
				if(i==1)
				{
					int k=0; readCount++;
					while(read[k]!='\0')
					{
						if(read[k]=='A') aCount++;
						if(read[k]=='T') tCount++;
						if(read[k]=='G') gCount++;
						if(read[k]=='C') cCount++;
						if(read[k]=='N') nCount++;
						k++; baseCount++;
					}
					int j=0; readCount1++;
					while(read1[j]!='\0')
					{
						if(read1[j]=='A') aCount1++;
						if(read1[j]=='T') tCount1++;
						if(read1[j]=='G') gCount1++;
						if(read1[j]=='C') cCount1++;
						if(read1[j]=='N') nCount1++;
						j++; baseCount1++;
					}
				}
				if(i==3)
				{
					q+= quality20(read); 
					q1+=quality20(read1);
					p+= quality30(read);
					p1+=quality30(read1);
				}
			}
			else
			{
				flag=0;
			}
			free(read); free(read1);
		}
	}
	gzclose(fp); gzclose(fd);
	averageLen=(float)baseCount/readCount; averageLen1=(float)baseCount1/readCount1;
	gc=(float)(gCount+cCount)/baseCount*100; gc1=(float)(gCount1+cCount1)/baseCount1*100;

	fprintf(stdout,"Iterm\tfastq1\tfastq2\n");
	fprintf(stdout,"read average length:\t%d\t%d\n",(int)averageLen,(int)averageLen1);
	fprintf(stdout,"read GC content(%%):\t%.2f\t%.2f\n",gc,gc1);
	fprintf(stdout,"total read Count:\t%lu\t%lu\n",readCount,readCount1);
	fprintf(stdout,"total base Count:\t%lu\t%lu\n",baseCount,baseCount1);
	fprintf(stdout,"\n");
	fprintf(stdout,"base A Count:\t%lu(%.2f%%)\t%lu(%.2f%%)\n",aCount,(float)aCount/baseCount*100.,aCount1,(float)aCount1/baseCount1*100.);
	fprintf(stdout,"base C Count:\t%lu(%.2f%%)\t%lu(%.2f%%)\n",cCount,(float)cCount/baseCount*100.,cCount1,(float)cCount1/baseCount1*100.);
	fprintf(stdout,"base G Count:\t%lu(%.2f%%)\t%lu(%.2f%%)\n",gCount,(float)gCount/baseCount*100.,gCount1,(float)gCount1/baseCount1*100.);
	fprintf(stdout,"base T Count:\t%lu(%.2f%%)\t%lu(%.2f%%)\n",tCount,(float)tCount/baseCount*100.,tCount1,(float)tCount1/baseCount1*100.);
	fprintf(stdout,"base N Count:\t%lu(%.2f%%)\t%lu(%.2f%%)\n",nCount,(float)nCount/baseCount*100.,nCount1,(float)nCount1/baseCount1*100.);
	fprintf(stdout,"\n");
	fprintf(stdout,"base Q value(q>20) or higher Count:\t%lu(%.2f%%)\t%lu(%.2f%%)\n",q,(float)q/baseCount*100.,q1,(float)q1/baseCount1*100.);
	fprintf(stdout,"base Q value(q>=30)or higher Count:\t%lu(%.2f%%)\t%lu(%.2f%%)\n",p,(float)p/baseCount*100.,p1,(float)p1/baseCount1*100.);
	return;
}

int mergeVcf(int argc, char *argv[])
{
	gzFile fp=gzopen(f,"r"); if(!fp) exit(1); 
	gzFile fo=gzopen(o,"w+"); if(!fo) exit(1);
		
	char *vcf=NULL; int i=0;
	while((vcf=readline(fp))!=NULL)
	{
		i++;
		gzFile ff=gzopen(vcf,"r");
		if(!ff) exit(1);
		char *each=NULL;
		while((each=readline(ff))!=NULL)
		{
			if(i==1)
			{
				gzprintf(fo,"%s\n",each);
			}
			else
			{
				if(each[0]=='#')
					continue;
				gzprintf(fo,"%s\n",each);
			}
			free(each);
		}
		gzclose(ff);
		free(vcf);
	}
	gzclose(fp);
	gzclose(fo);
	fprintf(stderr,"note:merged %d files done!\n",i);
	return 0;
}
