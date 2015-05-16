#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QVector>
#include <QQueue>
#include <pthread.h>
#include "defines.h"

/*
 * Code greatly inspired from https://juanchopanzacpp.wordpress.com/2013/02/26/concurrent-queue-c11/;
 */
class Network : public QObject
{
    Q_OBJECT
public:
    Network(QObject *parent = 0);
    ~Network();
    QVector<bool> getSend();

protected:
    QQueue<QVector<bool> > m_toSend;
    pthread_mutex_t m_mutex;
    pthread_cond_t m_condition; // GIVE MORE EXPLICIT NAMES LATER

signals:
    void gotData(QVector<bool>);
    
public slots:
    void send(QVector<bool>);
    
};

#endif // NETWORK_H
