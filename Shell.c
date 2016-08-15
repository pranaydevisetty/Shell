#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/wait.h>
#include<signal.h>
#include<fcntl.h>
#define MAX_SUB_COMMANDS 5
#define MAX_ARGS 10
struct SubCommand //The subcommand structure
{
	char *line; //line pointer to store the subcommand
	char *argv[MAX_ARGS]; // to store each argument that are with spaces
};
struct Command
{
	struct SubCommand sub_commands[MAX_SUB_COMMANDS];//sub command array to store the sub commands seperated by pipe
	int num_sub_commands;//counter for number of sub-commands
	char *stdin_redirect;//to store the stdin redirection 
	char *stdout_redirect;//to store stdout redirection
	int background;//to tell whether there is a background process "or" not
};

void ReadCommand(char *line, struct Command *command);//function for reading command
void PrintCommand(struct Command *command);// function to print the commanfd
void read_args(char *in, char *argv[MAX_ARGS], int size);//function to read the args
void read_args2(char *in, char *argv[MAX_ARGS], int size);//function to read the last subcommand
void print_args(char *argv[MAX_ARGS]);// function to print the argumentss of the subcommand
void ReadRedirectsAndBackground(struct Command *command);// function to populate stdin_redirect,stdout_redirect and background fields
void handler(int sig);
int main()//The main program
{
		char s[200],ba[10];// input array to store the given command
			//int cc1=0;
		//to print prompt character initially
		while(1)//to get the command infinitely many times as possible
		{
		char dir[200];
		req:getcwd(dir,200);
			printf("%s$ ",dir);
			fgets(s,200,stdin);
			s[strlen(s)-1]='\0';
			//making the last character null instead of /n
			if(s[0]!='\0')
			{
			struct Command cmd;//declaration of struct variable
			ReadCommand(s,&cmd);//passing the command entered to seperate the command into subcommans that are seperated by pipes{|)
			ReadRedirectsAndBackground(&cmd);//passing the command to filter the redirection operators
			//PrintCommand(&cmd);// finally printing the command with different args and subcommands.
			if(cmd.stdin_redirect!=NULL)
			{
				//printf("the fault5");
				cmd.stdin_redirect=strtok(cmd.stdin_redirect," ");
			}
			if(cmd.stdout_redirect!=NULL)
			{
				//printf("the fault6");
				cmd.stdout_redirect=strtok(cmd.stdout_redirect," ");
			}
			int cdcheck,pacheck;
			cdcheck=strcmp(cmd.sub_commands[0].line,"cd");
			if(cdcheck==0)
			{
				char *path=cmd.sub_commands[0].argv[1];
				if(path!=NULL)
				{
					pacheck=strcmp(path,"~");
				}
				if(path==NULL)
				{
					if(cmd.background==1)
					{
						printf("[%d]\n",(int) getpid());
						goto req;
					}
					else
					{
						//printf("Err:expected argument to'cd'\n");
						//goto req;
						chdir(getenv("HOME"));
					}
				}
				else if(pacheck==0)
				{
					chdir(getenv("HOME"));
					goto req;
				}
				else if(chdir(path)<0)
				{
					printf("cd: %s: No such file or directory\n",path);
					goto req;
				}
			}
			else
			{
			int i,a1,b1;
			int fdexp;
			int fds[2],sdf=0;
			int cfd=0,bfd=0;
			int ifc=0;
			int np=0;
			int i1;
			int ret=1;
			int check;
			for(i=0;i<(cmd).num_sub_commands&&ret!=0;i++)
			{
				if(i!=(cmd).num_sub_commands-1)
				{
					pipe(fds);
					np++;
				}
				ret=fork();
			}
		if (ret < 0)
		{
			perror("fork");
		}//end of if ret<0
		else if (ret == 0)
		{
			//printf("i:%d\n",i);
			i=i-1;
			if(i==0)
			{
				// Close read end of pipe
				if(((cmd).num_sub_commands)>1)
				{
				close(3);
				//close the standard output 
				close(1);
				if(dup(4)<0)
				perror("duplicate");
				//dup2(sdf,1);
				// Duplicate write end of pipe in standard input
				if(cmd.stdin_redirect!=NULL)
					{
						close(0);
						// Redirect output to "list.txt"
						int fd = open((cmd).stdin_redirect, O_RDONLY);
						if (fd < 0)
						{
							fprintf(stderr, "%s:invalidfile not found\n",(cmd).stdin_redirect);
							exit(1);
						}
					}
				}
				else 
				{
					if(cmd.stdin_redirect!=NULL)
					{
						close(0);
						// Redirect output to "list.txt"
						int fd = open((cmd).stdin_redirect, O_RDONLY);
						if (fd < 0)
						{
							fprintf(stderr, "%s:invalidfile not found\n",(cmd).stdin_redirect);
							exit(1);
						}
					}
					if(cmd.stdout_redirect!=NULL)
					{
						close(1);
					// Redirect output to "list.txt"
						int fd = open((cmd).stdout_redirect, O_WRONLY | O_CREAT, 0660);
						if (fd < 0)
						{
							fprintf(stderr, "%s:cannot create file\n",(cmd).stdout_redirect);
							exit(1);
						}
					}
				}
				//if(cmd.background==1)
				//{
				//	printf("[%d]\n",(int) getpid());
				//	break;
				//}
				if((execvp(cmd.sub_commands[i].argv[0], cmd.sub_commands[i].argv)==-1))
				{
					fprintf(stderr,"%s:command not found\n",(cmd).sub_commands[i].line);
					exit(0);
				}
			}
			else if((i<((cmd).num_sub_commands)-1))
			{	
				int fdc;
				close(0);
				close(1);
				for(fdc=3;fdc<=((((i+1)*2))+2);fdc++)
				{
					if((fdc!=((i+1)*2)-1)&&fdc!=((((i+1)*2))+2))
					{
						close(fdc);
					}
					if(fdc==(((i+1)*2))+2)
					{	
						if(dup(fdc)<0)
						printf("filedisc>3");
					}
					if(fdc==(((i+1)*2))-1)
					{
						if(dup(fdc)<0)
						printf("filedisc>3");
					}
				}
				if((execvp(cmd.sub_commands[i].argv[0], cmd.sub_commands[i].argv)==-1))
				{
					fprintf(stderr,"%s:command not found\n",(cmd).sub_commands[i].line);
					exit(0);
				}
			}
			else if((i==(cmd).num_sub_commands-1))
			{
				int fdc1;
				if(cmd.num_sub_commands==2)
				{
					close(4);
					close(0);
					dup(3);
					if(cmd.stdout_redirect!=NULL)
					{
						// Redirect output to "list.txt"
						close(1);
						int fd = open((cmd).stdout_redirect, O_WRONLY | O_CREAT, 0660);
						if (fd < 0)
						{
							fprintf(stderr, "%s:cannot create file\n",(cmd).stdout_redirect);
							exit(1);
						}
					}
				}
				else
				{
					for(fdc1=3;fdc1<=(((i+1)*2));fdc1++)
					{
						if((fdc1!=((i+1)*2)-1))
						{
							close(fdc1);
						}
						else
						{
							close(0);
							dup(fdc1);	
						}	
					}
				}
				if(cmd.stdout_redirect!=NULL)
				{
						// Redirect output to "list.txt"
					close(1);
					int fd = open((cmd).stdout_redirect, O_WRONLY | O_CREAT, 0660);
					if (fd < 0)
					{
						fprintf(stderr, "%s:cannot create file\n",(cmd).stdout_redirect);
						exit(1);
					}
				}
				if(cmd.stdin_redirect!=NULL)
				{
						// Redirect output to "list.txt"
					close(0);
					int fd = open((cmd).stdin_redirect,O_RDONLY);
					if (fd < 0)
					{
						fprintf(stderr, "%s:cannot create file\n",(cmd).stdin_redirect);
						exit(1);
					}
				}
			//	if(cmd.background==1)
			//	{
			//		printf("[%d]\n",(int)getpid());
			//		break;
			//	}
				if((execvp(cmd.sub_commands[i].argv[0], cmd.sub_commands[i].argv)==-1))
				{
				fprintf(stderr,"%s:command not found\n",(cmd).sub_commands[i].line);
				exit(0);
				}
			}
		}//end of elsif ret==0
		else
		{
				int ip1;
			for(i=3;i<=((2*np)+(2));i++)
			{
				close(i);
				
			}	
			//printf("ip1:%d",ip1);
			for(ip1=0;ip1<cmd.num_sub_commands;ip1++)
			{
				int *status;
				if(cmd.background==1)
				{
					printf("[%d]\n",ret);
					//printf("$ ");
				//bg[ret];
				setpgid(0,0);
				signal(SIGINT,handler);
				printf("%s$ ",dir);
				fgets(ba,10,stdin);
				int chec11=strcmp(ba,"\n");
				if(chec11==0)
				{
					printf("[%d] finished\n",ret);
				}
				break;
				}
				int tempw=waitpid(ret,status,WUNTRACED);
			}
			
		}//end of parent for the child bloc
		}
	}
	}//end of while loop
}//end of main
void handler(int sig)
{
	int ret;
	int status;
	int tw=wait(NULL);
	printf("[%d] finished\n",ret);
}
void ReadCommand(char *line, struct Command *command)
{ 
	const char *delim ="|";
	const char *k;
	char *copy;
	char *l;
	int i=0;
	(command)->num_sub_commands=0;
	//printf("the fault1");
	k=strtok(line,delim);
	while((i<MAX_SUB_COMMANDS)&& (k!=NULL))
	{
		l=strdup(k);
		(command)->sub_commands[i].line=l;
		//printf("(command)->sub_commands[%d].line=%s,l=%s\n",i,(command)->sub_commands[i].line,l);
		i++;
		//printf("%d iteration\n",i);
		(command)->num_sub_commands++;
		//printf("%d number of subcommands\n",(*command)->num_sub_commands);
		//printf("the fault2");
		k=strtok(NULL,delim);
	}
	//(command)->sub_commands[i-1].line=l;
	//printf("(*command)->num_sub_commands=%d\n",(command)->num_sub_commands);
	for(i=0;i<(command)->num_sub_commands;i++)
	{
		copy=strdup((command)->sub_commands[i].line);
		//printf("copy=%s",copy);
		//printf("(*command)->sub_commands[%d].line=%s,l=%s\n",i,(*command)->sub_commands[i].line,l);
		read_args(copy,(command)->sub_commands[i].argv,MAX_ARGS);
	}
}
void read_args(char *in, char *argv[MAX_ARGS], int size)
{
		const char *del =" ";
	int i=0;
		const char *k1;
		char *l1;
		//printf("the fault7");
		k1=strtok(in,del);
			while((i<size-1)&&(k1!=NULL))
			{
				l1=strdup(k1);
				argv[i]=l1;
				//printf("read_argsargv[%d]=%s\n",i,argv[i]);
				i++;
				//printf("k1=%s\n",k1);
				//printf("l1=%s\n",l1);
				//printf("the fault3");
				k1=strtok(NULL,del);
			}
	//printf("i2=%d\n",i);
	argv[i]=NULL;
}
void ReadRedirectsAndBackground(struct Command *command)
{
	if(((command)->num_sub_commands)!=0)
	{
	int i;int kl;char l;
	char *t=strdup((command)->sub_commands[((command)->num_sub_commands)-1].line);
	command->background=0;
	command->stdin_redirect=NULL;
	command->stdout_redirect=NULL;
	char go1[100]; 
	char go2[100];
	char go3[100];
	kl=strlen(t);
	int co1;
	int co2;
	int co3;
	int co4;
	int co5=(command->num_sub_commands);
	int j;
	//printf("string length of t:%d\n",kl);
for(j=strlen(t)-1;j>=0;j--)
{
	if(t[j]=='&')
	{
	t[j]='\0';
	command->background=1;
	}
	 if(t[j]=='<')
	{
	
		for(co3=j+1,i=0;t[co3]!='\0';co3++,i++)
		{
			go3[i]= t[co3];
	
		}
		go3[i]='\0';
	t[j]='\0';
	command->stdin_redirect=strdup(go3);	
	}
	 if(t[j]=='>')
	{
	
		for(co4=j+1,i=0;t[co4]!='\0';co4++,i++)
		{
			go2[i]= t[co4];

		}			
	go2[i]='\0';	
	t[j]='\0';
	command->stdout_redirect=strdup(go2);
	}
}
command->sub_commands[co5-1].line = t;
read_args(command->sub_commands[co5-1].line, command->sub_commands[co5-1].argv, MAX_ARGS);
}
}
void read_args2(char *in, char *argv[MAX_ARGS], int size)
{
		const char *del1 =" ";
		int i=0,j,p1,q1,r1,q2,r2,p2,len1;
		const char *k2;
		char *l1,*l2,*l3,temp;
		char *temp3;
		char *sl1;
		//k2=strtok(in,"");
		//printf("the fault 8");
		k2=strtok(in,del1);
			while((i<size-1)&&(k2!=NULL))//&&(argv[i]!=l2)
			{
				l1=strdup(k2);
				len1=strlen(l1);
				//printf("hi");
				l3=strdup(k2);
				l2=l1;
				char temp1;
				temp1=l2[0];
				//printf("temp1=%c\n",temp1);
				for(j=0;j<len1;j++)
				{
					temp=l3[j];
					//printf("temp=%c,%d\n",temp,j);
					//printf("this shouldnotcome\n");
				if(temp!='>' || temp!='<'||temp!='&')
				{
						//printf("this shouldnotcome");
						argv[i]=strtok_r((l3+(j)),"><&",&sl1);
						//argv[i]=strdup(temp3);
					break;
				}
					}	
				//l2=strtok(l3,"<>&");
				argv[i]=l3;
				//printf("2.read_args2argv[%d]=%s,l3=%s,l2=%s\n",i,argv[i],l3,l2);
				if(temp1=='>'||temp1=='<'||temp1=='&')
				{
					break;
				}
				if(temp=='>' || temp=='<'||temp=='&')
				{
					i++;
					//printf("this shouldnotcome");
					break;
				}
				//printf("read_args2argv[%d]=%s\n",i,argv[i]);
				//printf("k2=%s\n",k2);
				//printf("l1=%s\n",l1);
				//printf("the fault9");
				k2=strtok(NULL,del1);
				//printf("k2=%s\n",k2);
				i++;
				//printf("i2=%d\n",i);
				//k2=strtok(NULL,"");
			}
	//printf("i2=%d\n",i);
	argv[i]=NULL;
}
		
