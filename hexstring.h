#ifndef HEXSTRING_H
#define HEXSTRING_H

#include <QString>



class HexString
{
public:
    HexString();
    static QByteArray stringToHex(const QString str);
    static QString hexToString(const QByteArray hex);
};

#endif // HEXSTRING_H
