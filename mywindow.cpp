#include "mywindow.h"

MyWindow::MyWindow(QWidget *parent): QMainWindow(parent)
{
    setWindowTitle("Whiteboard");
    m_toolbar = new QToolBar;
    addToolBar(m_toolbar);

    m_mdi = new QMdiArea;

    m_sendboard = new Sendboard;
    m_sendboard->manager()->setToolbarAware(m_toolbar);

    m_mdi->addSubWindow(m_sendboard);

    m_receiveboard = new Receiveboard;
    m_mdi->addSubWindow(m_receiveboard);

    setCentralWidget(m_mdi);

    m_network = new Network(this);

    // connect(m_sendboard->manager(), SIGNAL(send(QVector<bool>)), m_receiveboard->manager(), SLOT(read(QVector<bool>)));
    connect(m_sendboard->manager(), SIGNAL(send(QVector<bool>)), m_sendboard->manager(), SLOT(read(QVector<bool>)));
    connect(m_sendboard->manager(), SIGNAL(send(QVector<bool>)), m_network, SLOT(send(QVector<bool>)));
    connect(m_network, SIGNAL(gotData(QVector<bool>)), m_receiveboard->manager(), SLOT(read(QVector<bool>)));
    // connect(m_network, SIGNAL(gotData(QVector<bool>)), this, SLOT(wa(QVector<bool>)));
}

MyWindow::~MyWindow()
{
    qDebug() << "MyWindow::~MyWindow(): Destructor called";
}

Network* MyWindow::network()
{
    return m_network;
}
