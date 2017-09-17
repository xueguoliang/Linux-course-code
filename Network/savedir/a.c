
int main()
{
    int server = myServer(9989, "127.0.0.1");
    if(server == -1)
    {
        return -1;
    }

    int newfd = myAccept(server, NULL, NULL);
    if(newfd == -1)
    {
        return -1;
    }

}
