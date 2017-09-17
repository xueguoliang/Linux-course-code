#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
struct Data
{
    char* buf;
    int size;
    int dataSize;
};

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    struct Data* data = (struct Data*)userdata;
    // ptr --> point to data
    // size * nmemb = sizeof  data
    // userdata: callback data pointer

    int dataSize = size*nmemb;

    memcpy(data->buf + data->dataSize, ptr, dataSize);
    data->dataSize += dataSize;

    return dataSize;
}

// 使用libcurl去获取网页
int main()
{
    // 创建curl对象
    // 设置curl对象参数
    // 通过curl对象向服务器发起请求
    // 读服务器的响应数据

    CURLcode code;

    // create curl object
    CURL* curl = curl_easy_init();
    if(curl == NULL)
    {
        printf("create curl error");
        return 0;
    }

    // setup curl object
    code = curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1/cgi-bin/a.out");
    if(code != CURLE_OK)
    {
        printf("setopt error\n");
        return 0;
    }

    struct Data data;
    data.buf = malloc(8192);
    data.size = 8192;
    data.dataSize = 0;

    code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "user=aaa&pass=bbb");
    curl_easy_setopt(curl, CURLOPT_POST, 1);

    // send request, 阻塞调用，服务器响应将会输出到标准输出
    code = curl_easy_perform(curl);
    if(code  != CURLE_OK)
    {
        printf("perform error code=%d\n", (int)code);
        return 0;
    }

    data.buf[data.dataSize] = 0;
    printf("recv data is: %s\n", data.buf);
    free(data.buf);

    curl_easy_cleanup(curl);
}
