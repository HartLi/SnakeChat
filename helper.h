#ifndef HELPER_H
#define HELPER_H

#include <QString>



class Helper
{
public:
    Helper();
    static QMap<QString, QString> const matchUrl(QString word);
    static int getMaxValue(QList<int> values);
    static bool checkUrl(QString url);
    static const QString argb2rgba(QString hex);
};

#endif // HELPER_H
