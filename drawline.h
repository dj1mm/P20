#ifndef DRAWLINE_H
#define DRAWLINE_H

#include <QPoint>
#include "abstractcommand.h"

class DrawLine : public AbstractCommand
{
public:
    DrawLine(QObject *parent = 0);
    virtual QString serialize();
    virtual void execute(QImage *);
    virtual QString name();

    void setFrom(QPoint);
    void setTo(QPoint);
    void setPenColour(QColor);

protected:
    QPoint m_from;
    QPoint m_to;
    QColor m_pencolour;

};

#endif // DRAWLINE_H
