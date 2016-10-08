#include "widget.h"

widget::widget(QWidget *parent) : QWidget(parent)
{

}

// рисуем оси координат, с учётом размера виджета, для масштобирования
void widget::paintGrid(QPainter &p)
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

// рисуем график функции, с дорисовкой и масштабированием и буферизацией
void widget::paintGraph(QPainter &p)
{
    static QPainterPath cashedPath;
    static int lastSize;
    static QRect cashedRect;
    static double curMaxY;
    static int lastx;
    static int lasty;
    // если новый график надо всё сбросить и пересчитать
    if (newGraph)
    {
        QPainterPath newPath;
        cashedPath = newPath;
        lastSize = 0;
        curMaxY = 0;
        newGraph = false;
    }
    // получаем текущий размер окна
    QRect a = p.window();
    //если заного перерисовываем весь график
    if (((a != cashedRect) || (cordsVector.size() < lastSize) || (lastSize == 0) || (maxY() > curMaxY)) && (cordsVector.size() > 0))
    {
        cashedRect = a;
        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;
        a.getCoords(&x1, &y1, &x2, &y2);
        double intervalVirtX = intervalFinish - intervalStart;
        double intervalRealX = x2;
        double coefX = intervalRealX / intervalVirtX;
        curMaxY = maxY();
        double intervalVirtY = curMaxY * 2;
        double intervalRealY = y2;
        double coefY = -intervalRealY / intervalVirtY;
        QPainterPath path;
// если точек очень много то в путь сохраняем только те что различаются, так ускоряется отрисовка
        int x = cordsVector[0].x * coefX + x2/2;
        int y = cordsVector[0].y * coefY + y2/2;
        lastx = x;
        lasty = y;
        path.moveTo(x, y);
        for (int i = 1; i < cordsVector.size(); i++)
        {
            x = cordsVector[i].x * coefX + x2/2;
            y = cordsVector[i].y * coefY + y2/2;
            if (x != lastx || y != lasty)
            {
                path.lineTo(x,y);
                lastx = x;
                lasty = y;
            }

        }
        QPen pen(Qt::red, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

        p.setPen(pen);
        cashedPath = path;
        p.drawPath(path);

    }
    else
    {
        // если нужно дорисовать график
        if ((cordsVector.size() != lastSize) && (cordsVector.size() > 0))
        {
            int x1 = 0;
            int y1 = 0;
            int x2 = 0;
            int y2 = 0;
            cashedRect.getCoords(&x1, &y1, &x2, &y2);
            double intervalVirtX = intervalFinish - intervalStart;
            double intervalRealX = x2;
            double coefX = intervalRealX / intervalVirtX;
            double intervalVirtY = maxY() * 2;
            double intervalRealY = y2;
            double coefY = -intervalRealY / intervalVirtY;
            QPainterPath path = cashedPath;
            int x = cordsVector[0].x * coefX + x2/2;
            int y = cordsVector[0].y * coefY + y2/2;
            lastx = x;
            lasty = y;
            path.moveTo(x, y);
            for (int i = 1; i < cordsVector.size(); i++)
            {
                x = cordsVector[i].x * coefX + x2/2;
                y = cordsVector[i].y * coefY + y2/2;
                if (x != lastx || y != lasty)
                {
                    path.lineTo(x,y);
                    lastx = x;
                    lasty = y;
                }

            }
            QPen pen(Qt::red, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

            p.setPen(pen);
            cashedPath = path;
            p.drawPath(path);
        }
        else
        {
            // и если рисуем уже нарисованный график
            if (cordsVector.size() > 0)
            {
                QPen pen(Qt::red, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

                p.setPen(pen);
                p.drawPath(cashedPath);
            }
        }
    }
//    QApplication::processEvents();


}


// отрисовка
void widget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    paintGrid(p);
    if (!(cordsVector.empty()))
        paintGraph(p);
}
// мышь нажата на виджете
void widget::mousePressEvent(QMouseEvent *event)
{
    mouseIsPressed();
}

//получаем пачку координат
void widget::getCordsVector(std::vector<cordsPair> cordsVectorPart)
{
    cordsVector.insert(cordsVector.end(), cordsVectorPart.begin(), cordsVectorPart.end());
    update();
}

//получаем инфцию об отрезке на котором строится график
void widget::getInterval(int start, int finish)
{
    intervalStart = start;
    intervalFinish = finish;
    cordsVector.clear();
    newGraph = true;
}

// ищем максимальное число У на графике
double widget::maxY()
{
    double maxY = 0;
    for (int i = 0; i < cordsVector.size(); i++)
    {
        if (maxY < fabs(cordsVector[i].y))
            maxY = fabs(cordsVector[i].y);
    }
    return maxY;
}
