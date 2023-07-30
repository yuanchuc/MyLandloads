#include "userplayer.h"

UserPlayer::UserPlayer(QObject *parent)
    : Player{parent}
{
    this->m_type = Type::Robot;
}

void UserPlayer::prepareCallLord()
{

}

void UserPlayer::preparePlayHand()
{

}
