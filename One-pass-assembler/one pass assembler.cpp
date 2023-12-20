#include<iostream>
#include<fstream>
#include<cstring>
#include<stdlib.h>
#define M 50

int SIZ=0;
int x=1;
char BUF[70]={'\0'};
using namespace std;

typedef struct symbol
{
	char label1[10],loc1[10];
	int ind;
	char flag;
	struct symbol *next;
}sym;

sym *head[M];



sym *datasymtab(sym *head[], char label[],int locctr)
{
	char loc[10];
	
	sprintf(loc,"%X",locctr);
	sym *p;
	
	for(int i=0;i<M;i++)
	{
		p=*(head+i);
		if(p->ind==0)
		{
			strcpy(p->label1,label);
			p->ind=1;
			strcpy(p->loc1,loc);
			break;
		}
	}
	
	return *head;
}


int checklabel(char label[])
{
	sym *p;
	
	int k=0;
	
	for(int i=0;i<M;i++)
	{
		p=*(head+i);
		
		if(strcmp(p->label1,label)==0)
		{
			k=1;										//returns 1 if label is in symtab
		}	
		
	}
	
	if(k==1)
	return k;
	else 
	return 0;
}

int checkoperand(char operand[])
{
		sym *p;
		int k=0;
		
	for(int i=0;i<M;i++)
	{
		p=*(head+i);
		
		if(strcmp(p->label1,operand)==0)
		{
			k=1;
		}
	}
	
	if(k==1)
	return 1;
	
	else 
	return 0;
}

sym *addsymtab(sym *head[], char label[], char operand[], int locctr,char opcode[], char mne[2], ofstream &fout4, ofstream &fout3)
{
	int location=locctr,ref=0;
	char objcode[10]={'\0'},opcode2[10];
	char lo[10],si[2];
	sprintf(lo,"%X",locctr);
	strcpy(opcode2,opcode);	
	sym *p,*q,*r;
	int modify,k;
	char loc[10];
	

	
	if(strcmp(label,"**")!=0)
	{
		
	
	k=checklabel(label);
	
	if(k==0)				//label not found
	{
		for(int i=0;i<M;i++)
		{
			p=*(head+i);
			
			if(p->ind==0)
			{
				strcpy(p->label1,label);
				p->ind=1;
				sprintf(loc,"%X",locctr);				//int to hexa string conversion
				strcpy(p->loc1,loc);
				break;
			}
		}
	}
	
	else if(k==1)				//label found
	{
		for(int i=0;i<M;i++)
		{
			p=*(head+i);
			
			if(strcmp(p->label1,label)==0)
			{
				if(p->flag=='*')
				{
					p->flag='0';
					sprintf(loc,"%X",locctr);
					strcpy(p->loc1,loc);
					r=p;	
					ref=1;
					break;
														// modfication needed traversal
				}
			}
		}
	}
	
	}
	k=checkoperand(operand);
	
	if(k==0)				//operand not found
	{
		for(int i=0;i<M;i++)
		{
			p=*(head+i);
			
			if(p->ind==0)
			{
				strcpy(p->label1,operand);
				p->ind=1;
				p->flag='*';
				
				p->next=new sym;
				p=p->next;
				p->next=NULL;
				
				
				sprintf(loc,"%X",locctr+1);				//add needed to be modify
				strcpy(p->loc1,loc);
				strcpy(objcode,mne);
				strcat(objcode,"0000");					//making 0 for undefined address
				break;	
			}
		}
	}
	
	else if(k==1)				//operand found
	{
		for(int i=0;i<M;i++)
		{
			q=*(head+i);
			
					if(strcmp(q->label1,operand)==0  && q->flag=='*')
					{
						while(q->next!=NULL)
						{
							q=q->next;											
						}
					
						q->next=new sym;
						q=q->next;
						q->next=NULL;
					
						modify=locctr+1;
			
						sprintf(loc,"%X",modify);
			
						strcpy(q->loc1,loc);
						
						strcpy(objcode,mne);
						strcat(objcode,"0000");					//making 0 for undefined address
						
						break;
					}
					
					else if(strcmp(q->label1,operand)==0 && q->flag=='0')
					{
				
						strcpy(objcode,mne);
						strcat(objcode,q->loc1);
						
						
						for(int i=0;i<10;i++)								//handling indexed addressing
						{
							if(operand[i]==',')
							{
								if(objcode[2]==char('0'))
								{
									objcode[2]=char('8');
								}
				
								else if(objcode[2]==char('1'))
								{
									objcode[2]=char('9');
								}
				
								else if(objcode[2]==char('2'))
								{
									objcode[2]=char('A');
								}
				
								else if(objcode[2]==char('0'))
								{
									objcode[2]=char('B');
								}
							}
						}
												
					}
			
		}
		
	}
	
	
	if(SIZ<=57)
	{
		if(strcmp(objcode,"******")!=0 && strcmp(opcode2,"END")!=0 )
		{
			if(x==1)								//needed to run for 1 time only
			SIZ=0;
			
						
			SIZ=SIZ+strlen(objcode);
			strcat(BUF,"^");
			strcat(BUF,objcode);
						
			x--;
		}
				
	}
			
	if(strcmp(objcode,"******")==0 || SIZ>57 || strcmp(opcode2,"END")==0 || ref==1)
	{
		SIZ=SIZ/2;
		if(SIZ!=0)
		{
			sprintf(si,"%X",SIZ);
			location=location-SIZ;
			sprintf(loc,"%X",location);
						
			fout3<<'T'<<'^'<<"00"<<loc<<'^'<<si<<BUF<<endl;

		}
		strcpy(BUF,"^");				
		SIZ=0;
	}
	
	if(ref==1)
	{		
		sym *s=r;
		s=s->next;
		while(s!=NULL)
		{
			fout3<<'T'<<'^'<<"00"<<s->loc1<<'^'<<"02"<<'^'<<r->loc1<<endl;
			s=s->next;
		}
		ref=0;
		SIZ=0;
	}

	fout4<<lo<<"\t"<<label<<"\t"<<opcode<<"\t"<<operand<<"\t"<<objcode<<endl;
	return *head;
}