void PrintCommand(struct Command *command)
{
	int i,a1,b1;
	for(i=0;i<(command)->num_sub_commands;i++)
	{
		printf("subcommand[%d]='%s' \n",i,(command)->sub_commands[i].line);
		print_args((command)->sub_commands[i].argv);
	}
	//a1=strcmp(NULL,((command)->stdout_redirect));
	//printf("a1:%d\n",a1);
	//b1=strcmp(NULL,((command)->stdin_redirect));
	//if(((command)->stdout_redirect)!=NULL)
	//{
		//printf("the fault4");
		//(command)->stdout_redirect=strtok((command)->stdout_redirect," ");
		//printf("Redirect stdout:%s\n",(command)->stdout_redirect);
		//printf("hi stdout");
	//}
	//else
	//{
		//printf("Redirect stdout: %s\n",(command)->stdout_redirect);
	//}
	//if(((command)->stdin_redirect)!=NULL)
	//{
		//printf("Redirect stdin:%s\n",(command)->stdin_redirect);
		//printf("hi stdout");
	//}
	//else
	//{
		//printf("Redirect stdin: %s\n",(command)->stdin_redirect);
	//}
	
	if((command)->background==1)
	{
		printf("background:yes\n");
	}
	else
	{
		printf("background:no\n");
	}
}
void print_args(char **argv)
{
	int i;
	for(i=0;argv[i]!=NULL;i++)
	{
		printf("argv[%d]='%s' \n",i,argv[i]);
	}
}
