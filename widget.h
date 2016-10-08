#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <cmath>
#include <QRectF>
#include <QPainter>
#include "functhread.h"
#include <QPen>
#include <QApplication>


class widget : public QWidget
{
    Q_OBJECT
private:
    void paintGrid(QPainter &p);
    void paintGraph(QPainter &p);

    std::vector<cordsPair> cordsVector;
    int intervalStart;
    int intervalFinish;
    double maxY();
    bool newGraph = true;


protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
public:
    explicit widget(QWidget *parent = 0);

signals:
    void mouseIsPressed();

public slots:
    void getInterval(int start, int finish);
    void getCordsVector(std::vector<cordsPair> cordsVectorPart);


};


#endif // WIDGET_H
