
#include <string.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>

// read socket
void readcb(struct bufferevent *bev, void *ctx)
{
    char buf[1024];
    memset(buf, 0, sizeof(buf));

    bufferevent_read(bev, buf, sizeof(buf));

    printf("read data is [%s]\n", buf);
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

void listen_cb(struct evconnlistener * listener, evutil_socket_t newfd, struct sockaddr * addr, int socklen, void *ptr)
{
    // newfd --> base
    struct event_base* base = (struct event_base*)ptr;

    struct bufferevent* bev = bufferevent_socket_new(base, newfd, BEV_OPT_CLOSE_ON_FREE);

    bufferevent_setcb(bev, readcb, NULL, eventcb, base);
    // EPOLLIN EPOLLOUT EPOLLONESHOT
    bufferevent_enable(bev, EV_READ|EV_WRITE);
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
