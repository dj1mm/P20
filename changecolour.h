#ifndef CHANGECOLOUR_H
#define CHANGECOLOUR_H

#include "abstractcommand.h"
#include <QColor>

class ChangeColour : public AbstractCommand
{
    Q_OBJECT
public:
    ChangeColour(QObject *parent = 0);
    virtual QString serialize();
    virtual void execute(QImage *);
    virtual QString name();

    void setColour(QColor);

protected:
    QColor m_colour;
    
signals:
    
public slots:
    
};

#endif // CHANGECOLOUR_H
