#ifndef CTIMER_H
#define CTIMER_H

#include <QObject>
#include <QTimer>
#include <QLabel>
class cTimer : public QTimer
{
public:
    cTimer();
    void setLabe(QLabel lbl);
    void update();

};

#endif // CTIMER_H
