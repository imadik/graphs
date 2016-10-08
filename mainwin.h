#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>
#include "functhread.h"
#include "widget.h"
#include "opensavewindow.h"

class OpenSaveWindow;
namespace Ui {
class MainWin;
}

/*
 * Уважаемые проверяющие данное тестовое задание, прошу отнестись с пониманием ко многим недочётам и корявым комментариям.
 * Дело в том что данное тестовое задание выполянлось в свободное от работы и многих других дел время, а это 2-4 часа(а чаще 1 часик)  по будням после работы, и часто в ночное время.
 * Потому прошу простить за бредовость многих комментариев, велосипеды и косытыли (2е и 3е скорее по неопытнысти)
 * Поскольку само задание получается достаточно обширным, особенно если пытаться охватить все возможные варианты развития событий, ну и по неопытнысти, в нём до сих пор могут присутствовать косяки
 * и не доработки. как то так.
 */
class MainWin : public QMainWindow
{
    Q_OBJECT
protected:
    void resizeEvent(QResizeEvent* event);
public:
    explicit MainWin(QWidget *parent = 0);
    ~MainWin();
    int func;
    double A;
    double B;
    double C;
    double start;
    double finish;
    double step;

private:
    Ui::MainWin *ui;
    widget *w;
    functhread *t;
    OpenSaveWindow *saveWin;
    bool inProgress;
    void makeActive();
    void makeDesactive();

public slots:
    void saveAction();
    void updateUi();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void getProcent(int procent);
    void on_pushButton_3_clicked();
    void graphLoaded();
};

#endif // MAINWIN_H
