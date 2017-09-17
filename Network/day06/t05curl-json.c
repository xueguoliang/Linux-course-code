
#include "../header.h"
#include "cJSON.h"
#include <curl/curl.h>

int main()
{
    CURL* curl = curl_easy_init();

    char* jsonBuffer;
/*
    {
        cmd: login,
        username: xxx,
        password: yyy
    }

        {
            cmd: register,
            username: xxx,
            password: yyy
        }
*/
    cJSON* root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "register");
    cJSON_AddStringToObject(root, "username", "xxx");
    cJSON_AddStringToObject(root, "password", "yyy");
    jsonBuffer = cJSON_Print(root);
    cJSON_Delete(root);

    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:9988");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBuffer);

    curl_easy_perform(curl);
    free(jsonBuffer);
}
