#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#define IP "0.0.0.0"
#define PORT 18000

#include <QObject>
#include <QUdpSocket>
#include <QBuffer>
#include <QByteArray>

class NetworkHandler : public QObject
{
    Q_OBJECT

private:
    QUdpSocket *socket;
    QByteArray  *readData;

    /* Buffer to save data tah has been read. */
    QBuffer *buffer;


public:
    explicit NetworkHandler(QObject *parent = 0);
    ~NetworkHandler();
    const QString getReadData();
signals:

public slots:
    /* Handle signal when data has been arrive. */
    void handleReadyRead();
    /* Handle error with the connection. */
    void handleError(QAbstractSocket::SocketError);
};

#endif // NETWORKHANDLER_H
