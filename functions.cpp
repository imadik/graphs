#include "functions.h"

Functions::Functions(QWidget *parent) : QWidget(parent)
{

}

void Functions::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    paintGrid(p);

}

void Functions::paintGrid(QPainter &p)
{
    p.fillRect(rect(), QBrush(Qt::white));

    QRect a = p.window();
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    a.getCoords(&x1, &y1, &x2, &y2);
    p.drawLine(x1, y2/2, x2, y2/2);
    p.drawLine(x2/2, y1, x2/2, y2);


    QRectF rect = QRectF(x2-30, y2/2 - 5, 30, 10);

    QPainterPath path;
    path.moveTo(rect.right(), rect.top() + (rect.height() / 2));
    path.lineTo(rect.bottomLeft());
    path.lineTo(rect.topLeft());
    path.lineTo(rect.right(), rect.top() + (rect.height() / 2));

    p.fillPath(path, QBrush(QColor ("black")));

    rect = QRectF(x2/2 - 5, 0, 10, 30);
    path.moveTo(rect.left()  + (rect.width() / 2), rect.top());
    path.lineTo(rect.bottomLeft());
    path.lineTo(rect.bottomRight());
    path.lineTo(rect.left()  + (rect.width() / 2), rect.top());
    p.fillPath(path, QBrush(QColor ("black")));
}

Func1::Func1(QWidget *parent) : QWidget(parent)
{

}

void Func1::set(int A, int B, int C, int from, int to, int step)
{

}

void Func1::start()
{

}

void Func1::stop()
{

}

void Func1::break_f()
{

}

void Func1::save()
{

}

void Func1::load()
{

}


