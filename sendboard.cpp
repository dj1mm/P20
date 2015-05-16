#include "sendboard.h"

Sendboard::Sendboard(QWidget *parent): QWidget(parent)
{
    setWindowTitle("Send Board");
    setMinimumSize(400, 400);

    m_image = new QImage(this->size(), QImage::Format_RGB32);
    m_image->fill(QColor(255, 255, 255));

    m_manager = new Manager;
    m_manager->setImage(m_image);

    connect(this, SIGNAL(mousePressed(QMouseEvent *)), m_manager, SLOT(onMousePressed(QMouseEvent*)));
    connect(this, SIGNAL(mouseReleased(QMouseEvent *)), m_manager, SLOT(onMouseReleased(QMouseEvent*)));
    connect(this, SIGNAL(mouseMoved(QMouseEvent*)), m_manager, SLOT(onMouseMoved(QMouseEvent*)));

    connect(m_manager, SIGNAL(imageModified()), this, SLOT(onImageModified()));

}

void Sendboard::mousePressEvent(QMouseEvent *event){
    // take mouse location and tell the manager
    //QPoint p1 = event->pos();

#ifdef DEBUG_APPLICATION
    qDebug() << "Sendboard::MousePressEvent(): Event Triggered";
#endif

    emit mousePressed(event);
}

void Sendboard::mouseReleaseEvent(QMouseEvent *event)
{
    //take mouse location and tell manager

#ifdef DEBUG_APPLICATION
    qDebug() << "Sendboard::mouseReleaseEvent(): Event Triggered";
#endif

    emit mouseReleased(event);
}

void Sendboard::mouseMoveEvent(QMouseEvent *event)
{
    //take mouse location and tell manager
#ifdef DEBUG_APPLICATION
    qDebug() << "Sendboard::mouseMoveEvent(): Event Triggered";
#endif

    emit mouseMoved(event);
}

void Sendboard::paintEvent(QPaintEvent *event)
{

#ifdef DEBUG_APPLICATION
    qDebug() << "Sendboard::paintEvent(): yaya";
#endif

    QPainter painter(this);
    painter.drawImage(this->rect(), *m_image, this->rect());

}

void Sendboard::resizeEvent(QResizeEvent *event)
{
    if(event->size().height() < m_image->height() || event->size().width() < m_image->width())
        return;

#ifdef DEBUG_APPLICATION
    qDebug() << "SendBoard:;resizeEvent(): Resizing image to " << event->size();
#endif

    // Create a new image (of new size of window)
    QImage *newImage = new QImage(event->size(), QImage::Format_RGB32);
    newImage->fill(QColor(255, 255, 255));

    // Paste old image on the new image
    QPainter painter(newImage);
    painter.drawImage(m_image->rect(), *m_image, m_image->rect());

    // replace
    *m_image = *newImage;
}

// SLOT

void Sendboard::onImageModified()
{
    update();
}

