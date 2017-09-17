#include "MyServer.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

MyServer::MyServer(QObject *parent) : QObject(parent)
{
    server.listen(QHostAddress::Any, 9988);

    connect(&server, SIGNAL(requestReady(Tufao::HttpServerRequest&,Tufao::HttpServerResponse&)),
            this, SLOT(onRequestReady(Tufao::HttpServerRequest&,Tufao::HttpServerResponse&)));

}

void MyServer::handlePostData(HttpServerRequest &request, HttpServerResponse &response)
{
    //  qDebug() <<"body is:" << request.readBody();
    QByteArray body = request.readBody();
    qDebug() << body;

    response.writeHead(HttpResponseStatus::OK); // 200

    // Parse
    QJsonDocument doc = QJsonDocument::fromJson(body);
    QJsonObject root = doc.object();// root object
    //    QJsonValue cmd = root.value("cmd");
    //    QJsonValue username = root.value("username");
    //    QJsonValue password = root.value("password");
    QString cmd = root.value("cmd").toString();
    QString username = root.value("username").toString();
    QString password = root.value("password").toString();
    if(cmd == "register")
    {
        /*
 * {
 *      result: "ok",
 *      cmd: register
 * }
*/
        // wirte username & password --> database
        QJsonObject obj;
        obj.insert("result", "ok");
        obj.insert("cmd", "register");

        QJsonDocument doc(obj);
        QByteArray respJson = doc.toJson();

        response.end(respJson);
        return;
    }
    else if(cmd == "login")
    {
        QJsonObject obj;
        obj.insert("result", "ok");
        obj.insert("cmd", "login");

        QJsonDocument doc(obj);
        QByteArray respJson = doc.toJson();

        response.end(respJson);
        return;
    }

    response.end("cmd not found");
}
/*
    request-line
    headers
    headers
    headers

    body
*/
void MyServer::onRequestReady(HttpServerRequest &request, HttpServerResponse &response)
{
#if 0
    qDebug() << "url is"<<request.url().path();
    //   qDebug() << request.httpVersion(); // HTTP/1.1
    qDebug() << request.method(); // POST  or GET

    // response: write data to Client
    response.writeHead(HttpResponseStatus::OK); // 200
    //    response.writeHead(HttpResponseStatus::NOT_FOUND); // 404
    response.end("<h1>hello world</h1>");
#endif

    if(request.method() == "POST")
    {
        connect(&request, &HttpServerRequest::end, [&](){
            handlePostData(request, response);
        });
    }

}
