#include "physical.h"

/*
    
    Synchronization code idea inspired from http://www.codeproject.com/Articles/828476/PiCom-PiFTP-a-GPIO-communication-protocol-for-the ..
    From idea to use a clocked line, of az1g14

    QUOTE: 
    The sync-function then boils down to 2 simple statements
        Write the current value to the current output-line (SLO1 or SLO2).
        Wait for the current input-line (SLI1 or SLI2) to assume the current value.

*/
Physical::Physical(QObject *parent): QObject(parent)
{
}

bool Physical::send(QVector<bool> bits)
{

#ifdef DEBUG_PHYSICAL
    qDebug() << "Physical::send(): Physical layer sending bits stream";
#endif

    // send Ready signal <- We will now be sending data, plz listen
    send(ORL, true);

    for(int i = 0; i < ceil(bits.size()/BITLENGTH); i++){

#ifdef DEBUG_PHYSICAL
        qDebug() << "";

        qDebug() << "Physical::send(): Sending solicitation and data";

        qDebug() << "Physical::send(): Sending " << i << true << bits[i * BITLENGTH + 0] << bits[i * BITLENGTH + 1] << bits[i * BITLENGTH + 2] << bits[i * BITLENGTH + 3] << bits[i * BITLENGTH + 4] << bits[i * BITLENGTH + 5] << bits[i * BITLENGTH + 6];
#endif

        send(OM0, bits[i * BITLENGTH + 0]); // cable_M = bits[i];
        send(OM1, bits[i * BITLENGTH + 1]); // cable_M = bits[i];
        send(OM2, bits[i * BITLENGTH + 2]); // cable_M = bits[i];
        send(OM3, bits[i * BITLENGTH + 3]); // cable_M = bits[i];
        send(OM4, bits[i * BITLENGTH + 4]); // cable_M = bits[i];
        send(OM5, bits[i * BITLENGTH + 5]); // cable_M = bits[i];
        send(OM6, bits[i * BITLENGTH + 6]); // cable_M = bits[i];

        send(ORL, true);

        // solicite receiver to read data. If not successful

        if(!this->solicite()){ 
            // send ready signal to false

#ifdef DEBUG_PHYSICAL
            qDebug() << "Physical::send(): Solicitation to read data is not successful. Time out sending data. Returning false";
#endif

            send(ORL, false);

            return false;
        }

#ifdef DEBUG_PHYSICAL
        qDebug() << "Physical::send(): Data sent ok. Sending terminating solicitation and waiting for ack";
#endif

        // solicite user whether can send next data. If is successful

        if(this->solicite()){

#ifdef DEBUG_PHYSICAL
            qDebug() << "Physical::send(): Solicitation successful. Sending next bit if any";
#endif

            // continue;
        }else{

#ifdef DEBUG_PHYSICAL
            qDebug() << "Physical::send(): Solicitation to send next data not successful. Time out sending data. Returning false";
#endif

            send(ORL, false);

            return false;
        }
    }

    send(ORL, false);

#ifdef DEBUG_PHYSICAL
    qDebug() << "Physical::send(): Physical layer bits stream sent";
#endif

    return true;

}

