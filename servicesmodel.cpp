#include "servicesmodel.h"
ServiceChannel::ServiceChannel(const int &type, const QString &channel, const QString &uuid)
    : _type(type), _channel(channel), _uuid(uuid)
{
}

bool ServiceChannel::operator==(const ServiceChannel &sc) const{
    return this->uuid() == sc.uuid();
}

int ServiceChannel::type() const
{
    return _type;
}

QString ServiceChannel::channel() const
{
    return _channel;
}

QString ServiceChannel::uuid() const
{
    return _uuid;
}

ServicesModel::ServicesModel(QObject *parent) : QAbstractListModel(parent)
{

}

void ServicesModel::addService(const ServiceChannel &serviceChannel)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _services << serviceChannel;
    endInsertRows();
    emit itemAdded();
}

QString const ServicesModel::removeService(const QString &uuid)
{   QString channel;
    foreach(const ServiceChannel &sc, _services){
        if(sc.uuid() == uuid){
            int indexSC = _services.indexOf(sc);
            if(indexSC > -1){
                beginRemoveRows(QModelIndex(), indexSC, indexSC);
                channel = sc.channel();
                _services.removeOne(sc);
                endRemoveRows();
                break;
            }
        }
    }
    return channel;
}

int ServicesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _services.count();
}

QVariant ServicesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > this->_services.count())
        return QVariant();
    const ServiceChannel &service = this->_services[index.row()];
    if (role == ChannelRole)
        return service.channel();
    else if (role == TypeRole)
        return service.type();
    else if (role == UUidRole)
        return service.uuid();
    return QVariant();
}

QHash<int, QByteArray> ServicesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ChannelRole] = "channel";
    roles[TypeRole] = "type";
    roles[UUidRole] = "uuid";
    return roles;
}