int main()
{
	
	char opcode[10],label[10],operand[10],mne[10],code[10],name[10];
	char loc[10],objcode[6],locctr1[10],R[1]={'T'},I[1]={'^'},BUF[70]={"\0"},len[2],len1[6];	//loc is string 
	int locctr,location,counter,start,length,k,convert,i=1,size=1,m=1,obcount=0;
	
	for(int i=0;i<M;i++)
	{
		*(head+i)=new sym;				//symtab creation
		(*(head+i))->next=NULL;
		(*(head+i))->ind=0;
		(*(head+i))->flag='0';
	}
	
	
	
	cout<<"WELCOME"<<endl;
	ofstream fout1,fout2,fout3,fout4;
	ifstream fin1,fin2,fin3,fin4;		//fin1->source; fin2->optab; fin3->records; fin4->output
	
	fin1.open("input1.txt");
	
	fout3.open("objectcode1.txt");
	fout4.open("output1.txt");
	
	

	fin1>>label>>opcode>>operand;
	
	if(strcmp(opcode,"START")==0)
	{
		start=4096;
		locctr=start;
		location=start;
		strcpy(name,label);
		fout4<<"\t"<<label<<"\t"<<opcode<<"\t"<<operand<<"\t\t"<<endl;
		strcpy(len1,"00004B");
		fout3<<"H^"<<name<<"^"<<"00"<<operand<<"^"<<len1<<endl;
	
		fin1>>label>>opcode>>operand;
	}
	
	else
		locctr=0;
		
		sprintf(loc,"%X",locctr);
		strcpy(locctr1,loc);
		
		
		while(strcmp(opcode,"END")!=0)
		{
			counter=locctr;
			
			if(strcmp(opcode,"BYTE")==0 || strcmp(opcode,"WORD")==0 || strcmp(opcode,"RESB")==0 || strcmp(opcode,"RESW")==0)
			{
				if(strcmp(opcode,"WORD")==0)
					{
						locctr=locctr+3;
						strcpy(objcode,"00000");
						convert=atoi(operand);
						sprintf(loc,"%X",convert);
						strcat(objcode,loc);
						
					}
				
				else if(strcmp(opcode,"RESW")==0)
					{
						locctr=locctr+3*atoi(operand);
						
						strcpy(objcode,"******");
					}
					
				
				else if(strcmp(opcode,"RESB")==0)
					{
						locctr=locctr+atoi(operand);
						
						strcpy(objcode,"******");
					}
				
				else if(strcmp(opcode,"BYTE")==0)
					{
						locctr=locctr+strlen(operand)-3;	//no space for C,','  or X,','
							char ch[10];
							
							for(int i=0;i<10;i++)
							{
								objcode[i]={'\0'};
							}
							
							int a,b=0;
							if(operand[0]=='C')
							{
								for(int i=2;i<strlen(operand)-1;i++)
								{
									a=int(operand[i]);
									sprintf(loc,"%X",a);
									strcpy(ch,loc);
									strcat(objcode,ch);
								}
							}
							
							else if(operand[0]=='X')
							{
								for(int i=2;i<strlen(operand)-1;i++)
								{
									objcode[i-2]=operand[i];
								}
							}
						
					}
					  
				*head = datasymtab(head,label,counter);
				
				sprintf(loc,"%X",counter);
				fout4<<loc<<"\t"<<label<<"\t"<<opcode<<"\t"<<operand<<"\t"<<objcode<<endl;
				
				if(size<=60)
				{
					if(strcmp(objcode,"******")!=0)
					{
						if(m==1)								//needed to run for 1 time only
						size=0;
						
						
						size=size+strlen(objcode);
						strcat(BUF,"^");
						strcat(BUF,objcode);
						
						m--;
						
					}
				
				}
				
				
				if(strcmp(objcode,"******")==0 || size>60)
				{
					char si[2];
					size=size/2;
					if(size!=0)
					{
						sprintf(si,"%X",size);
						sprintf(loc,"%X",location);
						
						fout3<<'T'<<'^'<<"00"<<loc<<'^'<<si<<BUF<<endl;
					  	location=locctr;
					}
					
					size=0;
				}
				
				strcpy(objcode," ");
				
				counter =locctr;		
				
		  	}
			
			else 
			{
				fin2.open("optab1.txt");
				
				fin2>>code>>mne;
				while(code!="END")
				{
					if(strcmp(code,opcode)==0)
					break;
					else
					fin2>>code>>mne;	
				}
				
				sprintf(loc,"%X",locctr);
				
				*head = addsymtab(head,label,operand,locctr,opcode,mne,fout4,fout3);
				
				locctr=locctr+3;
				fin2.close();
			}
			
			fin1>>label>>opcode>>operand;
		}
		
		length=locctr-start;
		
		
		cout<<"SOURCE CODE IS SUCCESSFULLY CONVERTED TO OBJECT CODE USING ONE PASS ASSEMBLER"<<endl;
			sprintf(loc,"%X",locctr);
			
			*head = addsymtab(head,label,operand,locctr,opcode,mne,fout4,fout3);
			
				
			char ch[6];
			strcpy(ch,"00");
			sprintf(loc,"%X",start);
			strcat(ch,loc);
			
			fout3<<"E"<<'^'<<ch;
			
			fin1.close();
			fout3.close();
			fout4.close();
			
	sprintf(loc,"%X",length);
	
	cout<<"name of program is "<<name<<endl;
	cout<<"length of file is "<<loc<<endl;
	sym *p;
	
	cout<<"\nSYMTAB"<<endl;
	
	cout<<"s.no		"<<"label 		"<<"location  "<<"flag"<<"\t\t"<<"extended flags"<<endl;
	cout<<"__________________________________________________________________________"<<endl;
	
	for(int i=0;i<M;i++)
	{
		p=*(head+i);
		
		if(p->ind==1)
		{
			cout<<"details "<<i+1<<"\t";
			cout<<p->label1<<"\t\t";
			cout<<p->loc1<<"\t"<<ends;
			cout<<p->flag;
			
			while(p!=NULL)
			{
				cout<<" -> "<<p->loc1<<ends;
				p=p->next;
			}	
			cout<<endl;
		}
	}	
}
	

