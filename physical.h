#ifndef PHYSICAL_H
#define PHYSICAL_H

#include <QObject>
#include <QVector>
#include <wiringPi.h>
#include <pthread.h> // Just for the sake of 3.4 shared bool variables: TO REMOVE FOR 3.5
#include <sys/time.h>
#include <math.h>
#include "defines.h"

#define PHYSICAL_DEFAULT_NUMBER_OF_TRIALS 5 // should really be in defines.h

// extern bool cable_R; // Just for the sake of 3.4 shared bool variables: TO REMOVE FOR 3.5
// extern bool cable_M;

extern pthread_mutex_t changingCableMutex;
extern pthread_cond_t cableChangedCondition;

class Physical : public QObject
{
    Q_OBJECT
public:
    Physical(QObject *parent = 0);
    bool send(QVector<bool>);
    QVector<bool> read();

    static void sleep(int);

    // IRL means Input Ready Line
    // IML means Input Message Line
    // ORL means Output Ready Line
    // OML means Output Message Line
    enum Pins{

        IRL = 7, ORL = 15, // GPIO pin id 7 8
        IM0 = 0, OM0 = 1,  // 11 12
        IM1 = 3, OM1 = 4,   // 15 16
        IM2 = 13, OM2 = 6, // 21 22
        IM3 = 14, OM3 = 10, // 23 24
        IM4 = 30, OM4 = 31, // 27 28
        IM5 = 22, OM5 = 26, // 31 32
        IM6 = 24, OM6 = 27, // 35 36

        IAL = 25, OAL = 28,
        ISL = 8, OSL = 9
    };

signals:
    
public slots:

protected:
    void send(Pins, bool);
    bool read(Pins);

    // bool connect();
    void acknowledge(bool);
    bool acknowledged(bool); // w8 for an ack from partner

    bool solicite();
    bool solicited(int trial = PHYSICAL_DEFAULT_NUMBER_OF_TRIALS);
    
};

#endif // PHYSICAL_H
