#include "search.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

/*
Fichier search.c : fichier permet la recherche d'une commande ou la recherche d'une commande prédifinis sur le programme
Dépendances : aucune dépendances
*/


/*
Fonction can_access_processus , cherche le chemin complet de la commande si elle trouve elle retoune le chemin complet sinon retourne NULL
Paramètre name : le nom de la commande
Retourne une chaine de caractére 
*/

 char *  can_access_processus(char  * name)
 {
 	char * path = getenv("PATH");
 	char * directory = strdup(path);
 	char * dupDirectory = strdup(directory);
  	for(char * p  = strtok_r(directory,":",&dupDirectory);p;p =  strtok_r(NULL,":",&dupDirectory)){
 		char * dup = strdup(p);
 		strcat(dup,"/");
 		strcat(dup,name);
 		// on vérifie si notre processus existe , si oui on retourne la chaine
 		if(access(dup,X_OK) == 0)
 		{
 			return dup;
 		}
 	}

 	return NULL;
  }



/*
Fonction get_command , on vérifie si on gére la commande sinon on retourne undifined
Paramètre name : le nom de la commande
Retourne static_cmd
*/
 static_cmd get_command(char  * name){

  	if(!strcmp(name,"cd")){
  		return CD;
  	}else 
  	if(!strcmp(name,"export")){
  		return EXPORT;
  	}else 
  	if(!strcmp(name,"clear")){
  		return CLEAR;
  	}else 
  	if(!strcmp(name,"unset")){
  		return UNSET;
  	}else
    if(!strcmp(name,"exit")){
      return EXIT;
    }

  	return UNDEFINED;
  } 