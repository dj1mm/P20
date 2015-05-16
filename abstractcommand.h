#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <QObject>
#include <QString>
#include <QImage>
#include <QPainter>
#include <QPen>
#include "defines.h"

class AbstractCommand : public QObject
{
    Q_OBJECT
public:
    AbstractCommand(QObject *parent = 0);
    virtual QString serialize() = 0;
    virtual void execute(QImage *) = 0;
    virtual QString name() = 0;
    
signals:
    
public slots:
    
};

#endif // ABSTRACTCOMMAND_H
