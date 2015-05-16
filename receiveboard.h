#ifndef RECEIVEBOARD_H
#define RECEIVEBOARD_H

#include <QWidget>
#include <QImage>

#include <QResizeEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>

#include "defines.h"
#include "manager.h"

class Receiveboard : public QWidget
{
    Q_OBJECT
public:
    Receiveboard(QWidget *parent = 0);
    Manager *manager(){ return m_manager; }

    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

protected:
    Manager *m_manager;
    QImage *m_image;

signals:

public slots:
    void onImageModified();
    
};

#endif // RECEIVEBOARD_H
