#include "networkhandler.h"

NetworkHandler::NetworkHandler(QObject *parent) : QObject(parent)
{

    readData = new QByteArray();
    buffer = new QBuffer(readData);
    buffer->open(QBuffer::ReadWrite);

    // create a QUDP socket
    socket = new QUdpSocket(this);

    // The most common way to use QUdpSocket class is
    // to bind to an address and port using bind()
    // bool QAbstractSocket::bind(const QHostAddress & address,
    //     quint16 port = 0, BindMode mode = DefaultForPlatform)
    socket->bind(QHostAddress::LocalHost, 8000);

    /* Signal detect when data has been arrived. */
    connect(socket, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));

}

NetworkHandler::~NetworkHandler()
{
    delete socket;
    delete readData;
    delete buffer;
}

/* Return read data. */
const QString NetworkHandler::getReadData()
{
    if(buffer->bytesAvailable() > 0)
    {
        return QString(buffer->readAll());
    }
    return QString("");
}

/* Handle signal when data has been arrive. */
void NetworkHandler::handleReadyRead()
{

    do {
        readData->resize(socket->pendingDatagramSize());
        socket->readDatagram(readData->data(), readData->size());
    } while (socket->hasPendingDatagrams());
}
