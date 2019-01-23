#ifndef SEARCH_H
#define SEARCH_H
 typedef enum{
 	CD , 
 	EXPORT,
 	UNSET,
 	CLEAR,
 	EXIT,
 	UNDEFINED
 }static_cmd;

 char *     can_access_processus(char  * name);
 static_cmd get_command(char * name);
#endif