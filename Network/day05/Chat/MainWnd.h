#ifndef MAINWND_H
#define MAINWND_H

#include <QWidget>
#include <QTreeWidget>
#include <QListWidget>
#include <QTextEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QTcpSocket>
#include <QHostAddress>

class MainWnd : public QWidget
{
    Q_OBJECT
public:
    explicit MainWnd(QWidget *parent = 0);

    QListWidget* left;
    QWidget* right;

    QTextEdit* input;
    QTextBrowser* output;
    QPushButton* send;

    QTcpSocket* socket;

    int packetSize;
    QByteArray buf;

    void readData();

signals:

public slots:
    void onReadyRead();
    void onSend();
};

#endif // MAINWND_H
