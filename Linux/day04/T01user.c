
#include "../header.h"
int main()
{
    // 执行这个文件的账户
    uid_t uid = getuid(); // 真实账户
    gid_t gid = getgid();

    // 行使权利的账户
    uid_t uid1 = geteuid(); // 有效账户
    gid_t gid1 = getegid();

    // 大部分情况下，这两个账户是一致的
    // 不一致的情况：设置了SUID，那么可能不一致

    printf(" uid %d,  gid %d\n", (int)uid, (int)gid);
    printf("euid %d, egid %d\n", (int)uid1, (int)gid1);
}
