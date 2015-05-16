#include "manager.h"

Manager::Manager(QObject *parent): QObject(parent)
{
    m_colour = Qt::black; // default colour is black

    m_drawline = new DrawLine;
    m_drawline->setPenColour(m_colour);

    m_clearscreen = new ClearScreen;
    m_changecolour = new ChangeColour;

    // drawline is default command
    m_current = m_drawline;
}

void Manager::setImage(QImage *image)
{
    m_image = image;
}

QVector<bool> Manager::serialize()
{
    QString serial;
    if(m_current->name() == m_drawline->name()){
#ifdef DEBUG_MANAGER
        qDebug() << "Manager::serialize(): drawLine:" << m_current->serialize();
#endif

        serial = m_current->serialize();
    }else if(m_current->name() == m_clearscreen->name()){
#ifdef DEBUG_MANAGER
        qDebug() << "Manager::serialize(): clearscreen:" << m_current->serialize();
#endif

        serial = m_current->serialize();
    }else if(m_current->name() == m_changecolour->name()){
        qDebug() << "Manager::serialize(): changecolour:" << m_current->serialize();
#ifdef DEBUG_MANAGER
        qDebug() << "Manager::serialize(): changecolour:" << m_current->serialize();
#endif

        serial = m_current->serialize();
    }

    // code to serial to bits stream
    QVector<bool> bits;

    QString serial2 = QString::number(serial.size()).rightJustified(MANAGER_BITHEADER, ' ');
    serial2 += serial;
    serial2.resize(MANAGER_BITHEADER + serial.size());

    serial = serial2;
    for(ssize_t i = 0; i < serial.size(); ++i){
        std::bitset<BITLENGTH> byte(serial.toStdString().c_str()[i]);
        for(size_t j = 0; j < byte.size(); j++){
            bits.push_back(byte[j] == 1 ? true : false);
        }
    }

#ifdef DEBUG_MANAGER
    qDebug() << "Manager::serialize(): Command serialized to bitstream ok";
#endif

    return bits;
}

void Manager::deserialize(QVector<bool> bits)
{
#ifdef DEBUG_MANAGER
    qDebug() << "Manager::deserialize(): Deserializing from bitstream";
#endif

    // Deserializing code from bitstream -> Qstring parsable by [*]Command

    QString deserial;
    for(int i = 0; i < bits.size() / BITLENGTH; i++){
        std::bitset<BITLENGTH> byte;
        for(size_t j = 0; j < BITLENGTH; j++){
            byte[j] = bits[i*BITLENGTH + j];
        }
        QChar character(static_cast<char>(byte.to_ulong()));
        if(!character.isPrint()){
            // qDebug() << "Manager::deserialize(): Got non character in our serial. Cancelling";
#ifdef DEBUG_MANAGER
            qDebug() << "Manager::deserialize(): Got non character in our serial. Cancelling";
#endif
            return;
        }
        deserial.append(static_cast<char>(byte.to_ulong()));
    }

    int numberOfCharsToRead = deserial.left(MANAGER_BITHEADER).toInt();
    QString serial = deserial.mid(MANAGER_BITHEADER, numberOfCharsToRead);
    size_t pos1, pos2;

    std::string str = serial.toStdString();
    pos2 = str.find (" "); // get position of next " "

    QString command = QString::fromStdString(str.substr(0, pos2));  // get from " " to the end

    // qDebug() << "Manager::deserialize(): Deserialized command: " << serial;
#ifdef DEBUG_MANAGER
    qDebug() << "Manager::deserialize(): Deserialized command: " << serial;
#endif

    if(command == m_drawline->name()){
#ifdef DEBUG_MANAGER
        qDebug() << "Manager::deserialize(): Interpreting drawline command";
#endif

        pos1 = str.find(" ");      // position of 1st " " in str
        pos2 = str.find(" ", pos1+1);     // get position of next " "

        QStringList parameter1 = QString::fromStdString(str.substr(pos1+1,pos2-pos1-1)).split(","); // get from " " to the end
        if(parameter1.size() != 2){
#ifdef DEBUG_MANAGER
            qDebug() << "Manager::deserialize(): Unknown parameter #1 for drawline command. Cancelling";
#endif
            return;
        }
        m_drawline->setFrom(QPoint(parameter1[0].toInt(), parameter1[1].toInt()));

        pos1 = str.find(" ", pos2);      // position of 3st word in str onwards

        QStringList parameter2 = QString::fromStdString(str.substr(pos1+1)).split(","); // get from " " to the end
        if(parameter2.size() != 2){
#ifdef DEBUG_MANAGER
            qDebug() << "Manager::deserialize(): Unknown parameter #2 for drawline command. Cancelling";
#endif
            return;
        }
        m_drawline->setTo(QPoint(parameter2[0].toInt(), parameter2[1].toInt()));

        m_drawline->execute(m_image);

        emit imageModified();

    }else if(command == m_clearscreen->name()){
#ifdef DEBUG_MANAGER
        qDebug() << "Manager::deserialize(): Executing clearscreen command";
#endif

        m_clearscreen->execute(m_image);

        emit imageModified();
    }else if(command == m_changecolour->name()){
#ifdef DEBUG_MANAGER
        qDebug() << "Manager::deserialize(): Interpreting change colour command";
#endif

        pos1 = str.find(" ");      // position of 1st " " in str
        pos2 = str.find(" ", pos1+1);     // get position of next " "

        QString parameter1 = QString::fromStdString(str.substr(pos1+1,pos2-pos1-1)); // get from " " to the end
        if(parameter1.isEmpty()){
#ifdef DEBUG_MANAGER
            qDebug() << "Manager::deserialize(): Unknown parameter #1 for change colour command. Cancelling";
#endif
            return;
        }

        pos1 = str.find(" ", pos2);      // position of 3st word in str
        pos2 = str.find(" ", pos1+1);     // get position of next " "

        QString parameter2 = QString::fromStdString(str.substr(pos1+1,pos2-pos1-1)); // get from " " to the next " "
        if(parameter2.isEmpty()){
#ifdef DEBUG_MANAGER
            qDebug() << "Manager::deserialize(): Unknown parameter #2 for change colour command. Cancelling";
#endif
            return;
        }

        pos1 = str.find(" ", pos2);      // position of 3st word in str onwards

        QString parameter3 = QString::fromStdString(str.substr(pos1+1)); // get from " " to the end
        if(parameter3.isEmpty()){
#ifdef DEBUG_MANAGER
            qDebug() << "Manager::deserialize(): Unknown parameter #3 for change colour command. Cancelling";
#endif
            return;
        }

        m_colour = QColor(parameter1.toInt(), parameter2.toInt(), parameter3.toInt());

        m_drawline->setPenColour(m_colour);

        m_changecolour->execute(m_image);

        emit imageModified();

    }


}

