#include "hexstring.h"
#include <QString>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <QDebug>
HexString::HexString()
{

}

QByteArray HexString::stringToHex(const QString str){
    return QByteArray::fromStdString(str.toStdString()).toHex();
}

QString HexString::hexToString(const QByteArray hex){
    return QString::fromStdString(hex.toStdString());
}
