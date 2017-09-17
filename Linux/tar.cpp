#include "header.h"
#include <string>
using namespace std;

void tar_file(string src, FILE* fp)
{
    fprintf(fp, "f\n");
    fprintf(fp, "%s\n", src.c_str());

    struct stat stat_buf;
    stat(src.c_str(), &stat_buf);
    fprintf(fp, "%d\n", (int)stat_buf.st_size);

    FILE* fpin = fopen(src.c_str(), "r");

    char buf[4096];
    while(1)
    {
        int ret = fread(buf, 1, sizeof(buf), fpin);
        if(ret <= 0) break;
        fwrite(buf, ret, 1, fp);
    }

    fclose(fpin);
}

void tar_dir(string src, FILE* fp)
{
    fprintf(fp, "d\n");
    fprintf(fp, "%s\n", src.c_str());

    DIR* dir = opendir(src.c_str());
    while(1)
    {
        struct dirent* entry = readdir(dir);

        if(entry == NULL) break;
        if(strcmp(entry->d_name, "..") == 0) continue;
        if(strcmp(entry->d_name, ".") == 0) continue;

        if(entry->d_type == DT_REG) // regular 普通文件
        {
            tar_file(src + "/" + entry->d_name, fp);
        }
        else if(entry->d_type == DT_DIR) // 目录
        {
            tar_dir(src + "/" + entry->d_name, fp);
        }
    }

    closedir(dir);
}

// a.out my.tar day03
// a.out my.tar ../../XT
// a.out cwd/my.tar XT
int main(int argc, char* argv[])
{
    //const char* dst = argv[1];
    string src1 = argv[2];

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    string dst = string(cwd) + "/" + argv[1];

    chdir((src1+"/..").c_str());
    getcwd(cwd, sizeof(cwd));

    // 反搜索/
    realpath(argv[2], cwd);
    string src = rindex(cwd, '/') + 1;

    printf("src=%s, dst=%s, cwd=%s\n", src.c_str(), dst.c_str(), cwd);

//    char src[1024];
//    realpath(src1+"/..", src);


    FILE* fp = fopen(dst.c_str(), "w");

    tar_dir(src, fp);

    fclose(fp);
}
