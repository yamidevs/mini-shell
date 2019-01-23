#include "stdio.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "stdlib.h"
#include "processus.h"
#include "parser.h"
#include "string.h"
#include "search.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "dispatcher.h"

/*
Fichier main.c : fichier principal du programe
Dépendances : parser.h processus.h dispatcher.h search.h
*/

int main(int argc , char ** argv){

  while(1){
   processus_t  * cmd  = (processus_t*)malloc(sizeof(processus_t) * MAX_CMD);
   //initilise le tableau de commande
   init_commands(cmd);
   char str[MAX_TOKEN_STRE];
   char repertoire[MAX_PATH_SIZE];
   //récupérer le path répértoire courant
   getcwd(repertoire, sizeof(repertoire));
   printf("\n%s $ ",repertoire);
   // récupérer le contenue de l'entrée standard
   fgets(str,MAX_TOKEN_STRE + 1,stdin);
   // si y'as aucun caractére on fait aucun traitement
   if(strlen(str)<2) continue;
   trim(str);
   clean(str);
   char ** tokens = (char**)malloc(sizeof(char*)*MAX_TOKEN_STRE);
   split_str(str,tokens,MAX_TOKEN_STRE);
   int n_cmd = 0;
   char ** current = tokens;  
   int next = 0;
   while(next != -1){
      separator_t sep;
      next = split_command(current,&sep);
   	  dispatch_redirection(&n_cmd,cmd,current);
      dispatch_tokens(&n_cmd,cmd,current);
      dispatch_sep(&n_cmd,cmd,sep);     
      current += next;
      n_cmd++;
   }

   //traitement des commandes
   exec_cmdline(cmd);
 }

   return 1;
}

