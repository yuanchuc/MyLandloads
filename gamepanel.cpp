#include "gamepanel.h"
#include "ui_gamepanel.h"
#include<QPainter>
#include<QRandomGenerator>
#include<QDebug>
GamePanel::GamePanel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GamePanel)
{
    ui->setupUi(this);

    // 1.背景图
    int num = QRandomGenerator::global()->bounded(10);
    QString path = QString(":/images/background-%1.png").arg(num+1);
    m_bkImage.load(path);
    // 2.窗口的标题大小
    this->setWindowTitle("欢乐斗地主");
    this->setFixedSize(1000,650);
    // 3.实例化游戏控制类
    GameControlInit();

    // 4.初始化玩家得分(更新)
    updatePlayerScore();

    // 5.切割游戏图片(card)
    initCardMap();

    // 6.初始化游戏中的按钮组
    initButtonsGroup();
    // 7.初始化玩家在窗口的上下文环境
    initPlayerContext();
    // 8.扑克牌场景初始化
    initGameScene();
    // 定时器实例化
    m_timer = new QTimer(this);
    connect(m_timer,&QTimer::timeout,this,&GamePanel::onDispatchCard);
}

GamePanel::~GamePanel()
{
    delete ui;
}

void GamePanel::GameControlInit()
{
    m_gameCtl = new GameControl(this);
    m_gameCtl->playerInit();
    // 3.1得到三个玩家的实例对象
    Robot * leftRobot = m_gameCtl->getLeftRobot();
    Robot * rightRobot = m_gameCtl->getRightRobot();
    UserPlayer * user = m_gameCtl->getUserPlayer();
    // 3.2存储的顺序：左侧机器人，右侧机器人，当前玩家
    this->m_playerList<<leftRobot<<rightRobot<<user;
    connect(m_gameCtl,&GameControl::playerStatusChanged,this,&GamePanel::onPlayerStatusChanged);
}

void GamePanel::updatePlayerScore()
{
    ui->scorePanel->setScores(m_playerList[0]->getScore(),m_playerList[1]->getScore(),m_playerList[2]->getScore());
}

void GamePanel::initCardMap()
{
    QPixmap pixmap(":/images/card.png");
    m_cardSize.setWidth(pixmap.width() / 13);
    m_cardSize.setHeight(pixmap.height() / 5);

    m_cardBackImg = pixmap.copy(2*m_cardSize.width(), 4*m_cardSize.height(),
                                m_cardSize.width(), m_cardSize.height());
    for(int i=0, suit=Card::Suit_Begin+1; suit<Card::Suit_End; ++suit, ++i)
    {
        for(int j=0, pt=Card::Card_Begin+1; pt<Card::Card_SJ; ++pt, ++j)
        {
            Card card((Card::CardPoint)pt, (Card::CardSuit)suit);
            cropImage(pixmap, j*m_cardSize.width(), i*m_cardSize.height(), card);
        }
    }
    Card c;
    c.setPoint(Card::Card_SJ);
    c.setSuit(Card::Suit_Begin);
    cropImage(pixmap, 0, 4*m_cardSize.height(), c);

    c.setPoint(Card::Card_BJ);
    c.setSuit(Card::Suit_End);
    cropImage(pixmap, m_cardSize.width(), 4*m_cardSize.height(), c);

}

void GamePanel::cropImage(QPixmap &pix, int x, int y, Card& c)
{
    QPixmap sub = pix.copy(x, y, m_cardSize.width(), m_cardSize.height());
    CardPanel* panel = new CardPanel(this);
    panel->setImage(sub, m_cardBackImg);
    panel->setCard(c);
    panel->setSelected(false);
    panel->hide();
    m_cardMap.insert(c, panel);
}

