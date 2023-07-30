#ifndef USERPLAYER_H
#define USERPLAYER_H

#include "player.h"
#include <QObject>

class UserPlayer : public Player
{
    Q_OBJECT
public:
    using Player::Player;   //允许使用基类构造函数
    explicit UserPlayer(QObject *parent = nullptr);
    void prepareCallLord()override;
    void preparePlayHand()override;
};

#endif // USERPLAYER_H
