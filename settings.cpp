#include "settings.h"
Settings::Settings(QString path, QSettings::Format fileType, QObject *parent) : QObject(parent)
{
    _settings = new QSettings(path,  fileType);

}

Settings::~Settings()
{
    _settings->deleteLater();
}

void Settings::setValue(const QString &key, const QVariant &value, const QString &group)
{
    if(!group.isEmpty()){
        _settings->beginGroup(group);
        _settings->setValue(key, value);
        _settings->endGroup();
    }else{
        _settings->setValue(key, value);
    }
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue, const QString &group) const
{
    if(!group.isEmpty()){
        _settings->beginGroup(group);
        const QVariant result = _settings->value(key, defaultValue);
        _settings->endGroup();
        return result;
    }else{
        return _settings->value(key, defaultValue);
    }
}

void Settings::beginGroup(const QString &group)
{
    _settings->beginGroup(group);
}

void Settings::endGroup()
{
    _settings->endGroup();
}

void Settings::remove(const QString &key, const QString &group)
{
    if(!group.isEmpty()){
        _settings->beginGroup(group);
        _settings->remove(key);
        _settings->endGroup();
    }else{
        _settings->remove(key);
    }
}

const QStringList Settings::readGroupKeys(QString group)
{
    _settings->beginGroup(group);
    const QStringList childKeys = _settings->childKeys();
    _settings->endGroup();
    return childKeys;
}

void Settings::removeValue(const QString &key)
{
    _settings->remove(key);
}

QSettings *Settings::settings() const
{
    return _settings;
}

void Settings::setSettings(QSettings *settings)
{
    _settings = settings;
}

QVariant Settings::readMessagesPort()
{
    return value("port", 12345, "MessagesSocket");
}

void Settings::writeMessagesPort(QVariant port)
{
    _settings->beginGroup("MessagesSocket");
    _settings->setValue("port", port.toInt());
    _settings->endGroup();
    emit messagesPortChanged(port);
}
