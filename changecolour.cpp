#include "changecolour.h"

ChangeColour::ChangeColour(QObject *parent): AbstractCommand(parent)
{
}

QString ChangeColour::serialize()
{
    return name() + " " + QString::number(m_colour.red()) + " " + QString::number(m_colour.green()) + " " + QString::number(m_colour.blue());
}

void ChangeColour::execute(QImage *image)
{
#ifdef DEBUG_APPLICATION
    qDebug() << "ChangeColour:;execute(): Changing colour!!!";
#endif
}

QString ChangeColour::name(){
    return "k"; // for kolor!
}

void ChangeColour::setColour(QColor colour)
{
    m_colour = colour;
}
