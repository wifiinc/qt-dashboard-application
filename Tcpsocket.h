#ifndef LIGHTCLIENT_H
#define LIGHTCLIENT_H

#include <QString>

class LightClient {
public:
    LightClient(const QString &host, int port);
    QString sendCommand(const QString &cmd); // "ON" or "OFF"

private:
    QString serverHost;
    int serverPort;
};

#endif // LIGHTCLIENT_H
