#include "styleparser.h"

#include <QFile>
#include <qtextstream.h>

StyleParser::StyleParser()
{

}

void StyleParser::parseStyle(QString path)
{
    QFile file(path);
    QTextStream in(&file);
    QString css;
    while (!in.atEnd()) {
        css.append(in.readLine());
    }
}
