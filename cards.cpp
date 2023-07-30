#include "cards.h"
#include<QRandomGenerator>
#include<algorithm>
Cards::Cards()
{

}

void Cards::add(const Card &card)
{
    m_cards.insert(card);
}

void Cards::add(const Cards &cards)
{
    this->m_cards.unite(cards.m_cards);
}

Cards &Cards::operator<<(const Card &card)
{
    this->add(card);
    return *this;
}
Cards &Cards::operator<<(const Cards &cards)
{
    this->add(cards);
    return *this;
}
void Cards::remove(Card &card)
{
    this->m_cards.remove(card);
}

void Cards::remove(Cards &cards)
{
    this->m_cards.subtract(cards.m_cards);
}

int Cards::cardCount()
{
    return this->m_cards.size();
}

bool Cards::isEmpty()
{
    return m_cards.isEmpty();
}

void Cards::clear()
{
    this->m_cards.clear();
}

Card::CardPoint Cards::maxPoint()
{
    //如果为空,返回Card::Card_Begin
    Card::CardPoint max = Card::Card_Begin;
    //如果卡牌不为空
    if(!this->m_cards.isEmpty()){
        for(auto card:this->m_cards){
            if(card.point() > max){
                max = card.point();
            }
        }
    }
    return max;
}

Card::CardPoint Cards::minPoint()
{
    //如果为空,返回Card::Card_Begin
    Card::CardPoint min = Card::Card_End;
    //如果卡牌不为空
    if(!this->m_cards.isEmpty()){
        for(auto card:this->m_cards){
            if(card.point() < min){
                min = card.point();
            }
        }
    }
    return min;
}

int Cards::pointCount(Card::CardPoint point)
{
    int count = 0;
    for(auto card:this->m_cards){
        if(card.point() == point){
            count++;
        }
    }
    return count;
}

bool Cards::contains(const Card &card)
{
    return this->m_cards.contains(card);
}

bool Cards::contains(const Cards &cards)
{
    return this->m_cards.contains(cards.m_cards);
}

Card Cards::takeRandCard()
{
    //生成一个随机数
    int num = QRandomGenerator::global()->bounded(this->m_cards.size());
    QSet<Card>::const_iterator it = this->m_cards.constBegin();
    for(int i=0;i<num;++i,++it);
    Card card = *it;
    this->m_cards.erase(it);
    return card;

}

CardList Cards::toCardList(SortType type)
{
    CardList list;
    for(auto card:this->m_cards){
        list<< card;
    }
    if(type == Asc){
        std::sort(list.begin(),list.end(),lessSort);
    }else if(type == Desc){
        std::sort(list.begin(),list.end(),greaterSort);
    }
    return list;
}
