
void fun()
{
    abort(); // 给自己发送个SIGABRT信号
}
void foo()
{
    func();
}
int main()
{
    foo();
}
