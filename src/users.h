#ifndef COS_USERS_H
#define COS_USERS_H
#include <stdint.h>

#define COS_MAX_USERS 16
#define COS_MAX_GROUPS 16
#define COS_NAME_MAX 32

typedef struct {
    uint32_t uid;
    uint32_t gid;
    char name[COS_NAME_MAX];
    uint8_t admin;
} cos_user_t;

void users_init(void);
const cos_user_t *users_get(uint32_t uid);
const cos_user_t *users_find(const char *name);
uint32_t users_current_uid(void);
void users_set_current(uint32_t uid);
uint8_t users_is_admin(uint32_t uid);
int users_switch_to_root(void);
int users_switch_to_user(uint32_t uid);

#endif