void GamePanel::initButtonsGroup()
{
    //初始化按钮组
    ui->btnGroup->initButtons();
    ui->btnGroup->selectPage(ButtonGroup::Start);
    //按钮-开始游戏
    connect(ui->btnGroup,&ButtonGroup::startGame,this,[=](){
        qDebug()<<"开始游戏";
        //界面的初始化
        ui->btnGroup->selectPage(ButtonGroup::Empty);
        m_gameCtl->clearPlayerScore();
        updatePlayerScore();
        //修改游戏状态
        gameSatusPrecess(GameControl::DispatchCard);
    });
    //按钮-出牌
    connect(ui->btnGroup,&ButtonGroup::playHand,this,[=](){});
    //按钮-跳过
    connect(ui->btnGroup,&ButtonGroup::pass,this,[=](){});
    //按钮-叫地主(点数)
    connect(ui->btnGroup,&ButtonGroup::betPoint,this,[=](){});
}

void GamePanel::initPlayerContext()
{
    // 1. 放置玩家扑克牌的区域
    const QRect cardsRect[] =
        {
            QRect(90, 130, 100, height() - 200),                    //左侧
            QRect(rect().right() - 190, 130, 100, height() - 200),  //右侧
            QRect(250, rect().bottom() - 120, width() - 500, 100)   //当前玩家
        };
    // 2. 玩家出牌的区域
    const QRect playHandRect[] =
        {
            QRect(260, 150, 100, 100),                              //左侧
            QRect(rect().right() - 360, 150, 100, 100),             //右侧
            QRect(150, rect().bottom() - 290, width() - 300, 105)   //当前玩家
        };
    // 3. 玩家头像显示的位置
    const QPoint roleImgPos[] =
        {
            QPoint(cardsRect[0].left()-80, cardsRect[0].height() / 2 + 20),//左侧
            QPoint(cardsRect[1].right()+10, cardsRect[1].height() / 2 + 20),//右侧
            QPoint(cardsRect[2].right()-10, cardsRect[2].top() - 10)        //当前玩家
        };
    // 循环
    int index = m_playerList.indexOf(m_gameCtl->getUserPlayer());
    for(int i=0; i<m_playerList.size(); ++i)
    {
        PlayerContext context;
        //对齐方式
        context.align = i==index ? Horizontal : Vertical;
        //卡牌正面 还是反面
        context.isFrontSide = i==index ? true : false;
        //防止扑克牌的区域
        context.cardRect = cardsRect[i];
        //出牌区域
        context.playHandRect = playHandRect[i];
        //提示信息
        context.info = new QLabel(this);
        context.info->resize(160, 98);
        context.info->hide();
        //显示到扑克牌中心区域
        QRect rect = playHandRect[i];
        QPoint pt(rect.left() + (rect.width() - context.info->width()) / 2,
                  rect.top() + (rect.height() - context.info->height()) / 2);
        context.info->move(pt);

        //玩家的头像
        context.roleImg = new QLabel(this);
        context.roleImg->resize(84, 120);
        context.roleImg->hide();
        context.roleImg->move(roleImgPos[i]);
        m_contextMap.insert(m_playerList.at(i), context);
    }
}

void GamePanel::initGameScene()
{
    // 1.发牌区的扑克牌
    m_baseCard = new CardPanel(this);
    m_baseCard->setImage(m_cardBackImg,m_cardBackImg);
    // 2.发牌过程中移动的扑克牌
    m_moveCard = new CardPanel(this);
    m_moveCard->setImage(m_cardBackImg,m_cardBackImg);
    // 3.最后的三张底牌(用于窗口的显示)
    for(int i=0;i<3;++i){
        CardPanel*panel = new CardPanel(this);
        panel->setImage(m_cardBackImg,m_cardBackImg);
        m_last3Card.push_back(panel);
        panel->hide();
    }
    // 扑克牌的位置
    m_baseCardPos = QPoint((width() - m_cardSize.width()) / 2,
                           height() / 2 - 100);
    m_baseCard->move(m_baseCardPos);
    m_moveCard->move(m_baseCardPos);

    int base = (width() - 3 * m_cardSize.width() - 2 * 10) / 2;
    for(int i=0; i<3; ++i)
    {
        m_last3Card[i]->move(base + (m_cardSize.width() + 10) * i, 20);
    }
}

