#ifndef CLEARSCREEN_H
#define CLEARSCREEN_H

#include "abstractcommand.h"

class ClearScreen : public AbstractCommand
{
public:
    ClearScreen(QObject *parent = 0);
    virtual QString serialize();
    virtual void execute(QImage *);
    virtual QString name();

protected:

};


#endif // CLEARSCREEN_H
