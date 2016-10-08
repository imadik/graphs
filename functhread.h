#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QMutex>
#include <cmath>


struct cordsPair;

Q_DECLARE_METATYPE(std::vector<cordsPair>);

class functhread: public QThread
{
    Q_OBJECT
private:
    QMutex mutex;
    int funcType;
    double A,B,C,intStart,intFinish,intStep;
    bool newFunc = false;
    bool oldFunc = false;
    bool notCompleeted;
    bool compleeted;
    bool working;
    std::vector<cordsPair> cordsVector;
    std::vector<cordsPair> cordsVectorPart;
    inline bool iteration();
    bool locked = true;


protected:
    void run();
public:
    functhread();
    void startStop();
    void startStop(bool stop);
    void setFunction(int funcType, double A, double B, double C, double intStart, double intFinish, double intStep);
public slots:
    void requestCords();
    void loadCords(int funcType, double A, double B, double C, double intStart, double intFinish, double intStep, std::vector<cordsPair> cordsVector);
//    void getCords(std::vector<cordsPair> cordsVector);
signals:
    void sendVectorPart(std::vector<cordsPair> cordsVectorPart);
    void sendInterval(int start, int finish);
    void sendProcent(int);
    void sendCordsToSave(std::vector<cordsPair> &cordsVector);
};

struct cordsPair
{
    double x;
    double y;
};

#endif // THREAD_H