QVector<bool> Physical::read()
{

#ifdef DEBUG_PHYSICAL
    qDebug() << "Physical::read(): Physical layer reading bits stream";
#endif

    QVector<bool> bits;
    int i = 0;

    // while Ready signal = false, just hang on there

    for(;;){

        bool r = read(Physical::IRL); // r is the ready signal

        if(!r && bits.isEmpty()) continue;

        if(!r) break; // if ready is false but got data in our bits buffer, this means that send/receive is done

        if(!this->solicited()){ // solicited to read the data. If is not successful
            // timed out but was it because we finished sending data ?
            if(!read(Physical::IRL)) break;

#ifdef DEBUG_PHYSICAL
            qDebug() << "Physical::read(): Terminating solicitation taking too much time. Dont wanna w8. Time out. Returning empty bits";
#endif

            bits.clear();
            return bits;
        }

#ifdef DEBUG_PHYSICAL
        qDebug() << "";

        qDebug() << "Physical::read(): Solicited ok ! Reading data";
#endif

        bool m0 = read(IM0);     //bool m = cable_M;
        bool m1 = read(IM1);
        bool m2 = read(IM2);
        bool m3 = read(IM3);
        bool m4 = read(IM4);
        bool m5 = read(IM5);
        bool m6 = read(IM6);

#ifdef DEBUG_PHYSICAL
        qDebug() << "Physical::read(): Reading " << i << r << m0 << m1 << m2 << m3 << m4 << m5 << m6;

        qDebug() << "Physical::read(): Data read. Waiting for terminating solicitation";
#endif

        bits.push_back(m0);
        bits.push_back(m1);
        bits.push_back(m2);
        bits.push_back(m3);
        bits.push_back(m4);
        bits.push_back(m5);
        bits.push_back(m6);

        i++;

        if(this->solicited()){ // finished reading the data ?
#ifdef DEBUG_PHYSICAL
            qDebug() << "Physical::read(): Terminating solicitation received. Now waiting for next bit if any";
#endif

        }else{ // timedout
#ifdef DEBUG_PHYSICAL
            qDebug() << "Physical::read(): Terminating solicitation taking too much time. Dont wanna w8. Time out. Returning empty bits";
#endif

            bits.clear();
            return bits;
        }
    }

#ifdef DEBUG_PHYSICAL
    qDebug() << "Physical::read(): Physical layer bits stream read ok";
#endif

    return bits;
}

// Protected functions to handle communication

void Physical::send(Pins pin, bool bit)
{
    if(pin == Physical::ORL){
        // cable_R = bit;
    }else if(pin == Physical::OM0){
        // cable_M = bit;
    }else if(pin == Physical::OM1){
        // cable_M = bit;
    }else if(pin == Physical::OM2){
        // cable_M = bit;
    }else if(pin == Physical::OM3){
        // cable_M = bit;
    }else if(pin == Physical::OM4){
        // cable_M = bit;
    }else if(pin == Physical::OM5){
        // cable_M = bit;
    }else if(pin == Physical::OM6){
        // cable_M = bit;
    }else{ // should not be writing to input pins so...
#ifdef DEBUG_PHYSICAL
        qWarning() << "Physical::send(): Tried to send data from unknown pin, cancelling";
#endif

        return;
    }

    digitalWrite(pin, bit ? HIGH : LOW);
}

bool Physical::read(Pins pin)
{
    if(pin == Physical::IRL){
        // return cable_R;
    }else if(pin == Physical::IM0){
        // return cable_M;
    }else if(pin == Physical::IM1){
        // return cable_M;
    }else if(pin == Physical::IM2){
        // return cable_M;
    }else if(pin == Physical::IM3){
        // return cable_M;
    }else if(pin == Physical::IM4){
        // return cable_M;
    }else if(pin == Physical::IM5){
        // return cable_M;
    }else if(pin == Physical::IM6){
        // return cable_M;
    }else{
#ifdef DEBUG_PHYSICAL
        qWarning() << "Physical::read(): Tried to read from unknown pin, cancelling. Returning false";
#endif

        return false;
    }
    return digitalRead(pin);
}

void Physical::sleep(int microseconds)
{
    delayMicroseconds(microseconds);
}

void Physical::acknowledge(bool value)
{
    digitalWrite(Physical::OAL, value);
}

bool Physical::acknowledged(bool value)
{
    int trial = PHYSICAL_DEFAULT_NUMBER_OF_TRIALS;
    while(digitalRead(Physical::IAL) != value){
        sleep(1000);
        trial--;
        if(trial < 0) return false;
    }

    return true; // Else if ack-ed
}

bool Physical::solicite() // solicite() should return bools. true = successful, false = failed
{
    static bool value = false;
    
    value ^= true;

    qDebug() << "Physical::solicite(): Sending " << value;

    digitalWrite(Physical::OSL, value);

    // sending req, w8 for acknowledgement

    if(this->acknowledged(value)){
        return true;
    }

    // timedout.. too bad

    return false;
    
}

bool Physical::solicited(int trial)
{
    static bool value = false;

    // int trial = 100;
    while(digitalRead(Physical::ISL) == value){
        sleep(1000);
        trial--;
        if(trial < 0) return false; // timedout
    }

    // got req, sending ack

    value = digitalRead(Physical::ISL);

    this->acknowledge(digitalRead(Physical::ISL));

    return true;
}
