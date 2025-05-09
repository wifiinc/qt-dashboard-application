#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QString>

class Tcpsocket {
public:
    Tcpsocket(const QString &host, int port);
    QString verzendPakket(const QString &cmd); // "ON" or "OFF"

private:
    QString serverHost;
    int serverPort;
};

#endif // TCPSOCKET_H
