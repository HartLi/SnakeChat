#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QObject>


class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant messagesPort READ readMessagesPort WRITE writeMessagesPort NOTIFY messagesPortChanged)
public:
    Settings(QString path, QSettings::Format fileType, QObject *parent = nullptr);
    ~Settings();
    Q_INVOKABLE void setValue(const QString & key, const QVariant & value, const QString &group = "");
    Q_INVOKABLE QVariant value(const QString &key, const QVariant &defaultValue = QVariant(), const QString &group = "") const;
    Q_INVOKABLE void beginGroup(const QString &group);
    Q_INVOKABLE void endGroup();
    Q_INVOKABLE void remove(const QString &key, const QString &group = "");
    const QStringList readGroupKeys(QString group);
    void removeValue(const QString &key);
    QSettings *settings() const;
    void setSettings(QSettings *settings);
    Q_INVOKABLE QVariant readMessagesPort();
    Q_INVOKABLE void writeMessagesPort(QVariant port);
private:
    QSettings *_settings;
Q_SIGNALS:
    void messagesPortChanged(QVariant port);
};

#endif // SETTINGS_H
