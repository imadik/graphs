#include "functhread.h"

functhread::functhread()
{
    mutex.lock();

}

//поток просчитывающий координаты
void functhread::run()
{
    while (1)
    {
        QMutexLocker locker(&mutex);
        if (!compleeted)
            iteration();
        else if (cordsVectorPart.size() > 0)
        {
            sendVectorPart(cordsVectorPart);
            cordsVectorPart.clear();
        }

    }
}

//фнкция запуска и остановки
void functhread::startStop()
{

    if (locked)
    {
        locked = false;
        mutex.unlock();
    }
    else
    {
        locked = true;
        mutex.lock();
    }
}
//перегруженная функция запуска остановки
void functhread::startStop(bool stop)
{
    if (locked)
    {
        if (!stop)
        {
            locked = false;
            mutex.unlock();
        }
    }
    else
    {   if (stop)
        {
            locked = true;
            mutex.lock();
        }
    }
}

//задание новой функции
void functhread::setFunction(int funcType, double A, double B, double C, double intStart, double intFinish, double intStep)
{
    functhread::funcType = funcType;
    functhread::A = A;
    functhread::B = B;
    functhread::C = C;
    functhread::intStart = intStart;
    functhread::intFinish = intFinish;
    functhread::intStep = intStep;
    newFunc = true;
    compleeted = false;
    cordsVector.clear();
    //посылка сигнала виджету с отрезком
    sendInterval(intStart, intFinish);
}
//загрузка функции из файла
void functhread::loadCords(int funcType, double A, double B, double C, double intStart, double intFinish, double intStep, std::vector<cordsPair> cordsVector)
{
    startStop(true);
    functhread::funcType = funcType;
    functhread::A = A;
    functhread::B = B;
    functhread::C = C;
    functhread::intStart = intStart;
    functhread::intFinish = intFinish;
    functhread::intStep = intStep;
    //посылка сигнала виджету с отрезком
    sendInterval(intStart, intFinish);
    functhread::cordsVector.clear();
    functhread::cordsVector = cordsVector;
    oldFunc = true;
    compleeted = false;
    // посылка координат виджту
    sendVectorPart(cordsVector);
    if (!isRunning())
        start();
//    startStop(false);

}

// функция итерации потока
inline bool functhread::iteration()
{
    static int count;
    count++;
    static double currentX;
    double currentY;
    static int lastProcent = 0;
    //настройка если новая функция
    if (newFunc)
    {
        currentX = intStart;
        newFunc = false;
        cordsVector.clear();
        cordsVectorPart.clear();
        lastProcent = 0;
    }
    //настройка если загружена старая функция
    if (oldFunc)
    {
        currentX = cordsVector[cordsVector.size() - 1].x;
        oldFunc = false;

        lastProcent = 0;
    }
    // то что должно быть важным, вычисление функции
    switch (funcType) {
    case 0:
        currentY = A*(currentX*currentX) + B*currentX + C;
        break;
    case 1:
        currentY = A * sin(currentX) + B * cos( C*currentX );
        break;
    case 2:
        currentY = A*log( B*currentX );
        break;
    case 3:
        currentY = A / ( sin(currentX*currentX) * B );
        break;
    default:
        break;
    }

    cordsPair currentPair;
    currentPair.x = currentX;
    currentPair.y = currentY;
    if ((!(std::isnan(currentY))) && (!(std::isinf(currentY))))
    {
        cordsVector.push_back(currentPair);
        cordsVectorPart.push_back(currentPair);
    }
// копим координаты и отправляем только раз в 10к, это делает интерфейс более отзывчивым, так как он не успевает отрисовываться
    //можно ввести задержки, но это не дело
    //можно заставить ждать отрисовки, но это тоже не дело
    if((currentX >= intFinish) || count > 10000)
    {
        count = 0;
        sendVectorPart(cordsVectorPart);

        cordsVectorPart.clear();
    }
    currentX += intStep;
    if (currentX >= intFinish)
        compleeted = true;
// вычисление прогресса
    int procent = ((fabs(currentX - intStart) / fabs(intFinish - intStart)) * 100);
    if (procent > lastProcent)
    {
        lastProcent = procent;
        sendProcent(procent);
    }
}

// кто то запрашивает координаты
void functhread::requestCords()
{
    sendCordsToSave(cordsVector);
}
