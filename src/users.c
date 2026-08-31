#include "users.h"

static cos_user_t users[COS_MAX_USERS];
static uint32_t user_count;
static uint32_t current_uid;

static void copy_name(char *dst, const char *src) {
    uint32_t i = 0;
    while (i + 1 < COS_NAME_MAX && src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

void users_init(void) {
    user_count = 1;
    current_uid = 0;
    users[0].uid = 0;
    users[0].gid = 0;
    users[0].admin = 1;
    copy_name(users[0].name, "root");
}

const cos_user_t *users_get(uint32_t uid) {
    for (uint32_t i = 0; i < user_count; i++) if (users[i].uid == uid) return &users[i];
    return 0;
}

const cos_user_t *users_find(const char *name) {
    if (!name) return 0;
    for (uint32_t i = 0; i < user_count; i++) {
        uint32_t j = 0;
        while (users[i].name[j] == name[j] && users[i].name[j] && name[j]) j++;
        if (!users[i].name[j] && !name[j]) return &users[i];
    }
    return 0;
}

uint32_t users_current_uid(void) { return current_uid; }
void users_set_current(uint32_t uid) { if (users_get(uid)) current_uid = uid; }
uint8_t users_is_admin(uint32_t uid) {
    const cos_user_t *u = users_get(uid);
    return u ? u->admin : 0;
}

int users_switch_to_root(void) {
    if (!users_get(0)) return -1;
    current_uid = 0;
    return 0;
}
