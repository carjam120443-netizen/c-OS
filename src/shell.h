#ifndef COS_SHELL_H
#define COS_SHELL_H
#include <stdint.h>

#define SHELL_LINE_MAX 128
void shell_init(void);
void shell_run_command(const char *line);
void shell_print_prompt(void);

#endif
