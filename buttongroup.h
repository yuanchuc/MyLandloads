#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>

namespace Ui {
class ButtonGroup;
}

class ButtonGroup : public QWidget
{
    Q_OBJECT

public:
    //按钮组切换的枚举
    enum Panel{
        Start,PassOrPlay,CallLord,Empty,PlayCard
    };
    explicit ButtonGroup(QWidget *parent = nullptr);
    ~ButtonGroup();
    //初始化按钮
    void initButtons();
    //处理page页面的切换
    void selectPage(Panel type);

signals:
    //开始游戏的信号
    void startGame();
    //出牌
    void playHand();
    //不出牌
    void pass();
    //抢地主(bet为分数)
    void betPoint(int bet);

private:
    Ui::ButtonGroup *ui;
};

#endif // BUTTONGROUP_H
