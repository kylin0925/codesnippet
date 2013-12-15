#ifndef HIGHTLIGHT_H
#define HIGHTLIGHT_H
#include <QSyntaxHighlighter>

class QTextDocument;

class hightlight : public QSyntaxHighlighter
{
    Q_OBJECT
private:
    QStringList keywordPatterns;
protected:
     void highlightBlock(const QString &text);

public:
    hightlight(QTextDocument *parent = 0);
};

#endif // HIGHTLIGHT_H
