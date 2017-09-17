#include "MainWnd.h"
#include <QSplitter>
#include <QHBoxLayout>
#include <QtEndian>
#include <QDebug>
MainWnd::MainWnd(QWidget *parent) : QWidget(parent)
{
    left = new QListWidget;
    right = new QWidget;

    QHBoxLayout* lay = new QHBoxLayout(this);

    QSplitter* splitter = new QSplitter;
    splitter->addWidget(left);
    splitter->addWidget(right);

    lay->addWidget(splitter);

    QVBoxLayout* vBox = new QVBoxLayout(right);
    vBox->setMargin(0);
    vBox->addWidget(output = new QTextBrowser, 2);
    vBox->addWidget(input = new QTextEdit, 1);
    vBox->addWidget(send = new QPushButton("send"));

    // connect to server
    socket = new QTcpSocket;
    socket->bind(QHostAddress("192.168.19.42"));
    socket->connectToHost(QHostAddress("192.168.19.35"), 9988);
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    connect(send, SIGNAL(clicked()), this, SLOT(onSend()));
}

void MainWnd::readData()
{
    buf+=socket->read(packetSize+4 - buf.size());
}

void MainWnd::onReadyRead()
{
    while(socket->bytesAvailable() > 0)
    {
        if(buf.size() < 4)
        {
            buf += socket->read(4-buf.size());
            if(buf.size() == 4)
            {
                uint32_t p = *(uint32_t*)buf.data();
              //  packetSize = ntohl(p);
                packetSize = qFromBigEndian(p);
                qDebug() <<"packet Size is:" <<packetSize;

                readData();
            }
        }
        else
        {
            readData();
        }
        if(packetSize + 4 == buf.size())
        {
            buf = buf.mid(4);
            qDebug() << buf;

            // abc
            // 192.168.19.35
            // 192.168.19.35|chat |content
            int index = buf.indexOf("|");
            QString ip;

            if(index == -1) // online msg
            {
                ip = buf;
                this->left->addItem(ip);
            }
            else // chat msg
            {
                QString msg;
                ip = buf.left(index);
                msg = buf.mid(index+1);

                this->output->append(ip + " say: " + msg );
            }
        }
        buf.clear(); // readSize = 0
    }
}

void MainWnd::onSend()
{
    if(input->toPlainText().size() == 0) return;
    if(left->currentItem() == NULL) return;

    QListWidgetItem* item = left->currentItem();
    QString ip = item->text();

    QByteArray buf = ip.toUtf8() + "|" + input->toPlainText().toUtf8();
    uint32_t size = buf.size();
    size = qToBigEndian(size);

    socket->write((const char*)&size, 4);
    socket->write(buf);

    output->append("i say: "+input->toPlainText());
    input->clear();

}
