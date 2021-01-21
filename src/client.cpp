#include "client.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

Mere::RPC::Client::Client(const QString &path, QObject *parent)
    : QObject(parent),
      m_path(path)
{
    if (m_path.startsWith("/"))
        m_path = m_path.prepend("/");

    QStringList parts = path.split("/");
    if (parts.size() < 3)
        throw std::invalid_argument("Invalid service path!");

    m_service = parts.takeLast();
    if(m_service.isNull() || m_service.isEmpty())
        throw std::invalid_argument("Invalid service name in service path!");

    m_server  = parts.join("/");

    m_client = new Mere::Message::Client(m_server.toStdString().c_str());
    connect(m_client, SIGNAL(message(const QString &)), this, SLOT(message(const QString &)));

    m_client->join();
}

Mere::RPC::Client* Mere::RPC::Client::method(const QString &method)
{
    m_method = method;
    return this;
};

Mere::RPC::Client* Mere::RPC::Client::with(const std::vector<QVariant> args)
{
    m_args = args;

    return this;
}

void Mere::RPC::Client::call()
{
    QJsonArray args;
    for(const auto &arg : m_args)
        args.append(arg.toJsonValue());

    // create the main object
    QJsonObject jsonObj;
    jsonObj.insert("version", 1.0);
    jsonObj.insert("uuid", QUuid::createUuid().toString());
    jsonObj.insert("service", m_service);
    jsonObj.insert("method", m_method);
    jsonObj.insert("args", args);

    QJsonDocument jsonDocument(jsonObj);
    QString request(jsonDocument.toJson(QJsonDocument::Compact));

    m_client->send(request);
}

void Mere::RPC::Client::message(const QString &message)
{
    if (m_callback)
        m_callback(QVariant(message), QVariant());
}