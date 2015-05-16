#include "clearscreen.h"

ClearScreen::ClearScreen(QObject *parent): AbstractCommand(parent)
{
}

QString ClearScreen::serialize()
{
    return name();
}

void ClearScreen::execute(QImage *image)
{
#ifdef DEBUG_APPLICATION
    qDebug() << "ClearScreen:;execute(): Clearing screen";
#endif

    image->fill(QColor(255, 255, 255));
}

QString ClearScreen::name(){
    return "c";
}
