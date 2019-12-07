#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>


typedef struct{
	char code[4];
	}codontype;

typedef struct{
	int occur ;
	char acid;
	}NUM_OCCUR;
	
typedef struct{
	char annotation[300];
	char sequence[7000];
}CODETYPE;


NUM_OCCUR occurence[26];

int spacecheck=-1, numberofproteins;

int len,opensequence=0;

int cmpfunc(const void * a, const void * b);
void printhistogram(FILE *outfile, char*protein);
int errorcheck=0,dnacheck=0;
void input(char *annotation,char *sequence,FILE *f);

int count_and_multitounifasta(char *file);
void storecodon(int n,char *sequence,char *protein);
void deletechar(char *sequence);
void transcription(char *sequence);
void output(int i,int n,char *protein,FILE *outfile,char *order,char *annotation);
void complimentarysequence(char *sequence);
void sequencereversal(char *sequence);

int main(int argc, char*argv[])
{
	char sequence[7000], protein[5000];
	numberofproteins=count_and_multitounifasta(argv[1]);
	FILE *f= fopen("output.txt","r");
	if(f==NULL)
	{	printf("Enter a valid input file!\n");
 		exit(1);
	}
	FILE * outfile=fopen(argv[2],"a");
    if(outfile==NULL)
    { 
		printf("Enter a valid output file!\n");
 		exit(2);
    }
    int i,j;
          
    for(i=0;i<numberofproteins;i++)
   	{
        char annotation[500];
        sequence[0]='\0';
   		input(annotation,sequence,f);
       	transcription(sequence);
		len=strlen(sequence)/3; 
			//printf("%d",strlen(sequence)/3);
                 	
  		for(j=0;j<3;j++)
    	{ 
    	 	for(int x=0; x < 26; x++)
			{
				occurence[x].occur = 0;
				occurence[x].acid =(char)(x+65);
			}
			storecodon(j,sequence,protein);
            output(i,j+1,protein,outfile,"original",annotation);
            printhistogram(outfile,protein);
        }
		complimentarysequence(sequence);
	 	for(j=0;j<3;j++)
   		{ 
       		for(int x=0; x < 26; x++)
			{
				occurence[x].occur = 0;
				occurence[x].acid =(char)(x+65);
			}
			storecodon(j,sequence,protein);	 	            	
   		    output(i,j+1,protein,outfile,"reverse",annotation);
   		    printhistogram(outfile,protein);
   		}
   	}
	fclose(f);
	fclose(outfile);
	return 0;
}

void input(char *annotation,char *sequence,FILE *f)
{     // printf("i");
	fgets(annotation,500,f);
	fgets(sequence,7000,f);
	//printf("%s\n",sequence);
        int count1=0,count2=0,i, count3=0;
        len=(int)strlen(sequence)/3;
 	deletechar(sequence);
           
           
}  

void deletechar(char *sequence)
{
	int i=0,k=0;
 	for(i=0;i<strlen(sequence);i++)
 	{
        if((isupper(sequence[i])))
        { 
		  	sequence[k++]=sequence[i];
		} 
	    else if(islower(sequence[i]))
	    {
    	    sequence[k++]=(char)((int)sequence[i]+(int)'A'-(int)'a');
	    }
	}
	sequence[k]='\0';
}

void transcription(char *sequence)
{  
    int i;
    for(i=0;i<strlen(sequence);i++)
    {
		if(sequence[i]=='T') sequence[i]='U';
    }
}

void output(int i,int n,char *protein,FILE *outfile,char *order,char *annotation)
{              
	if(spacecheck!=i)
    { 
	 	fprintf(outfile,"\n\n\n\n");
		fprintf(outfile,"%s\n\n",annotation);
    }
    fprintf(outfile,"Translated protein for %s sequence in reading frame %d is: %s",order,n,protein);
	fprintf(outfile," \t\tlength is %d\n",(int)strlen(protein));
    if(opensequence==1) fprintf(outfile,"\tOpen sequence\n");	
	opensequence=0;
    spacecheck=i;
}

void sequencereversal(char *sequence)
{
    char rev;
    for(int i = 0; i < (strlen(sequence) + 1) / 2; i++)
    {
        rev = sequence[i];
        sequence[i] = sequence[strlen(sequence) - 1 - i];
        sequence[strlen(sequence) - 1 - i] = rev;
    }
}

