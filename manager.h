#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QImage>
#include <QMouseEvent>
#include <string>
#include <QStringList>
#include <QToolBar>
#include <QAction>
#include <sstream>
#include <bitset>
#include <QChar>
#include <QColorDialog>
#include <QColor>

#include "abstractcommand.h"
#include "drawline.h"
#include "clearscreen.h"
#include "changecolour.h"
#include "defines.h"

class Manager : public QObject
{
    Q_OBJECT
public:
    Manager(QObject *parent = 0);
    void setImage(QImage *);
    QVector<bool> serialize();
    void deserialize(QVector<bool>);
    void setToolbarAware(QToolBar *);

protected:
    QImage *m_image;
    QToolBar *m_toolbar;
    QColor m_colour;

    AbstractCommand *m_current;
    DrawLine *m_drawline;
    QAction *m_drawlineAction;
    ClearScreen *m_clearscreen;
    QAction *m_clearscreenAction;
    ChangeColour *m_changecolour;
    QAction *m_changecolourAction;


signals:
    void imageModified();
    void send(QVector<bool>); // QVector contains the bitstream corresponding to the sent command
    // void onDrawlineActionClick()
    
public slots:
    void onMousePressed(QMouseEvent *);
    void onMouseReleased(QMouseEvent *);
    void onMouseMoved(QMouseEvent *);
    void onDrawlineActionTriggered();
    void onClearscreenActionTriggered();
    void onChangeColourActionTriggered();

    void read(QVector<bool>); // Will get the command eg.drawline -1,2 3,-4 , and will execute the current cmd
    
};

#endif // MANAGER_H
