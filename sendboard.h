#ifndef SENDBOARD_H
#define SENDBOARD_H

#include <QWidget>
#include <QImage>

#include <QMouseEvent>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>

#include "defines.h"
#include "manager.h"

class Sendboard : public QWidget
{
    Q_OBJECT
public:
    Sendboard(QWidget *parent = 0);
    Manager *manager(){ return m_manager; }

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

protected:
    Manager *m_manager;
    QImage *m_image;

signals:
    void mousePressed(QMouseEvent *);
    void mouseReleased(QMouseEvent *);
    void mouseMoved(QMouseEvent *);

public slots:
    void onImageModified();

};

#endif // SENDBOARD_H