void complimentarysequence(char *sequence)
{
    sequencereversal(sequence);
    for(int i = 0; i < strlen(sequence); i++)
    {
            if(sequence[i] == 'A') sequence[i] = 'U';
            else if(sequence[i] == 'G') sequence[i] = 'C';
            else if(sequence[i] == 'U') sequence[i] = 'A';
            else if(sequence[i] == 'C') sequence[i] = 'G';
    }  
}

int count_and_multitounifasta(char *file)
{
	char temp1[7000], temp2[1000];
	int i=0, count=0;

	FILE *f=fopen(file,"r");
	if(f==NULL)
	{
	 	printf("\nplease enter a valid input file\n");
	 	exit(3);
	}
	while(1)
 	{
		char ch=fgetc(f);

	  	if(ch==EOF)
		break;

		if(ch=='>')
		count++;
	}

	fclose(f);
	f=fopen(file,"r");	

	CODETYPE code[count];

	fgets(temp1,150,f); printf("%c",temp1[2]);

	if(temp1[0]=='>')

	strcpy(code[i].annotation,temp1);	

 	for(i=0;i<count;i++)   
	{
		temp1[0]='\0';
	    fgets(temp1,7000,f);
	    temp1[strlen(temp1)-1]='\0';	
		while(1)
		{ 
	 		fgets(temp2,5000,f);
	 		if(feof(f)) break;
	 		if(temp2[0]=='>')
			{ 
				strcpy(code[i+1].annotation,temp2); 
				break;
			}
			else
			{
				temp2[strlen(temp2)-1]='\0';
				strcat(temp1,temp2);
			}
			
		}
		temp2[0]='\0';
		code[i+1].sequence[strlen(code[i+1].sequence)]='\r';
		strcpy(code[i].sequence,temp1);
	}
	fclose(f);
	f = fopen("output.txt","w+");
	for(i=0; i< count;i++)
	{
	    if(i!=0) fprintf(f,"\n");
		fprintf(f,"%s",code[i].annotation);
		fprintf(f,"%s",code[i].sequence);	
	}
	fclose(f);
	return count;
}

int cmpfunc(const void * a, const void * b)
{
	
	return(((NUM_OCCUR*)b)->occur - ((NUM_OCCUR*)a)->occur);
}

void printhistogram(FILE *outfile, char *protein)
{
		qsort(occurence,26,sizeof(NUM_OCCUR),cmpfunc);
		for(int i =0; i < 20; i++)
		occurence[i].occur = ((float)(occurence[i].occur /(float)strlen(protein) ) * 200);

		int i = 0, j = 0;
		fprintf(outfile, "HISTOGRAM of number of occurences of every amino acid in descending order:\n\n");
		for(; i < 20; i++)
		{
			fprintf(outfile,"Amino Acid %c\t",occurence[i].acid);
			char hist[occurence[i].occur + 1];
			for(j=0; j < occurence[i].occur; j++)
			hist[j] = '*';
			hist[j] = '\0';
			fprintf(outfile,"%s\n", hist);	
		}
		fprintf(outfile,"\n\n");
}   

