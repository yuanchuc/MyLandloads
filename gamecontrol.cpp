#include "gamecontrol.h"
#include"QRandomGenerator"
GameControl::GameControl(QObject *parent)
    : QObject{parent}
{

}

void GameControl::playerInit()
{
    //对象实例化
    this->m_left = new Robot("机器人A",this);
    this->m_right = new Robot("机器人B",this);
    this->m_user = new UserPlayer("我自己",this);

    //头像显示方位
    this->m_left->setDirection(Player::Left);
    this->m_right->setDirection(Player::Left);
    this->m_user->setDirection(Player::Left);

    //性别
    Player::Sex sex;
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    this->m_left->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    this->m_right->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    this->m_user->setSex(sex);

    //玩家出牌顺序
    //user
    this->m_user->setPrevPlayer(m_left);
    this->m_user->setNextPlayer(m_right);

    //left user
    this->m_left->setPrevPlayer(m_right);
    this->m_left->setNextPlayer(m_user);

    //right user
    this->m_right->setPrevPlayer(m_user);
    this->m_right->setNextPlayer(m_left);

    //指定当前玩家
    this->m_currPlayer = m_user;


}

Robot *GameControl::getLeftRobot()
{
    return this->m_left;
}

Robot *GameControl::getRightRobot()
{
    return this->m_right;
}

UserPlayer *GameControl::getUserPlayer()
{
    return this->m_user;
}

void GameControl::setCurrentPlayer(Player *player)
{
    this->m_currPlayer = player;
}

Player *GameControl::getCurrentPlayer()
{
    return this->m_currPlayer;
}

Player *GameControl::getPendPlayer()
{
    return this->m_pendPlayer;
}

Cards GameControl::getPendCards()
{
    return this->m_pendCards;
}

void GameControl::initAllCards()
{
    this->m_allCards.clear();
    //获得50张扑克牌
    for(int p = Card::Card_Begin+1;p<Card::Card_SJ;++p){
        for(int s = Card::Suit_Begin+1;s<Card::Suit_End;++s){
            Card c((Card::CardPoint)p, (Card::CardSuit)s);
            m_allCards.add(c);
        }
    }
    m_allCards.add(Card(Card::Card_SJ,Card::Suit_Begin));
    m_allCards.add(Card(Card::Card_BJ,Card::Suit_End));
}

Card GameControl::takeOneCard()
{
    return this->m_allCards.takeRandCard();
}

Cards GameControl::getSurplusCards()
{
    //返回剩下的三张牌
    return this->m_allCards;
}

void GameControl::resetCardData()
{
    // 洗牌
    initAllCards();
    //清空所有玩家的牌
    m_left->clearCards();
    m_right->clearCards();
    m_user->clearCards();
    //初始化出牌玩家和牌
    m_pendCards.clear();
    m_pendPlayer = nullptr;
}

//准备叫地主
void GameControl::startLordCard()
{
    //虚函数(多态)
    //面对玩家和机器人两种处理逻辑
    m_currPlayer->prepareCallLord();
    emit playerStatusChanged(m_currPlayer,ThinkingForCallLord);
}

void GameControl::becomeLord(Player *player)
{
    player->setRole(Player::Lord);
    player->getPrevPlayer()->setRole(Player::Farmer);
    player->getNextPlayer()->setRole(Player::Farmer);

    m_currPlayer = player;
    player->storeDispatchCard(m_allCards);
    //准备出牌
    m_currPlayer->preparePlayHand();
}

void GameControl::clearPlayerScore()
{
    m_left->setScore(0);
    m_right->setScore(0);
    m_user->setScore(0);
}
