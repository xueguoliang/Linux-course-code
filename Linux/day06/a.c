
// 检测系统是大端还是小端字节序
int main()
{
    int a = 0x12345678;

    char* p = (char*)&a;
    printf("0x%x\n", (int)*p);



}
