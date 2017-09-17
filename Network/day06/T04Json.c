#include <inttypes.h>
#include "../header.h"
#include "cJSON.h"

struct Person
{
    int age;
    char name[32];
    int sex;
};

#if 0
{
    age: 18,
    name: "tom",
    man: true
}
#endif

int main()
{
#if 0
    {
        name:"tom",
        age:18,
        mobile:{
            branch: "apple",
            version: 6
        }
    }
#endif

    cJSON* mobile = cJSON_CreateObject();
    cJSON_AddStringToObject(mobile, "branch", "iphone");
    cJSON_AddNumberToObject(mobile, "version", 6);

    cJSON* root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "tom");
    cJSON_AddItemToObject(root, "mobile", mobile);
    cJSON_AddNumberToObject(root, "age", 18);

    char* p = cJSON_Print(root);
    //printf("%s\n", p);

    {
        cJSON* root = cJSON_Parse(p);
        cJSON* name = cJSON_GetObjectItem(root, "name");

        printf("%s=%s\n", name->string, name->valuestring);
//        name->string ;// key
 //       name->valuestring;// value

        cJSON* age = cJSON_GetObjectItem(root, "age");
        printf("%s=%d, %s=%g\n", age->string, age->valueint, age->string, age->valuedouble);
  //      age->string; // key
  //      age->valuedouble;// value
  //      age->valueint; // value

        cJSON_Delete(root);
    }


    free(p);
    cJSON_Delete(root);
}

#if 0
int main()
{
    struct Person p;
    p.age = 19;
    strcpy(p.name, "tom");
    p.sex = 1;

    // 打包成Json
    cJSON* root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", p.name);
    cJSON_AddNumberToObject(root, "age", p.age);
    cJSON_AddBoolToObject(root, "man", p.sex);

    char* jsonBuf = cJSON_Print(root);

    printf("jsonBuf:\n%s\n", jsonBuf);

    cJSON_Delete(root);
    free(jsonBuf);

}
#endif
