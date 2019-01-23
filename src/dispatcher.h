#ifndef DISPATCHER_H
#define DISPATCHER_H
#include "processus.h"
#include "parser.h"
void dispatch_redirection(int * n_cmd , processus_t * cmd,char ** current);
void dispatch_tokens(int * n_cmd , processus_t * cmd,char ** current);
void dispatch_sep(int  * n_cmd , processus_t * cmd, separator_t sep);
#endif