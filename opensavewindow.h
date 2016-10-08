#ifndef OPENSAVEWINDOW_H
#define OPENSAVEWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtXml>
#include <QStandardItemModel>
#include <QFileDialog>
//#include "functhread.h"
#include "mainwin.h"

class MainWin;
namespace Ui {
class OpenSaveWindow;
}

class OpenSaveWindow : public QMainWindow
{
    Q_OBJECT

public:
    std::vector<cordsPair> vectorToSave;
    std::vector<cordsPair> openedVector;
    explicit OpenSaveWindow(QWidget *parent = 0);
    ~OpenSaveWindow();

private:
    Ui::OpenSaveWindow *ui;
    int func;
    double A;
    double B;
    double C;
    double start;
    double finish;
    double step;
    MainWin *parent;
public slots:
    void getVectorToSave(std::vector<cordsPair> &vectorTS);
private slots:

    void on_pushButton_save_clicked();
    void on_pushButton_2_save_clicked();
signals:
    void fileOpened();
    void vectorLoaded(int funcType, double A, double B, double C, double intStart, double intFinish, double intStep, std::vector<cordsPair> cordsVector);
    void graphLoaded();
};

#endif // OPENSAVEWINDOW_H
