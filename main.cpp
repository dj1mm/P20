#include <QApplication>

#include <pthread.h>
#include <wiringPi.h>
#include <QThread>

#include "window.h"
#include "mywindow.h"
#include "network.h"
#include "physical.h"
#include "defines.h"

bool cable_R; // Just for the sake of 3.4 shared bool variables: TO REMOVE FOR 3.5
bool cable_M;

pthread_mutex_t changingCableMutex;
pthread_cond_t cableChangedCondition;

void* send(void* argument)
{

    Physical *m_physical = new Physical;
    Network *m_network = (Network *) argument;

    qDebug() << "main.h/send: Send thread started.";

    // do something....
    QVector<bool> bits; // hold the bits

    // forever loop
    for(int i = 0;;i++){
        bits = m_network->getSend();

        // do stuffs with bits
        qDebug() << "";
        qDebug() << "main.cpp/send: Sending command #" << i;

        if(bits.isEmpty()) continue;

        // qDebug()<< "main.cpp/send: Sending" << bits;
        while(true){
            if(m_physical->send(bits) == true) break;

            // not succesful

            qDebug() << "main.cpp/send: Sending command #" << i << "not successful. Retrying";
            Physical::sleep(1000000);
        }

        bits.clear();

        // delay 0.5 s to get enoough time to read
        Physical::sleep(1000000);

        qDebug() << "main.cpp/send: Bits sent ok";
    }

    // end thread
    pthread_exit(NULL);
}

void* read(void* argument)
{

    Physical *m_physical = new Physical;
    Network *m_network = (Network *) argument;

    qDebug() << "main.h/read: Read thread started";

    // do something...
    QVector<bool> bits;
    for(int i = 0;;i++){
        bits = m_physical->read();
        if(bits.isEmpty()){
            qDebug() << "main.cpp/read: Read unsuccessful. Timed out";
            continue;
        }

        Physical::sleep(100000);

        // qDebug()<< "main.cpp/read: Reading" << bits;

        qDebug() << "main.cpp/read: Reading command #" << i;
        // if got stuffs read

        // tell the Network obj that we got something so that it can do whatever it wants
        QMetaObject::invokeMethod(m_network, "gotData", Qt::QueuedConnection, Q_ARG(QVector<bool>, bits));
        // delay(200)
        // delayMicroseconds(500000);
        // usleep(50);
    }

    // end thread
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    // setup GPIO interface - uncomment when needed
    // needs to run with root via sudo in terminal.
    wiringPiSetup();
    pinMode(Physical::OM0, OUTPUT);
    pinMode(Physical::OM1, OUTPUT);
    pinMode(Physical::OM2, OUTPUT);
    pinMode(Physical::OM3, OUTPUT);
    pinMode(Physical::OM4, OUTPUT);
    pinMode(Physical::OM5, OUTPUT);
    pinMode(Physical::OM6, OUTPUT);
    pinMode(Physical::ORL, OUTPUT);
    pinMode(Physical::OSL, OUTPUT);
    pinMode(Physical::OAL, OUTPUT);

    pinMode(Physical::IM0, INPUT);
    pinMode(Physical::IM1, INPUT);
    pinMode(Physical::IM2, INPUT);
    pinMode(Physical::IM3, INPUT);
    pinMode(Physical::IM4, INPUT);
    pinMode(Physical::IM5, INPUT);
    pinMode(Physical::IM6, INPUT);
    pinMode(Physical::IRL, INPUT);
    pinMode(Physical::ISL, INPUT);
    pinMode(Physical::IAL, INPUT);

    digitalWrite(Physical::OM0, false); // defaulted to false
    digitalWrite(Physical::OM1, false); // defaulted to false
    digitalWrite(Physical::OM2, false); // defaulted to false
    digitalWrite(Physical::OM3, false); // defaulted to false
    digitalWrite(Physical::OM4, false); // defaulted to false
    digitalWrite(Physical::OM5, false); // defaulted to false
    digitalWrite(Physical::OM6, false); // defaulted to false
    digitalWrite(Physical::ORL, false); // in case turn off prgm before it returend to false
    digitalWrite(Physical::OSL, false);
    digitalWrite(Physical::OAL, false); // defaulted to f

    pthread_mutex_init(&changingCableMutex, NULL);
    pthread_cond_init(&cableChangedCondition, NULL); // Just for the sake of 3.4 shared bool variables: TO REMOVE FOR 3.5

    qRegisterMetaType<QVector<bool> >("QVector<bool>");

    // setup Qt GUI
    QApplication a(argc, argv);
    MyWindow w;
    w.show();

    // starting worker thread(s)
    int rc;
    pthread_t send_thread;
    rc = pthread_create(&send_thread, NULL, send, (void*) w.network());
    if (rc) {
        qDebug() << "main.h/main: Unable to start send thread.";
        exit(1);
    }
    pthread_t read_thread;
    rc = pthread_create(&read_thread, NULL, read, (void*) w.network());
    if (rc) {
        qDebug() << "main.h/main: Unable to start read thread.";
        exit(1);
    }

    // start window event loop
    qDebug() << "main.h/main: Starting event loop...";
    int ret = a.exec();
    qDebug() << "main.h/main: Event loop stopped.";

    // cleanup pthreads
    // pthread_exit(NULL);

    // exit
    return ret;
}
