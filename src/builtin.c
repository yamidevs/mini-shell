

#include "builtin.h"
#include "string.h"
#include "unistd.h"
#include <stdio.h>
#include <stdlib.h>
#include "processus.h"

/*
Fichier search.c : fichier permet de gérer des commandes comme export , cd ... 
Dépendances : processus.h
*/


/*
 Fonction cmd_exit
 Paramètre : Aucun
 Role : Permet d arreter la console
 Retourne 0 si tout est ok
 */
int builtin_exit () {
    exit(EXIT_SUCCESS);
    return 0;
}

/*
 Fonction cmd_clear
 Paramètre : Aucun
 Role : Permet de nettoyer l ecran de la console
 Retourne 0 si tout est ok
 */
int builtin_clear () {
	printf("\033[2J\033[1;1H");
	return 0;
}

/*
 Fonction cmd_cd
 Paramètre command_t *cmd : structure avec les commandes
 Paramètre char repertoire [MAX_REP]: Contient le repertoire cible
 Role : Permet de se deplacer
 Retourne 0 si tout est ok
 */
int builtin_cd(processus_t *cmd){
	if (cmd->argv[1] == NULL){
		chdir (getenv("HOME"));
		
	} else {
		char repertoire[MAX_PATH_SIZE];
   		getcwd(repertoire, sizeof(repertoire));
		if (*(cmd->argv[1]) == '/'){
			strcpy(repertoire, cmd->argv[1]);
		}else{
			char* saveptr;
			char* token;
			char* str = strdup(cmd->argv[1]);
		while ((token = strtok_r(str, "/", &saveptr))){
				str = NULL;
				if (strcmp(token, "..") == 0){
					int i = (int) (strlen(repertoire));
					while (repertoire[i] != '/' && i != 1){
						repertoire[i] = '\0';
						--i;
					}
					repertoire [i] = '\0';
					continue;
				}
				if (repertoire [strlen(repertoire)-1] != '/')
					strcat(repertoire, "/");
				strcat(repertoire,token);
			}
		}

		chdir(repertoire);
	}
	return 0;
}

/*
 Fonction cmd_export
 Paramètre command_t *cmd : structure avec les commandes
 Role : Permet d enregister une variable dans le tableau env.
 Retourne 0 si tout est ok
 */
int builtin_export (processus_t *cmd) {
	char* saveptr;
	char* str = strdup(cmd->argv[1]);
	
	if (str == NULL) {
		return 0;
	}
	
	char * nomVar = strtok_r(str, "=", &saveptr);
	str = NULL;
	if (nomVar == NULL) {
		return 0;
	}
	char * valeurVar = strtok_r(str, "=", &saveptr);
	if (valeurVar == NULL) {
		return 0;
	}
	setenv(nomVar,valeurVar,1);
	return 0;
}

/*
 Fonction cmd_unset
 Paramètre command_t *cmd : structure avec les commandes
 Role : Permet de supprimer une variable dans le tableau env.
 Retourne 0 si tout est ok
 */
int builtin_unset (processus_t *cmd){
    if(cmd->argv[1] == NULL){
        return 0;
    }
    unsetenv(cmd->argv[1]);
    return 0;
}