void GamePanel::gameSatusPrecess(GameControl::GameStatus status)
{

    // 记录游戏状态
    m_gameStatus = status;
    // 处理游戏状态
    switch(status){
        //发牌状态
    case GameControl::DispatchCard:
        startDispatchCard();
        break;
        //叫地主状态
    case GameControl::CallingLord:{
        // 取出底牌数据
        CardList last3Card = m_gameCtl->getSurplusCards().toCardList();
        // 给底牌窗口设置图片
        for(int i = 0;i<last3Card.size();i++){
            QPixmap front = m_cardMap[last3Card.at(i)]->getImage();
            m_last3Card[i]->setImage(front,m_cardBackImg);
            m_last3Card[i]->hide();
        }
        // 开始叫地主
        m_gameCtl->startLordCard();
        break;
    }
        //出牌状态
    case GameControl::PlayingHand:
        break;
    default:
        break;
    }
}

void GamePanel::startDispatchCard()
{
    // 初始化
    for(auto it = m_cardMap.begin(); it!= m_cardMap.end(); ++it)
    {
        it.value()->setSelected(false);
        it.value()->setFrontSide(true);
        it.value()->hide();
    }
    // 隐藏三张底牌
    for(int i=0;i<m_last3Card.size();i++){
        m_last3Card.at(i)->hide();
    }
    // 重置玩家的窗口上下文信息
    int index = m_playerList.indexOf(m_gameCtl->getUserPlayer());
    for(int i = 0;i<m_playerList.size();i++){
        auto context = m_contextMap[m_playerList.at(i)];
        context.lastCard.clear();
        context.lastCard.clear();
        context.roleImg->hide();
        context.isFrontSide = i==index? true:false;
    }
    // 重置所有玩家的卡牌数据
    m_gameCtl->resetCardData();
    // 显示底牌
    m_baseCard->show();
    // 隐藏按钮面板
    ui->btnGroup->selectPage(ButtonGroup::Empty);
    // 启动定时器
    m_timer->start(10);
    // 播放背景音乐
}

void GamePanel::cardMoveStep(Player *player, int curPos)
{
    QRect cardRect = m_contextMap[player].cardRect;
    //每个玩家的步长
    const int unit[] = {
        (m_baseCardPos.x() - cardRect.right()) / 100,
        (cardRect.left() - m_baseCardPos.x()) / 100,
        (cardRect.top() - m_baseCardPos.y()) / 100
    };
    //每次窗口移动的时候每个玩家对应的牌的实时坐标位置
    QPoint pos[] =
        {
            QPoint(m_baseCardPos.x() - curPos * unit[0], m_baseCardPos.y()),
            QPoint(m_baseCardPos.x() + curPos * unit[1], m_baseCardPos.y()),
            QPoint(m_baseCardPos.x(), m_baseCardPos.y() + curPos * unit[2]),
        };
    //移动扑克牌
    int index = m_playerList.indexOf(player);
    m_moveCard->move(pos[index]);

    if(curPos == 0)
    {
        m_moveCard->show();
    }
    if(curPos == 100)
    {
        m_moveCard->hide();
    }
}

void GamePanel::disposCard(Player *player, Cards &cards)
{
    CardList list = cards.toCardList();
    for(int j=0;j<list.size();++j){
        CardPanel* panel = m_cardMap.value(list.at(j));
        panel->setOwner(player);
    }
    //更新扑克牌在窗口中的显示
    updatePlayerCards(player);
}

