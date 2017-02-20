#ifndef SERVICESMODEL_H
#define SERVICESMODEL_H

#include <QAbstractListModel>

class ServiceChannel
{
//    Q_OBJECT

public:
    explicit ServiceChannel(const int &type, const QString &channel, const QString &uuid);

    int type() const;
    QString channel() const;
    QString uuid() const;


    bool operator==(const ServiceChannel &sc) const;
private:
    int _type;
    QString _channel;
    QString _uuid;
};


class ServicesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ServicesModel(QObject *parent = nullptr);
    enum ServicesRoles {
        TypeRole = Qt::UserRole + 1,
        ChannelRole,
        UUidRole
    };


    void addService(const ServiceChannel &serviceChannel);
    QString const removeService(const QString &uuid);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<ServiceChannel> _services;


};

#endif // SERVICESMODEL_H
