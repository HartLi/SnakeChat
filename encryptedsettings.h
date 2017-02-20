#ifndef ENCRYPTEDSETTINGS_H
#define ENCRYPTEDSETTINGS_H

#include <QObject>
#include "simplecrypt.h"
#include <QSettings>

class EncryptedSettings : public QObject
{
    Q_OBJECT
public:
    EncryptedSettings(QString key, QString path, QSettings::Format fileType, QObject *parent);
    ~EncryptedSettings();
    Q_INVOKABLE void setValue(const QString & key, const QVariant & value);
    Q_INVOKABLE QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    Q_INVOKABLE void beginGroup(const QString &group);
    Q_INVOKABLE void endGroup();
private:
    QSettings *_settings;
    QByteArray _hexKey;
    SimpleCrypt *_crypter;
};

#endif // ENCRYPTEDSETTINGS_H
