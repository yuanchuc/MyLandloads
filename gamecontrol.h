#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>
#include"robot.h"
#include"userplayer.h"
#include"cards.h"
class GameControl : public QObject
{
    Q_OBJECT
public:
    //游戏状态
    enum GameStatus{
        DispatchCard,   //分牌状态
        CallingLord,    //叫地主状态
        PlayingHand     //出牌状态
    };
    //玩家状态
    enum PlayerStatus{
        ThinkingForCallLord,//考虑叫地主
        ThinkingForPlayHand,//考虑出牌
        winning             //赢了
    };

    explicit GameControl(QObject *parent = nullptr);
    //初始化玩家
    void playerInit();
    //得到玩家的实例化对象
    Robot*getLeftRobot();
    Robot*getRightRobot();
    UserPlayer* getUserPlayer();
    //当前出牌玩家
    void setCurrentPlayer(Player*player);
    Player* getCurrentPlayer();
    //得到出牌玩家和打出的牌
    Player* getPendPlayer();
    Cards getPendCards();

    //初始化扑克牌
    void initAllCards();
    //每次发一张牌
    Card takeOneCard();
    //得到最后的三张底牌
    Cards getSurplusCards();

    //重置卡牌数据
    void resetCardData();

    //准备叫地主
    void startLordCard();
    //成为地主
    void becomeLord(Player*player);

    //清空所有玩家的得分
    void clearPlayerScore();

signals:
private:
    Robot *m_left;
    Robot *m_right;
    UserPlayer *m_user;
    Player *m_currPlayer;
    Player *m_pendPlayer;
    Cards m_pendCards;
    Cards m_allCards;
};

#endif // GAMECONTROL_H
