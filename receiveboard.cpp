#include "receiveboard.h"

Receiveboard::Receiveboard(QWidget *parent): QWidget(parent)
{
    setWindowTitle("Receive Board");
    setMinimumSize(400, 400);

    m_image = new QImage(this->size(), QImage::Format_RGB32);
    m_image->fill(QColor(255, 255, 255));

    m_manager = new Manager;
    m_manager->setImage(m_image);

    connect(m_manager, SIGNAL(imageModified()), this, SLOT(onImageModified()));

    //m_image = new QImage(this->size(), QImage::Format_RGB32);
}

void Receiveboard::paintEvent(QPaintEvent *event)
{

#ifdef DEBUG_APPLICATION
    qDebug() << "Receiveboard::paintEvent(): Painting to Receive Board";
#endif

    QPainter painter(this);
    painter.drawImage(this->rect(), *m_image, this->rect());

}

void Receiveboard::resizeEvent(QResizeEvent *event)
{
    if(event->size().height() < m_image->height() || event->size().width() < m_image->width())
        return;

#ifdef DEBUG_APPLICATION
    qDebug() << "Receiveboard:;resizeEvent(): Resizing image to " << event->size();
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

void Receiveboard::onImageModified()
{
    update();
}

