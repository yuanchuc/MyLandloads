#include "player.h"

Player::Player(QObject *parent)
    : QObject{parent}
{

}

Player::Player(QString name, QObject *parent)
{
    this->m_name = name;
}

void Player::setName(QString name)
{
    this->m_name = name;
}

QString Player::getName()
{
    return this->m_name;
}

void Player::setRole(Role role)
{
    this->m_role = role;
}

Player::Role Player::getRole()
{
    return this->m_role;
}

void Player::setSex(Sex sex)
{
    this->m_sex = sex;
}

Player::Sex Player::getSex()
{
    return this->m_sex;
}

void Player::setDirection(Direction direction)
{
    this->m_direction = direction;
}

Player::Direction Player::getDirection()
{
    return this->m_direction;
}

void Player::setType(Type type)
{
    this->m_type = type;
}

Player::Type Player::getType()
{
    return this->m_type;
}

void Player::setScore(int score)
{
    this->m_score = score;
}

void Player::setWin(bool flag)
{
    this->m_isWin = flag;
}

bool Player::isWin()
{
    return this->m_isWin;
}

void Player::setPrevPlayer(Player *player)
{
    this->m_prev = player;
}

void Player::setNextPlayer(Player *player)
{
    this->m_next = player;
}

Player *Player::getPrevPlayer()
{
    return this->m_prev;
}

Player *Player::getNextPlayer()
{
    return this->m_next;
}

void Player::storeDispatchCard(Card &card)
{
    this->m_cards.add(card);
}

void Player::storeDispatchCard(Cards &cards)
{
    this->m_cards.add(cards);
}

Cards Player::getCards()
{
    return this->m_cards;
}

void Player::clearCards()
{
    this->m_cards.clear();
}

void Player::playHand(Cards &cards)
{
    this->m_cards.remove(cards);
}

void Player::setPendingInfo(Player *player, Cards &cards)
{
    this->m_pendCards =cards;
    this->m_pendCards = cards;
}

Cards Player::getPendCards()
{
    return m_pendCards;
}

Player *Player::getPendPlayer()
{
    return m_pendPlayer;
}

void Player::prepareCallLord()
{

}

void Player::preparePlayHand()
{

}
