#include "Sender.h"
#include <QtNetwork>

#define BROADCAST_PORT 45454

Sender::Sender(QWidget *)
{
    udpSocket = new QUdpSocket;
}

void Sender::broadcastDatagram(QByteArray datagram)
{
    if(udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, BROADCAST_PORT) == -1)
    {
        qDebug() << "[Sender]: " << udpSocket->errorString();
    }
}
