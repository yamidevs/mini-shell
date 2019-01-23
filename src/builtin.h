#ifndef BUILTIN_H
#define BUILTIN_H
    #include "processus.h"
    int builtin_exit();
    int builtin_clear ();
    int builtin_cd (processus_t *cmd);
    int builtin_export (processus_t *cmd);
    int builtin_unset (processus_t *cmd);
#endif