void storecodon(int n,char *sequence,char *protein)
{  
	int i=0,k=0,j=0; codontype codon;
	for(i=n;i<4*((int)strlen(sequence)-n)/3;i++)
	{ 
		char c[4]; 
		for(k=n;k<n+3;k++) codon.code[k-n]=sequence[k];
 		codon.code[3]='\0';
        n=k;
    	if(!strcmp(codon.code,"AUG")) 
		{
			protein[j++] = 'M';
			   opensequence=1;
			occurence[12].occur++;
		}
		else if(!strcmp(codon.code,"UAA") || !strcmp(codon.code,"UAG") || !strcmp(codon.code,"UGA"))
		{
			protein[j++] = '#'; 
		}
		else if(!strcmp(codon.code,"UUU")||!strcmp( codon.code,"UUC")) 
		{
			protein[j++] = 'F';
			
			occurence[5].occur++;
		}
		else if(!strcmp(codon.code,"UUA") || !strcmp(codon.code,"UUG") || !strcmp(codon.code,"CUU") || !strcmp(codon.code,"CUC") || !strcmp(codon.code,"CUA") || !strcmp(codon.code,"CUG")) 
		{
			protein[j] = 'L';
			j++;
			occurence[11].occur++;
		}
		else if(!strcmp(codon.code,"AUU") || !strcmp(codon.code,"AUC") || !strcmp(codon.code,"AUA")) 
		{
			protein[j] = 'I';
			j++;
			occurence[8].occur++;
		}
		else if(!strcmp(codon.code,"GUU") || !strcmp(codon.code,"GUC") || !strcmp(codon.code,"GUA") || !strcmp(codon.code,"GUG")) 
		{
			protein[j] = 'V';
			j++;
			occurence[21].occur++;
		}
		else if(!strcmp(codon.code,"UCU") ||!strcmp(codon.code ,"UCC")||!strcmp(codon.code,"UCA")|| !strcmp(codon.code,"UCG")) 
		{
			protein[j] = 'S';
			j++;
			occurence[18].occur++;
		}
		else if(!strcmp(codon.code,"CCU") || !strcmp(codon.code,"CCC") || !strcmp(codon.code,"CCA") || !strcmp(codon.code,"CCG")) 
		{
			protein[j] = 'P';
			j++;
			occurence[15].occur++;
		}
		else if(!strcmp(codon.code,"ACU") || !strcmp(codon.code,"ACC") ||!strcmp(codon.code ,"ACA") || !strcmp(codon.code,"ACG")) 
		{
			protein[j] = 'T';
			j++;
			occurence[19].occur++;					
		}
		else if(!strcmp(codon.code,"GCU") || !strcmp(codon.code,"GCC") || !strcmp(codon.code,"GCA") || !strcmp(codon.code,"GCG")) 
		{
			protein[j] = 'A';
			j++;
			occurence[0].occur++;					
		}
		else if(!strcmp(codon.code,"UAU") || !strcmp(codon.code,"UAC")) 
		{
			protein[j] = 'Y';
			j++;
			occurence[24].occur++;					
		}
		else if(!strcmp(codon.code,"CAU") || !strcmp(codon.code,"CAC")) 
		{
			protein[j] = 'H';
			j++;
			occurence[7].occur++;					
		}
		else if(!strcmp(codon.code, "CAA") || !strcmp(codon.code,"CAG")) 
		{
			protein[j] = 'Q';
			j++;
			occurence[16].occur++;					
		}
		else if(!strcmp(codon.code,"AAU") || !strcmp(codon.code,"AAC")) 
		{
			protein[j] = 'N';
			j++;
			occurence[13].occur++;					
		}
		else if(!strcmp(codon.code,"AAA")|| !strcmp(codon.code,"AAG")) 
		{
			protein[j] = 'K';
			j++;
			occurence[10].occur++;					
		}
		else if(!strcmp(codon.code,"GAU") || !strcmp(codon.code,"GAC")) 
		{
			protein[j] = 'D';
			j++;
			occurence[3].occur++;					
		}
		else if(!strcmp(codon.code,"GAA") || !strcmp(codon.code,"GAG")) 
		{
			protein[j] = 'E';
			j++;
			occurence[4].occur++;					
		}
		else if(!strcmp(codon.code,"UGU") || !strcmp(codon.code,"UGC")) 
		{
			protein[j] = 'C';
			j++;
			occurence[2].occur++;
		}
		else if(!strcmp(codon.code,"UGG")) 
		{
			protein[j] = 'W';
			j++;
			occurence[22].occur++;
		}
		else if(!strcmp(codon.code,"CGU") || !strcmp(codon.code,"CGC") || !strcmp(codon.code,"CGA") || !strcmp(codon.code,"CGG") || !strcmp(codon.code,"AGA") || !strcmp(codon.code,"AGG")) 
		{
			protein[j] = 'R';
			j++;
			occurence[17].occur++;					
		}
		else if(!strcmp(codon.code,"AGU") || !strcmp(codon.code,"AGC")) 
		{
			protein[j] = 'S';
			j++;
			occurence[18].occur++;					
		}
		else if(!strcmp(codon.code,"GGU") || !strcmp(codon.code,"GGC") ||!strcmp( codon.code,"GGA") || !strcmp(codon.code,"GGG")) 
		{
			protein[j] = 'G';
			j++;
			occurence[6].occur++;					
		}
		else
		{ protein[j++]='X';}
	}
}   