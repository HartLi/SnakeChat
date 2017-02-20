#include "encryptedsettings.h"
#include "hexstring.h"
#include <QDebug>


EncryptedSettings::EncryptedSettings(QString key, QString path, QSettings::Format fileType, QObject *parent)
{
    _settings = new QSettings(path,  fileType);
    _hexKey = HexString::stringToHex(key);
    _crypter = new SimpleCrypt(_hexKey.toInt(), parent);
//    QString raw = "My head id going crazy!!!";
//    QString cr1 = _crypter->encryptToString(raw);
//    QString cr2 = _crypter->encryptToString(raw);
//    QString cr3 = _crypter->encryptToString(raw);
//    qDebug() << _crypter->decryptToString(cr1);
//    qDebug() << _crypter->decryptToString(cr2);
//    qDebug() << _crypter->decryptToString(cr3);
//    qDebug() << _hexKey << endl;
//    this->beginGroup("Twitch");
//    qDebug() << this->value("twitch_username").toString() << endl;
//    qDebug() << this->value("twitch_token").toString() << endl;
//    this->endGroup();
}

EncryptedSettings::~EncryptedSettings()
{
    _settings->deleteLater();
    _crypter->deleteLater();
}

void EncryptedSettings::setValue(const QString &key, const QVariant &value)
{
    QString enValue = _crypter->encryptToString(value.toString());
    _settings->setValue(key, enValue);
}

QVariant EncryptedSettings::value(const QString &key, const QVariant &defaultValue) const
{
    QString enDefaultValue = _crypter->encryptToString(defaultValue.toString());
    QString enValue = _settings->value(key, enDefaultValue).toString();
    return _crypter->decryptToString(enValue);
}

void EncryptedSettings::beginGroup(const QString &group)
{
    _settings->beginGroup(group);
}

void EncryptedSettings::endGroup()
{
    _settings->endGroup();
}