void GamePanel::updatePlayerCards(Player *player)
{
    Cards cards = player->getCards();
    CardList list = cards.toCardList();
    //展示扑克牌的位置
//    m_cardsRect = QRect();
//    m_userCards.clear();
    int cardSpace = 20;
    QRect cardsRect = m_contextMap[player].cardRect;
    for(int i=0; i<list.size(); ++i)
    {
        CardPanel* panel = m_cardMap.value(list.at(i));
        panel->show();
        panel->raise();
        panel->setFrontSide(m_contextMap[player].isFrontSide);
        //水平 or 垂直显示
        if(m_contextMap[player].align == Horizontal)
        {
            int leftX = cardsRect.left() + (cardsRect.width() - (list.size() - 1) * cardSpace - panel->width()) / 2;
            int topY = cardsRect.top() + (cardsRect.height() - m_cardSize.height()) / 2;
            if(panel->isSelected())
            {
                topY -= 10;
            }
            panel->move(leftX + cardSpace * i, topY);
//            m_cardsRect = QRect(leftX, topY,cardSpace * i + m_cardSize.width(), m_cardSize.height());
//            int curWidth = 0;
//            if(list.size() - 1 == i)
//            {
//                curWidth = m_cardSize.width();
//            }
//            else
//            {
//                curWidth = cardSpace;
//            }
//            QRect cardRect(leftX + cardSpace * i, topY, curWidth, m_cardSize.height());
//            m_userCards.insert(panel, cardRect);
        }
        else
        {
            int leftX = cardsRect.left() + (cardsRect.width() - m_cardSize.width()) / 2;
            int topY = cardsRect.top() + (cardsRect.height() - (list.size() - 1) * cardSpace - panel->height()) / 2;
            panel->move(leftX, topY + i * cardSpace);
        }
    }

//    QRect playCardRect = m_contextMap[player].playHandRect;
//    Cards lastCards = m_contextMap[player].lastCards;
//    if(!lastCards.isEmpty())
//    {
//        int playSpacing = 24;
//        CardList lastCardList = lastCards.toCardList();
//        CardList::ConstIterator itplayed = lastCardList.constBegin();
//        for(int i=0; itplayed != lastCardList.constEnd(); ++itplayed, i++)
//        {
//            CardPanel* panel = m_cardMap[*itplayed];
//            panel->setFrontSide(true);
//            panel->raise();
//            if(m_contextMap[player].align == Horizontal)
//            {
//                int leftBase = playCardRect.left() +
//                               (playCardRect.width() - (lastCardList.size() - 1) * playSpacing - panel->width()) / 2;
//                int top = playCardRect.top() + (playCardRect.height() - panel->height()) /2 ;
//                panel->move(leftBase + i * playSpacing, top);
//            }
//            else
//            {
//                int left = playCardRect.left() + (playCardRect.width() - panel->width()) / 2;
//                int top = playCardRect.top() ;
//                panel->move(left, top + i * playSpacing);
//            }
//            panel->show();
//        }
//    }
}

//发牌阶段
void GamePanel::onDispatchCard()
{
    // 记录扑克牌的位置
    static int curMovePos = 0;
    // 当前玩家
    Player* curPlayer = m_gameCtl->getCurrentPlayer();
    if(curMovePos >=100 ){
        // 给玩家发一张牌
        Card card = m_gameCtl->takeOneCard();
        curPlayer->storeDispatchCard(card);
        Cards cs(card);
        disposCard(curPlayer,cs);
        // 移动扑克牌
        cardMoveStep(curPlayer,curMovePos);
        // 切换玩家
        m_gameCtl->setCurrentPlayer(curPlayer->getNextPlayer());
        curMovePos = 0;

        //判断牌是否发完了
        if(m_gameCtl->getSurplusCards().cardCount()==3){
            //终止定时器
            m_timer->stop();
            //切换游戏状态
            gameSatusPrecess(GameControl::CallingLord);
            // 移动扑克牌
            cardMoveStep(curPlayer,curMovePos);
            curMovePos += 15;
            return;
        }
    }

    // 移动扑克牌
    cardMoveStep(curPlayer,curMovePos);
    curMovePos += 15;
}

void GamePanel::onPlayerStatusChanged(Player *player, GameControl::PlayerStatus status)
{
    switch(status){
    case GameControl::ThinkingForCallLord:
        if(player == m_gameCtl->getUserPlayer()){
            ui->btnGroup->selectPage(ButtonGroup::CallLord);
        }
        break;
    case GameControl::ThinkingForPlayHand:
        break;
    case GameControl::winning:
        break;
    default:
        break;
    }
}

void GamePanel::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.drawPixmap(rect(),m_bkImage);

}

