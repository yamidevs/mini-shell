#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "processus.h"
#include "search.h"
#include "string.h"
#include "builtin.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <glob.h>
#include "string.h"
#include <stdio.h>
 

/*
Fichier processus.c : fichier permet de gérer des commandes comme export , cd ... 
Dépendances : search.h , builtin.h 
*/


/*
Fonction exec_process permet de verifier si la commade existe ou pas 
Paramètre cmd : processus actuel qui vas étre traiter
Retourne un int 
*/
int exec_process(processus_t* process) {

    char * path;
    can_access_processus(process->argv[0]);
    if((path = can_access_processus(process->argv[0])) || process->stderr != 2)
    {
    	if(path != NULL)
        	strcpy(process->path,path);
        else
        {
            if(process->stderr != 2)
	    	{
	    	
	    		dup2(process->stderr,2);
	    		close(process->stderr);
	    		
	    	}

        	fprintf(stderr,"commande  %s introuvable",process->argv[0]);
        }
        call_process(process);
    }else
    {
        exec_static_cmd(process);
    }     
}


 /*
Fonction call_process  crée un processus fils et appelle la commande , gestion de pipe , redirection de flux
Paramètre process : processus actuel
Retourne un int  
*/
int  call_process(processus_t* process){

    if((process->pid = fork()) == 0) { 

    		if(process->stdin != 0 && process->stdout != 1)
    		{	    		
    				dup2(process->stdin,0);
    				dup2(process->stdout,1);
    				close(process->fd[0]);
	        		close(process->fd[1]);

    		}else
    		{
    			if(process->stdin != 0)
	    		{	    	
					dup2(process->stdin,0);
	        		close(process->stdin);

	        		if(process->fd[1] != 1)
	        			close(process->fd[1]);
	    		}

	    		if(process->stdout != 1)
	    		{	    	
	    			dup2(process->stdout,1);
	    			close(process->stdout);
	    			if(process->fd[0] != 0)
	    				close(process->fd[0]);	    		
	    		}

	    		if(process->stderr != 2)
	    		{
	    	
	    			dup2(process->stderr,2);
	    			close(process->stderr);
	    		
	    		}
    		}
    			    	

    		execv(process->path,process->argv);
    		exit(1);
    	
    }else{
    	int statue;  

    	if(!process->background) 	
    		waitpid(process->pid ,&statue,0);

    	if(process-> stdin != 0)
    	{
    		close(process-> stdin);
    	}
        if(process-> stdout != 1)
    	{
    		close(process-> stdout );
    	}

    }
}


 /*
Fonction exec_static_cmd  appelle la fonction pour gérer la commande prédéfinis dans le systéme 
Paramètre process : processus actuel
Retourne un void 
*/
void exec_static_cmd(processus_t* process){

    char  * name  = process->argv[0];
    switch(get_command(name)){
        case CD:
        builtin_cd(process);
        break;
        case EXPORT:
        builtin_export(process);
        break;
        case CLEAR:
        builtin_clear();
        break;
        case UNSET:
        builtin_unset(process);
        break;
        case EXIT:
        builtin_exit();
        break;
        default:
        printf("%s name commande not found\n",name);
    }

}
 
 /*
Fonction exec_cmdline  boucle sur notre chaine et éxécute les commandes
Paramètre process : liste de processus
Retourne un int 
*/
int exec_cmdline(processus_t* process) {

    processus_t* p= process;
    while(p != NULL) {

         exec_process(p);
        if(p->next != NULL){
            p=p->next;
            continue;
        }
        if(p->next_success != NULL){
           if(WEXITSTATUS(p->status) == 0)
        	{
        		p = p->next_success;
        		continue;
        	}
        }
        if(p->next_fail != NULL){
        	if(WEXITSTATUS(p->status) == 1)
        	{
        		p = p->next_fail;
        		continue;
        	}
        }
       
        break;
    }
}


/*
Fonction init_commands  initialise le processus
Paramètre cmd : processus actuel
Retourne un void 
*/
void init_commands(processus_t * cmd ){
    for(int i = 0;i<MAX_CMD;i++){
      cmd[i].background = 0;
      cmd[i].stdin = 0;
      cmd[i].stdout = 1;
      cmd[i].stderr = 2;
      cmd[i].next = NULL;
      cmd[i].next_success = NULL;
      cmd[i].next_fail = NULL;
      cmd[i].pipe = NULL;
	  cmd[i].fd[0] = 0;
	  cmd[i].fd[1] = 1;

    }
  }

