#include "network.h"

Network::Network(QObject *parent): QObject(parent)
{
    qDebug() << "Network::Network(): Initializing network layer (pthread mutex and condition)";

    pthread_mutex_init(&m_mutex, NULL);
    pthread_cond_init(&m_condition, NULL);
}

Network::~Network()
{
    qDebug() << "Network::~Network(): Destructor called";

    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_condition);
}

QVector<bool> Network::getSend(){
    //lock mutex
    pthread_mutex_lock(&m_mutex);

    // w8 for condition, and then..
    // pthread_cond_wait(&m_condition, &m_mutex); Now Network::getSend() will be non blocking

    //if queue is not empty do something?
    QVector<bool> bits;
    while(m_toSend.isEmpty()){
        pthread_mutex_unlock(&m_mutex);

        // In this time, Network::send() will get time to lock unlock and add to m_toSend

        pthread_mutex_lock(&m_mutex);
    }

    bits = m_toSend.dequeue();

#ifdef DEBUG_NETWORK
    qDebug() << "Network::getSend(): bits are being dequeued and placed in vector";
#endif

    //unlock mutex
    pthread_mutex_unlock(&m_mutex);

    // return the data to send
    return bits;
}

void Network::send(QVector<bool> bits)
{

#ifdef DEBUG_NETWORK
    qDebug() << "Network:;send(): bit qvector are going to be enqueued to be sent";
#endif

    // lock mutex
    pthread_mutex_lock(&m_mutex);

    // do queue.append()
    m_toSend.enqueue(bits);

    // signal condition
    // pthread_cond_signal(&m_condition);

    // unlock mutex
    pthread_mutex_unlock(&m_mutex);
}
