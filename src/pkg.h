#ifndef COS_PKG_H
#define COS_PKG_H
#include <stdint.h>

#define PKG_NAME_MAX 48
#define PKG_VERSION_MAX 32
#define PKG_REPO_MAX 128

void pkg_init(void);
int pkg_command(const char *line);
int pkg_update(void);
int pkg_search(const char *name);
int pkg_install(const char *name);
int pkg_remove(const char *name);
int pkg_list(void);
int pkg_info(const char *name);

#endif
