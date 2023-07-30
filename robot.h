#ifndef ROBOT_H
#define ROBOT_H

#include "player.h"
#include <QObject>

class Robot : public Player
{
    Q_OBJECT
public:
    using Player::Player;   //允许使用基类构造函数
    explicit Robot(QObject *parent = nullptr);
    void prepareCallLord()override;
    void preparePlayHand()override;
};

#endif // ROBOT_H
