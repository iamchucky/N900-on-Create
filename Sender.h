#ifndef SENDER_H
#define SENDER_H

class QUdpSocket;
class QWidget;
class QByteArray;

class Sender
{
public:
    Sender(QWidget *parent = 0);
    void broadcastDatagram(QByteArray datagram);
private:
    QUdpSocket *udpSocket;
};

#endif // SENDER_H