void Manager::setToolbarAware(QToolBar *toolbar){
    m_toolbar = toolbar;
    m_drawlineAction = m_toolbar->addAction("drawline function");
    connect(m_drawlineAction, SIGNAL(triggered()), this, SLOT(onDrawlineActionTriggered()));

    m_clearscreenAction = m_toolbar->addAction("clearscreen action");
    connect(m_clearscreenAction, SIGNAL(triggered()), this, SLOT(onClearscreenActionTriggered()));

    m_changecolourAction = m_toolbar->addAction("change colour action");
    connect(m_changecolourAction, SIGNAL(triggered()), this, SLOT(onChangeColourActionTriggered()));
}

// SLOTS

void Manager::onMousePressed(QMouseEvent *event)
{
    if(m_current->name() == m_drawline->name()){
#ifdef DEBUG_MANAGER
        qDebug() << "Manager::onMousePressed(): drawLine" << event->pos();
#endif

        ((DrawLine *) m_current)->setFrom(event->pos());
    }
}

void Manager::onMouseReleased(QMouseEvent *event)
{
    if(m_current->name() == m_drawline->name()){
#ifdef DEBUG_MANAGER
        qDebug() << "Manager::onMouseReleased(): drawLine" << event->pos();
#endif

        ((DrawLine *) m_current)->setTo(event->pos());

        // serialize the command, to pass to receive boardc
        // deserialize n execute();

        emit send(serialize());
    }
}

void Manager::onMouseMoved(QMouseEvent *event)
{
    if(m_current->name() == m_drawline->name()){
#ifdef DEBUG_MANAGER
        qDebug() << "Manager::onMousePressed(): drawLine. Current pos " << event->pos();
#endif

        ((DrawLine *) m_current)->setTo(event->pos());

        emit send(serialize());
    }
}

void Manager::onDrawlineActionTriggered()
{
#ifdef DEBUG_MANAGER
    qDebug() << "Manager::onDrawlineActionTriggered(): Drawline selected";
#endif

    m_current = m_drawline;
}

void Manager::onClearscreenActionTriggered()
{
#ifdef DEBUG_MANAGER
    qDebug() << "Manager::onClearscreenActionTriggered(): Clearscreen selected. Executing cls and setting back to drawline";
#endif

    m_current = m_clearscreen;

    emit send(serialize());

    m_current = m_drawline;
}

void Manager::onChangeColourActionTriggered()
{
#ifdef DEBUG_MANAGER
    qDebug() << "Manager::onChangeColourActionTriggered(): Change colour selected. Executing c kolour and setting back to drawline";
#endif

    m_current = m_changecolour;

    QColor colour = QColorDialog::getColor(m_colour, NULL, "Chose a colour plz");
    if(colour.isValid()) m_colour = colour;
    else qWarning() << "Manager::serialize(): Unknown colour. Cancelling serialize";

    m_changecolour->setColour(m_colour);

    emit send(serialize());

    m_current = m_drawline;
}

void Manager::read(QVector<bool> bits)
{
#ifdef DEBUG_MANAGER
    qDebug() << "Manager::Read(): Command being read";
#endif

    deserialize(bits);
}
