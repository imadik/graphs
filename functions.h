#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QWidget>

class Functions : public QWidget
{
    Q_OBJECT
protected:
    virtual void paintEvent(QPaintEvent *) = 0;

public:
    explicit Functions(QWidget *parent = 0);



signals:

private slots:
    void paintGrid(QPainter &p);
public slots:

};

class Func1 : public Functions
{
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent *);
public:
    explicit Func1(QWidget *parent = 0);
    void set(int A, int B, int C, int from, int to, int step);
    void start();
    void stop();
    void break_f();
    void save();
    void load();
private slots:

};

#endif // FUNCTIONS_H
