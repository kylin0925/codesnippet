#include <QtGui>
#include "hightlight.h"

//http://harmattan-dev.nokia.com/docs/library/html/qt4/richtext-syntaxhighlighter-highlighter-cpp.html
hightlight::hightlight(QTextDocument  *parent)
    :QSyntaxHighlighter(parent)
{
     //QStringList keywordPatterns;
     keywordPatterns<<"\\bfor\\b"  <<"\\bdo\\b"   <<"\\bwhile\\b"
                    <<"\\bint\\b"  <<"\\bchar\\b" <<"\\bfloat\\b"<<"\\bdouble\\b"<<"\\bvoid\\b"
                    <<"\\bauto\\b" <<"\\bstatic\\b"<<"\\bregister\\b"
                    <<"\\bif\\b"   <<"\\belse\\b"<<"\\bswitch\\b"<<"\\bcase\\b"
                    <<"\\bconst\\b";
}
void hightlight::highlightBlock(const QString &text)
{
    QRegExp expression("");
    QTextCharFormat format;
    format.setForeground(Qt::darkBlue);
    format.setFontWeight(QFont::Bold);
    foreach(const QString &pat,keywordPatterns){
        expression.setPattern(pat);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, format);
            index = expression.indexIn(text, index + length);
        }
    }
    //comment
    QRegExp cmtS("/\\*");
    QRegExp cmtE("\\*/");
    format.setFontItalic(true);
    format.setForeground(Qt::darkGreen);
    int start=0,end=0,cmtlen;
    setCurrentBlockState(0);
    if(previousBlockState()!=1)
        start = cmtS.indexIn(text);

    //qDebug()<<"start "<<start;
    while(start>=0)
    {
        end = cmtE.indexIn(text,start);
        //qDebug()<<"end "<<end;
        if(end == -1)
        {
            setCurrentBlockState(1);
            cmtlen = text.length() - start;
        }
        else
        {
            cmtlen =  end - start + cmtE.matchedLength();
        }
        setFormat(start,cmtlen,format);
        start = cmtS.indexIn(text,start+cmtlen);
    }
}
