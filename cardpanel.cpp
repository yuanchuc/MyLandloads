#include "cardpanel.h"

CardPanel::CardPanel(QWidget *parent)
    : QWidget{parent}
{
    this->m_isfront = true;
}

void CardPanel::setImage(QPixmap &front, QPixmap &back)
{
    this->m_front = front;
    this->m_back = back;

    setFixedSize(m_front.size());

    //调用paintEvent绘制
    update();
}

QPixmap CardPanel::getImage()
{
    return this->m_front;
}

void CardPanel::setFrontSide(bool flag)
{
    this->m_isfront = flag;
}

bool CardPanel::isFrontSide()
{
    return this->m_isfront;
}

void CardPanel::setSelected(bool flag)
{
    this->m_isSelect = flag;
}

bool CardPanel::isSelected()
{
    return this->m_isSelect;
}

void CardPanel::setCard(Card &card)
{
    this->card = card;
}

Card CardPanel::getCard()
{
    return this->card;
}

void CardPanel::setOwner(Player *player)
{
    this->m_owner = player;
}

Player *CardPanel::getOwner()
{
    return this->m_owner;
}

void CardPanel::paintEvent(QPaintEvent *event)
{
    // 创建 QPainter 对象，传入 QWidget 对象作为绘制设备
    QPainter painter(this);
    //开始绘制
    if(this->m_isfront){
        //如果是正面
        painter.drawPixmap(rect(),m_front);
    }else{
        //如果是背面
        painter.drawPixmap(rect(),m_back);
    }
}

void CardPanel::mousePressEvent(QMouseEvent *event)
{

}
