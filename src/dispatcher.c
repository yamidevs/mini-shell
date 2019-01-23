#include "dispatcher.h"
#include "parser.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <glob.h>
#include "string.h"
#include <stdio.h>
#include <fcntl.h>
#include<unistd.h>

/*
Fichier dispatcher.c : permet de remplir tableau de commandes , les flux ...
Dépendances : parser.h  dispatcher.h 
*/


/*
Fonction dispatch_redirection
Paramètre n_cmd : pointer entier le numéro acutel des commandes 
Paramètre cmd : type processus_t , listes des commandes
Paramètre current : type tableau de chaine de caractére ,chaine entrée mais découper par les espaces
Retourne un void
*/

void dispatch_redirection(int * n_cmd , processus_t * cmd,char ** current){
	  redirection_t redirection_index;
      int path_index = redirection(current,&redirection_index);
      if(path_index != -1)
         strtok(current[path_index],"\n"); 
      switch(redirection_index)
      {
      	//ajoute a la fin d'un fichier
      	case OUTPUT_APPEND:
      	  cmd[*n_cmd].stdout = open(current[path_index],O_CREAT | O_APPEND | O_WRONLY,0660);
      	break;
      	// ajoute au début du fichier
      	case OUTPUT:
      	  cmd[*n_cmd].stdout = open(current[path_index],O_CREAT | O_WRONLY , S_IWRITE|S_IREAD);
      	break;
      	//redirection d'un fichier dans l'entrée standar
      	case INPUT:
      	  cmd[*n_cmd].stdin = open(current[path_index],O_RDONLY,S_IREAD);
      	break;
      	//redurection flux erreur dans un fichier , et dans le debut du fichier
      	case ERROR:
      	  cmd[*n_cmd].stderr = open(current[path_index],O_CREAT | O_WRONLY , S_IWRITE|S_IREAD);
      	break;
      	//redurection flux erreur dans un fichier , l'ajoute a la fin du fichier
      	case ERROR_APPEND:
      	  cmd[*n_cmd].stderr = open(current[path_index],O_CREAT | O_APPEND ,S_IWRITE| S_IWRITE);
      	break;
      	case ERROR_OUTPUT:
      	  cmd[*n_cmd].stderr = cmd[*n_cmd].stdout;
      	break;
      }	
}

/*
Fonction dispatch_tokens
Paramètre n_cmd : pointer entier le numéro acutel des commandes 
Paramètre cmd : type processus_t , listes des commandes
Paramètre current : type tableau de chaine de caractére ,chaine entrée mais découper par les espaces
Retourne un void
*/

void dispatch_tokens(int * n_cmd , processus_t * cmd,char ** current){

	int i = 0;
	for(char ** p = current ; *p != NULL ;p++){
           strtok(*p,"\n"); 


           if(**p == ' ' || **p == '\n' || **p == '\t' || **p == '\0')
           	continue;


           //en remplace le token par sa valeur exemple echo $var
           if(**p == '$')
           {
           	   char * t_var = *p;
           	   t_var++;
           	   cmd[*n_cmd].argv[i++] = getenv(t_var);

           }else{

           	   //gestion du joker
           	   char * token = strdup(*p);
           	   if (strstr(*p,"*") != NULL) {
                	glob_t g;
                	if (glob(token,0,NULL,&g)==0){
                    	for (int gp=0;gp<g.gl_pathc;++gp)
                        	cmd[*n_cmd].argv[i++]=strdup(g.gl_pathv[gp]);
                	}
                	globfree(&g);
                	continue;
            	}else{
					       cmd[*n_cmd].argv[i++] = *p;
            	}
               
           }

      }
      cmd[*n_cmd].argv[i] = NULL;
}


/*
Fonction dispatch_sep
Paramètre n_cmd : pointer entier le numéro acutel des commandes 
Paramètre cmd : type processus_t , listes des commandes
Paramètre sep : type separator_t , enumerateur pour definir separateur entre commande
Retourne un void
*/
void dispatch_sep(int  * n_cmd , processus_t * cmd , separator_t sep){
     int fd[2];
      switch(sep)
      {
         case AND : 
            cmd[*n_cmd].next_success = &cmd[*n_cmd + 1];
         break;
         case OR : 
            cmd[*n_cmd].next_fail = &cmd[*n_cmd + 1];
         break;
         case SEMICOL : 
            cmd[*n_cmd].next = &cmd[*n_cmd + 1];
            cmd[*n_cmd].next_fail = NULL;
         break;
         case PARAL : 
            cmd[*n_cmd].next = &cmd[*n_cmd + 1];
            cmd[*n_cmd].background = 1;
         break;
         case PIPE : 
           pipe(fd);
           cmd[*n_cmd].stdout = fd[1];
           cmd[*n_cmd].pipe = &cmd[*n_cmd + 1];
           cmd[*n_cmd].fd[0] = fd[0];
           cmd[*n_cmd].fd[1] = fd[1];
           cmd[*n_cmd].next = &cmd[*n_cmd + 1];
           cmd[*n_cmd + 1].stdin = fd[0];
           cmd[*n_cmd + 1].fd[0] = fd[0];
           cmd[*n_cmd + 1].fd[1] = fd[1];
         break;
      }
}