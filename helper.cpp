#include "helper.h"
#include <QRegularExpression>
#include <QUrl>
#include <string>
#include <regex>
#include <QDebug>
#include <iostream>
using namespace std;
Helper::Helper()
{

}

int Helper::getMaxValue(QList<int> values)
{
    return *std::max_element(values.begin(), values.end());
}

QMap<QString, QString> const Helper::matchUrl(QString text)
{
    bool foundmatch = false;
    QMap<QString, QString> matchResult;
//    qDebug() << text;
    string subject = text.toStdString();
    try {
        std::regex re(R"((?![^<]*>|[^<>]*<\/)((https?:\/\/)?(www.)?([\w.-]+)(\.[a-z]{2,6}\.?)(\/[\w.]*)*\/?\S*)(?![^<]*>|[^<>]*<\/))", std::regex_constants::icase);
        std::smatch match;
        foundmatch = std::regex_search(subject, match, re);
        if(foundmatch && match.size() > 1){
            matchResult.insert(QString::fromStdString(match.str(4)+match.str(5)), QString::fromStdString(match.str(0)));
        }else{
        }
    }  catch (exception& e) {
        qDebug() << e.what();
    }
//    qDebug() << foundmatch;

//   bool matched = regex_match(text.toStdString(), rgx);
   //   QRegularExpression re(pattern, QRegularExpression::DotMatchesEverythingOption);
//   QRegularExpressionMatch match = re.match(word);
//   qDebug() << QString::fromStdString(result);
   return matchResult;
}

bool Helper::checkUrl(QString url)
{
    bool result = false;
//    QUrl *url = new QUrl(word);
//    if(url->isValid())
//        result = true;
//    delete url;

    return result;
}

const QString Helper::argb2rgba(QString hex)
{
    hex = hex.replace("#","");
    uint8_t alpha;
    uint8_t red;
    uint8_t blue;
    uint8_t green;
    if(hex.length() > 6){
        bool ok;
        alpha = (hex.mid(0,2).toUInt(&ok,16));
        red =   hex.mid(2,2).toUInt(&ok,16);
        green = hex.mid(4,2).toUInt(&ok,16);
        blue =  hex.mid(6,6).toUInt(&ok,16);
    }
    else{
        bool ok;
        alpha = 255;
        red =   hex.mid(0,2).toUInt(&ok,16);
        green = hex.mid(2,2).toUInt(&ok,16);
        blue =  hex.mid(4,6).toUInt(&ok,16);
    }
    return QString("rgba(%1, %2, %3, %4)").arg(red).arg(green).arg(blue).arg((double)alpha/255);
}
