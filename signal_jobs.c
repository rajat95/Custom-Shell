/*Rajat Arora 2013A7PS104P*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

/*struct for background processes*/
struct background
{
	pid_t id;
	int bit;
	char *name;
	struct background *next;

};

typedef struct background background;

/*utility function to print background jobs*/
void print_jobs(struct background *head)
{
	head = head->next;
	
	while(head!=NULL)
	{
		if(head->bit == 1)
			printf("%d running %s \n",head->id,head->name);
		else
			printf("%d stopped %s \n",head->id,head->name);
		head = head->next;
	}
}

/*function to stop a job according to pid*/
void stop_job(struct background *head, pid_t pid)
{
	int output;
	output = kill(pid,SIGTSTP);
	if(output == -1)
		{
			printf("no process with given pid\n");
		}
	head = head->next;
	while(head!=NULL)
	{
		if(head->id == pid)
		{
			head->bit = 0;
			break;	
		}
		head = head->next;
		
	}
}


/*function to start a stopped job*/
void start_job(struct background *head, pid_t pid)
{
	kill(pid,SIGCONT);
	head = head->next;
	while(head!=NULL)
	{
		if(head->id == pid)
		{
			head->bit = 1;
			break;	
		}
		head = head->next;
		
	}
}


/*function to delete terminated functions*/
delete_process(struct background *head, pid_t pid,struct background **node)
{
	struct background* prev = (struct background*)malloc(sizeof(struct background));
	prev = head;
	head = head->next;
	while(head!=NULL)
	{
		if(head->id == pid)
		{
			prev->next=head->next;
			if(head->next == NULL)
			{
				*node = prev;
			}
			head->next= NULL;
			free(head);
			break;
		}
		prev = head;
		head = head->next;
	}
}

int main(void)
{
	int status;
	char *input;
	char *job;
	char *init;
	int background[1000];
	int count = 0;
	pid_t w,pid,fore;
	struct background *head;
	struct background *node;
	head = (struct background*)malloc(sizeof(struct background));
	node = head;
	
	/*block signals*/
	signal(SIGQUIT,SIG_IGN);
	signal(SIGINT,SIG_IGN);
	signal(SIGTSTP,SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	while(1)
	{
		/*initialisiing foreground process group*/
		tcsetpgrp(0,getpid());
		int flag = 0;
		input = (char*)malloc(100*sizeof(char));
		printf("command<<");
		fgets(input,100,stdin);
		init = input;
		
		/*if user presses newline continue*/
		if(strcmp(input,"\n") == 0)
		{
			continue;
		}
		
		if(input[strlen(input)-1] == '\n')
			input[strlen(input)-1] = '\0';
		/*printf("%s",input);*/
		
		if(input[strlen(input)-1] == '&'){
			input[strlen(input)-1] = '\0';
			/*flag indicates if process is background*/
			flag = 1;
		}
		
		if (strcmp(input,"jobs") == 0)
		{
			int stat;
			pid_t changed;
			while(1)
			{
				/*print_jobs(head);*/
				/*check for terminated background jobs*/
				changed = waitpid(-1,&stat,WNOHANG);
				if(changed == 0||changed == -1)
					break;
				/*printf("%d\n",changed);*/
				delete_process(head,changed,&node);
			}
			print_jobs(head);
			continue;
		}
		if(strcmp(input,"quit") == 0)
		{
			break;
		}
		
		char *args[20];
		int k;
		for(k = 0;k<20;k++)
		{
			args[k] = NULL;
		}
		char *token;
		char s[] =" ";
		token = strtok(input,s);
		int i = 0,j;
		
		/*tokenising the command*/
		if(token == NULL)
		{
			
			job = input;
		}
		else
		{
			job = token;
			if(strcmp(job,"stop") == 0)
			{
				token = strtok(NULL,s);
				/*printf("%s",token);*/
				pid_t proc = atoi(token);
				stop_job(head,proc);
				continue;
			}
			if(strcmp(job,"start") == 0)
			{
				token = strtok(NULL,s);
				pid_t proc = atoi(token);
				start_job(head,proc);
				continue;
				
			}
			args[i] = token;
			token = strtok(NULL,s);
			
			while(token!=NULL)
			{
				i++;
				args[i] = token;
				token = strtok(NULL,s);
					
			}
		}

		{	pid = fork();
			if(!flag)
			fore = pid;
			if(pid == 0)
			{
				
				
				if(flag)
				{
					signal(SIGCHLD,SIG_IGN);
					signal(SIGTTOU, SIG_IGN);
					signal(SIGTTIN, SIG_IGN);
					setpgid(0,0);
				}
				else{
					signal(SIGTSTP,SIG_DFL);
					signal(SIGTTOU, SIG_IGN);
					setpgid(getpid(),getpid());
					tcsetpgrp(0,getpgid(getpid()));
				}
				if(i ==0){
					execlp(input,input,(char *)0);
					perror ("error in execlp\n");
				}
				else{
					/*printf("%s %s\n",job,args[1]);*/
					execvp(job,args);
					perror ("error in execvp\n");
					}
			}
			else
			{
				
				if(flag == 1)
				{
					/*store process initiated in a linked list*/
					int stat;
					node->next = (struct background*)malloc(sizeof(struct background));
					node = node->next;
					node->bit = 1;
					node->id = pid;
					node->name = (char*)malloc(100*sizeof(char));
					node->name = input;
					
					
					/*while(waitpid(-1,&stat,WNOHANG|WUNTRACED));*/
				}
				else
				{
					/*if process is foreground, wait*/
					if(pid == fore){
					do
					{
					
						w = waitpid(-1,&status,WUNTRACED);
						if(WIFSTOPPED(status))
						{
							kill(pid,SIGSTOP);
						}
					}while(!WIFEXITED(status)&&!WIFSIGNALED(status)&&!WIFSTOPPED(status));
					/*setting shell back to foreground*/
					tcsetpgrp(0,getpgid(0));}
				}
				
			}
			
		
		}
	}
	
	
}
