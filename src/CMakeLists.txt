add_executable( minishell parser.c processus.c  main.c search.c builtin.c dispatcher.c)
install( TARGETS minishell DESTINATION bin)
