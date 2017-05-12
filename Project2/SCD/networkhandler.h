#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#define SDC_IP "0.0.0.0"
#define SDC_PORT 18000
#define GALILEO_IP "10.9.99.183"
#define GALILEO_PORT 20000

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
    void sendCommand(QString command);
signals:
    void bufferHasData(void);
public slots:
    /* Handle signal when data has been arrive. */
    void handleReadyRead();
    /* Handle error with the connection. */
    void handleError();
};

#endif // NETWORKHANDLER_H
