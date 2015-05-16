#include "drawline.h"

DrawLine::DrawLine(QObject *parent): AbstractCommand(parent)
{
}

QString DrawLine::serialize()
{
    return QString(name() + " " + QString::number(m_from.x()) + "," + QString::number(m_from.y()) + " " + QString::number(m_to.x()) + "," + QString::number(m_to.y()));
}

void DrawLine::execute(QImage *image)
{
#ifdef DEBUG_APPLICATION
    qDebug() << "DrawLine:;execute(): Drawing line";
#endif

    QPainter painter(image);

    QPen pen;
    pen.setWidth(5);
    pen.setColor(m_pencolour);
    painter.setPen(pen);

    painter.drawLine(m_from, m_to);

    setFrom(m_to);

}

QString DrawLine::name(){
    return "d";
}

void DrawLine::setFrom(QPoint from){
    m_from = from;
}

void DrawLine::setTo(QPoint to){
    m_to = to;
}

void DrawLine::setPenColour(QColor colour)
{
    m_pencolour = colour;
}
