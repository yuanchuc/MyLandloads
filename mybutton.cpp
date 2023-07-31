#include "mybutton.h"

#include <QMouseEvent>

MyButton::MyButton(QWidget *parent)
    : QPushButton{parent}
{

}

void MyButton::setImage(QString normal, QString hover, QString pressed)
{
    this->m_normal = normal;
    this->m_hover = hover;
    this->m_pressed = pressed;
    this->m_pixmap.load(this->m_normal);
    update();
}

void MyButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton){
        m_pixmap.load(m_pressed);
        update();
    }
    QPushButton::mousePressEvent(e);
}


void MyButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton){
        m_pixmap.load(m_normal);
        update();
    }
    QPushButton::mouseReleaseEvent(e);
}

void MyButton::enterEvent(QEvent *event)
{
    m_pixmap.load(m_hover);
    update();
}

void MyButton::leaveEvent(QEvent *event)
{
    m_pixmap.load(m_normal);
    update();
}

void MyButton::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.drawPixmap(rect(),m_pixmap);
}
