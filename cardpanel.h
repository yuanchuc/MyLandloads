#ifndef CARDPANEL_H
#define CARDPANEL_H

#include <QWidget>
#include<QPainter>
#include"card.h"
#include"player.h"
class CardPanel : public QWidget
{
    Q_OBJECT
//信号
signals:

public:
    explicit CardPanel(QWidget *parent = nullptr);
    //设置正面图片和后面图片
    void setImage(QPixmap &front,QPixmap &back);

    //获取正面图片
    QPixmap getImage();

    //扑克牌显示的那一面
    void setFrontSide(bool flag);
    bool isFrontSide();

    //记录窗口是否被选中
    void setSelected(bool flag);
    bool isSelected();

    //扑克牌的花色和点数
    void setCard(Card& card);
    Card getCard();

    //卡牌的所有者
    void setOwner(Player * player);
    Player * getOwner();

protected:
    //绘制图片
    void paintEvent(QPaintEvent *event)override;
    //鼠标事件
    void mousePressEvent(QMouseEvent *event)override;
private:
    QPixmap m_front;
    QPixmap m_back;
    bool m_isfront;
    bool m_isSelect;
    Card card;
    Player* m_owner = nullptr;
};

#endif // CARDPANEL_H
