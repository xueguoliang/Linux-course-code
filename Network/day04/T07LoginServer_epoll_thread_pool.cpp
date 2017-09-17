
#include "../network.h"
#include <list>
#include <map>
using namespace std;

int epollfd;

// 文件描述符队列
list<int> socks;
pthread_mutex_t mutex;
//pthread_cond_t cond;
sem_t sem;

struct Channel
{
    int fd;
    char* buf;   //  [size]xxxxxxxxx
    int readSize;  // readSize = packetSize + 4
    int packetSize;

    Channel(int fd)
    {
        buf = new char[4096];
        readSize = 0;
        packetSize = 0;
        this->fd = fd;
    }
    ~Channel()
    {
        delete buf;
        close(fd);
    }
};

map<int, Channel*> channels;

void handleData(Channel* channel)
{
    int newfd = channel->fd;

    char* buf = channel->buf + 4;
    char* saveptr;
    const char* type = strtok_r(buf, "|", &saveptr);
    const char* user = strtok_r(NULL, "|", &saveptr);
    const char* pass = strtok_r(NULL, "|", &saveptr);

    if(type[0] == 'r')
    {
        // 注册, 用户名一行，密码一行
        FILE* fp = fopen("user.data", "a+");
        fprintf(fp, "%s\n%s\n", user, pass);
        fclose(fp);

        // [报文长度]r|ok
        myWriteBuf(newfd, "r|ok");
    }
    else if(type[0] == 'l')
    {
        char ubuf[1024];
        char pbuf[1024];
        char* p;
        // 登录
        FILE* fp = fopen("user.data", "r");
        while(1)
        {
            p = fgets(ubuf, sizeof(ubuf), fp);
            if(p == NULL)
                break;
            fgets(pbuf, sizeof(pbuf), fp); 

            ubuf[strlen(ubuf)-1] = 0; // remove \n
            pbuf[strlen(pbuf)-1] = 0; // remove \n

            if(strcmp(ubuf, user) == 0 &&
                    strcmp(pbuf, pass) == 0)
            {
                myWriteBuf(newfd, "l|ok");
                break;
            }
        }

        if(p == NULL) // 遍历整个文件，都没有匹配
        {
            myWriteBuf(newfd, "l|err");
        }

        fclose(fp);
    }
    else
    {
        myWriteBuf(newfd, "unknown command");
        // 说明有人捣乱，直接关闭socket
    }

    delete channel;
}

void recvData(Channel* channel)
{
    int ret = read(channel->fd, channel->buf + channel->readSize, channel->packetSize - channel->readSize + 4);
    if(ret > 0)
    {
        channel->readSize += ret;
        // 如果readSize是packetSize+4，那么报文读结束了
        if(channel->readSize == channel->packetSize + 4)
        {
            handleData(channel);   
        }
    }
    else if((ret < 0 && errno != EAGAIN) || ret == 0)
    {
        pthread_mutex_lock(&mutex);
        channels.erase(channels.find(channel->fd));
        pthread_mutex_unlock(&mutex);
        delete channel;
    }
    else 
    {
        // 继续读
        struct epoll_event ev;
        ev.data.fd = channel->fd;
        ev.events = EPOLLIN | EPOLLONESHOT;
        epoll_ctl(epollfd, EPOLL_CTL_MOD, channel->fd, &ev);

    }
}

void handle(int newfd)
{
    pthread_mutex_lock(&mutex);
    Channel* channel = channels[newfd];
    pthread_mutex_unlock(&mutex);

    if(channel->readSize < 4)
    {
        int ret = read(channel->fd, 
                channel->buf + channel->readSize, 
                4-channel->readSize);
        if(ret > 0)
        {
            channel->readSize += ret;
            // 读足够了，可以获取报文长度了
            if(channel->readSize == 4)
            {
                uint32_t* p = (uint32_t*)channel->buf;
                channel->packetSize = ntohl(*p);
                recvData(channel);
            }
        }
        else if((ret < 0 && errno != EAGAIN) || ret == 0) // 出错了，或者对方关闭socket，那么要清理
        {
            pthread_mutex_lock(&mutex);
            channels.erase(channels.find(newfd));
            pthread_mutex_unlock(&mutex);
            delete channel;
        }
        else
        {
            // 继续读
            struct epoll_event ev;
            ev.data.fd = newfd;
            ev.events = EPOLLIN | EPOLLONESHOT;
            epoll_ctl(epollfd, EPOLL_CTL_MOD, newfd, &ev);
        }
    }
    else
    {
        recvData(channel);
    }

}

void* thread_func(void* ptr)
{
    while(1)
    {
        // 等待信号
        sem_wait(&sem);

        pthread_mutex_lock(&mutex);
        if(socks.size() == 0)
        {
            pthread_mutex_unlock(&mutex);
            break;  // 一个信号对应一个任务，如果一个线程收到信号，但是没有任务，可以设计为线程结束
        }
        int newfd = *socks.begin();
        socks.erase(socks.begin());
        pthread_mutex_unlock(&mutex);

        handle(newfd);
    }

    return NULL;
}

void epoll_add(int epollfd, int sock, int event)
{
    struct epoll_event ev;
    ev.data.fd = sock;
    ev.events = event;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &ev);
}

int main()
{
    /// 3
    int server = myServer(9989, "0.0.0.0");

    // 创建文件
    close(open("user.data", O_CREAT|O_EXCL, 0777));

    pthread_mutex_init(&mutex, NULL);
    //    pthread_cond_init(&cond, NULL);
    sem_init(&sem, 0, 0);

    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, thread_func, NULL);
    pthread_create(&tid2, NULL, thread_func, NULL);
    pthread_create(&tid3, NULL, thread_func, NULL);

    epollfd = epoll_create(1024);
    epoll_add(epollfd, server, EPOLLIN);

    struct epoll_event ev[8];
    while(1)
    {
        int ret = epoll_wait(epollfd, ev, 8, 5000);
        if(ret > 0)
        {
            for(int i=0; i<ret; ++i)
            {
                if(ev[i].data.fd == server)
                {
                    // 4
                    int newfd = myAccept(server, NULL, NULL);
                    epoll_add(epollfd, newfd, EPOLLIN|EPOLLONESHOT);

                    Channel* channel = new Channel(newfd);
                    channels[newfd] = channel;

                    // newfd --> O_NONBLOCK
                    int flags = fcntl(newfd, F_GETFL);
                    flags |= O_NONBLOCK;
                    fcntl(newfd, F_SETFL, &flags);
                }
                else
                {
                    pthread_mutex_lock(&mutex);
                    socks.push_back(ev[i].data.fd);
                    pthread_mutex_unlock(&mutex);
                    // pthread_cond_signal(&cond);
                    sem_post(&sem); // 发送信号

                    // 把4号描述符从epoll中删除
                    // epoll_ctl(epollfd, EPOLL_CTL_DEL, ev[i].data.fd, NULL);
                }
            }
        }
    }

}
