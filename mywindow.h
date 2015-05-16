#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QToolBar>
#include "network.h"
#include "sendboard.h"
#include "receiveboard.h"

class MyWindow : public QMainWindow
{
    Q_OBJECT
public:
    MyWindow(QWidget *parent = 0);
    ~MyWindow();
    Network* network();

protected:
    QToolBar *m_toolbar;
    QMdiArea *m_mdi;
    Sendboard *m_sendboard;
    Receiveboard *m_receiveboard;

    // Network layer
    Network *m_network;
    
signals:

    
public slots:

    
};

#endif // MYWINDOW_H
