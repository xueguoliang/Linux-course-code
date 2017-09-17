#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <string>
#include <map>
using namespace std;

class Channel
{
public:
    Channel(){
        readSize = 0;
    }
    bufferevent* bev;
    char buf[4096];
    int readSize;
    int packetSize;
    string ip;
};

// ip
map<string, Channel*> channels;

void readHeader(struct bufferevent* bev, Channel* channel)
{
    channel->readSize += bufferevent_read(bev, channel->buf+channel->readSize, 4-channel->readSize);
    if(channel->readSize == 4)
    {
        uint32_t packetSize = *(uint32_t*)channel->buf;
        channel->packetSize = ntohl(packetSize);
    }
}
void readData(struct bufferevent* bev, Channel* channel)
{
    channel->readSize += bufferevent_read(bev, channel->buf+channel->readSize,
                                          channel->packetSize+4 - channel->readSize);
}

void handleData(Channel* channel)
{
    // [changdu]191.123.11.11|content
    channel->buf[channel->readSize] = 0;
    char* buf = channel->buf+4;

    char* toip = strtok(buf, "|");
    char* content = strtok(NULL, "\0");

    Channel* peer = channels[toip];
    if(peer == NULL)
    {
        printf("peer offline");
    }
    else
    {
        // [changdu]192.11.11.11|content
        char buf[4096];
        sprintf(buf, "%s|%s", channel->ip.c_str(), content);

        uint32_t len = strlen(buf);
        len = htonl(len);
        bufferevent_write(peer->bev, (char*)&len, 4);
        bufferevent_write(peer->bev, buf, strlen(buf));
    }

    channel->readSize = 0; // for next read
}

// read socket
void readcb(struct bufferevent *bev, void *ctx)
{
    Channel* channel = (Channel*)ctx;
    if(channel->readSize < 4)
    {
        readHeader(bev, channel);
        if(channel->readSize == 4)
        {
            readData(bev, channel);
        }
    }
    else
    {
        readData(bev, channel);
    }

    if(channel->readSize == channel->packetSize+4)
    {
        handleData(channel);
    }

#if 0
    char buf[1024];
    memset(buf, 0, sizeof(buf));

    bufferevent_read(bev, buf, sizeof(buf));

    printf("read data is [%s]\n", buf);
#endif
}

// write socket
void writecb(struct bufferevent *bev, void *ctx)
{

}

// close socket
void eventcb(struct bufferevent *bev, short what, void *ctx)
{
    if(what & BEV_EVENT_CONNECTED)
    {
        printf("connected\n");
    }
    else
    {
        if(what & BEV_EVENT_EOF)
        {
            printf("peer close socket\n");
        }

        bufferevent_free(bev);
    }
}
/*
   void bufferevent_setcb(struct bufferevent *bufev,
   bufferevent_data_cb readcb, bufferevent_data_cb writecb,
   bufferevent_event_cb eventcb, void *cbarg);
   */

/*1, 2, 3
    4
*/
void newUserOnlie(Channel* channel)
{
    for(auto it = channels.begin(); it!=channels.end(); ++it)
    {
        Channel* user = it->second;

        uint32_t len = strlen(user->ip.c_str());
        len = htonl(len);
        bufferevent_write(channel->bev, (char*)&len, 4);
        bufferevent_write(channel->bev, user->ip.c_str(), strlen(user->ip.c_str()));

        len = strlen(channel->ip.c_str());
        len = htonl(len);
        bufferevent_write(user->bev, (char*)&len, 4);
        bufferevent_write(user->bev, channel->ip.c_str(), strlen(channel->ip.c_str()));
    }
}

void listen_cb(struct evconnlistener * listener, evutil_socket_t newfd, struct sockaddr * addr, int socklen, void *ptr)
{
    struct sockaddr_in* p = (struct sockaddr_in*)addr;
    string ip = inet_ntoa(p->sin_addr);
    printf("connect ip is %s\n", ip.c_str());

    // send allusers --> channel
    // send channel --> allusers


    // newfd --> base
    struct event_base* base = (struct event_base*)ptr;

    struct bufferevent* bev = bufferevent_socket_new(base, newfd, BEV_OPT_CLOSE_ON_FREE);

    Channel* channel = new Channel;
    bufferevent_setcb(bev, readcb, NULL, eventcb, channel);
    // EPOLLIN EPOLLOUT EPOLLONESHOT
    bufferevent_enable(bev, EV_READ|EV_WRITE);


    channel->ip = ip;
    channel->bev = bev;

    uint32_t len = 5;
    len = htonl(len);
    bufferevent_write(bev, (char*)&len, 4);
    bufferevent_write(bev, "hello", 5);

    newUserOnlie(channel);

    channels[ip] = channel;
}

int main()
{
    // 创建event_base，event_base是事件集合
    struct event_base* base = event_base_new();

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9988);
    addr.sin_addr.s_addr = INADDR_ANY;


    // 创建监听的服务器socket，并且加入到base集合中
    // 当有客户端连接时，回调listen_cb函数
    //
    // 已初始化状态（initialized）
    struct evconnlistener* listener = evconnlistener_new_bind(base,
                                                              listen_cb,
                                                              base,
                                                              LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,
                                                              250,
                                                              (struct sockaddr*)&addr,
                                                              sizeof(addr));
    // 进入 未决（pending） 状态，只有pending状态的event，才可能被激活
    evconnlistener_enable(listener);

    // 进入死循环，在死循环中，监听所有未决状态的event，是否ready，一旦条件ready，那么这个event就变成激活状态，回调回调函数
    event_base_dispatch(base);

    // end ...
    event_base_free(base);

    return 0;

}
