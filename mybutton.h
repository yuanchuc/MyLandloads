#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include<QPainter>
class MyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyButton(QWidget *parent = nullptr);
    void setImage(QString normal,QString hover,QString pressed);
protected:
    //鼠标按下
    void mousePressEvent(QMouseEvent *e);
    //鼠标释放
    void mouseReleaseEvent(QMouseEvent *e)override;
    //鼠标进入
    void enterEvent(QEvent *event);
    //鼠标离开
    void leaveEvent(QEvent *event);
    //绘图
    void paintEvent(QPaintEvent *ev);
signals:
private:
    QString m_normal;
    QString m_hover;
    QString m_pressed;
    QPixmap m_pixmap;
};

#endif // MYBUTTON_H
