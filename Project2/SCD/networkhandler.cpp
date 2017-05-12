#include "networkhandler.h"

NetworkHandler::NetworkHandler(QObject *parent) : QObject(parent)
{

    readData = new QByteArray();
    buffer = new QBuffer(readData);
    buffer->open(QBuffer::ReadWrite);

    // create a QUDP socket
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress(SDC_IP), SDC_PORT);

    /* Signal detect when data has been arrived. */
    connect(socket, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError()));
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

void NetworkHandler::sendCommand(QString command)
{
    QByteArray datagram = command.toUtf8();
    socket->writeDatagram(datagram, QHostAddress(GALILEO_IP), GALILEO_PORT);
}

/* Handle signal when data has been arrive. */
void NetworkHandler::handleReadyRead()
{
    do {
        readData->resize(socket->pendingDatagramSize());
        socket->readDatagram(readData->data(), readData->size());
        buffer->reset();
        emit bufferHasData();
    } while (socket->hasPendingDatagrams());
}

/* Handle error with the connection. */
void NetworkHandler::handleError()
{
        qDebug() << QObject::tr("Error." ) << endl;
}
