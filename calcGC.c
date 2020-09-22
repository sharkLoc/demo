#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
 
#define buff 1024
 
typedef unsigned long long int u_llong;
 
static void usage(int num,const char *str)
{
    if(num !=2)
    {
        fprintf(stderr,"usage: %s fqFile\n",str);
        exit(0);
    }
}
 
static u_llong* gcN(char base[buff])
{
    base[strlen(base)-1]='\0';
     
    int i;
    static u_llong gactn[]={0,0,0,0,0};
    for(i=0; i<strlen(base); i++)
    {
        if(base[i]=='G')
            gactn[0]++;
        if(base[i]=='A')
            gactn[1]++;
        if(base[i]=='C')
            gactn[2]++;
        if(base[i]=='T')
            gactn[3]++;
        if(base[i]=='N')
            gactn[4]++;
    }
    return gactn;
}
 
static void calc(const char *fqfile)
{
    //FILE *fq;
    gzFile fq;
    if((fq=gzopen(fqfile,"r")) == NULL)
    {
        perror("fopen");
        exit(1);
    }
    //fprintf(stderr,"fq file <%s> open suceed!\n",fqfile);
     
    char base[buff];
    char qual=0;
    u_llong *p=NULL;
    while((gzgets(fq,base,buff))!= NULL)  //  这里用 gzgets 替代 fgets
    {
        if(base[0]=='@')
        {
            continue;
        }
        if(base[0]=='+')
        {
            qual=1;
            continue;
        }
        if(qual==1)
        {
            qual=0;
            continue;
        }
         
        p=gcN(base); // G A C T N
    }
 
    float GClevel;
    u_llong sum=0;
    for(int i=0; i<5; i++)
    {
        sum+=*(p+i);
    }
    GClevel=(float)(*p+*(p+2)) / sum * 100;
     
    fprintf(stdout,"G:%lld\tA:%lld\nC:%lld\tT:%lld\nN:%lld\tsum:%lld\n",*p,*(p+1),*(p+2),*(p+3),*(p+4),sum);
    fprintf(stdout,"GC:%.2f%%\n",GClevel);
}
 
int main(int argc,const char *argv[])
{      
    usage(argc,argv[0]);
    calc(argv[1]);
 
    exit(0);
}
