#ifndef MERE_RPC_SERVER_H
#define MERE_RPC_SERVER_H

#include "registry.h"

#include "mere/message/server.h"

#include <QObject>
#include <QDebug>

namespace Mere
{
namespace RPC
{

class Server : public QObject
{
    Q_OBJECT
public:
    ~Server();
    explicit Server(const std::string &server, QObject *parent = nullptr);
    int start() const;

    int add(const std::string &name, QObject *service);
    QObject* get(const std::string &name);

public slots:
    void message(const std::string &message);

private:
    std::string m_path;
    Mere::Message::Server *m_server;
    Registry *m_registry;
};

}
}


#endif // MERE_RPC_SERVER_H
