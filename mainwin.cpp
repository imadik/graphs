#include "mainwin.h"
#include "ui_mainwin.h"

MainWin::MainWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWin),
    w(new widget(this)),
    t(new functhread()),
    saveWin(new OpenSaveWindow(this))
{
    ui->setupUi(this);
    // Заполняем комбо бокс
    ui->comboBox->addItem("f(x) = A*(x*x) + B*x + C",1);
    ui->comboBox->addItem("f(x) = A * sin(x) + B * cos( C*x )",2);
    ui->comboBox->addItem("f(x) = A*log( B*x )",3);
    ui->comboBox->addItem("f(x) = A / ( sin(x*x) * B )",4);
//Задаём мин и максимальное вводимое значение и количество точек после запятой
    ui->lineEdit->setValidator( new QDoubleValidator(-1000, 1000, 5,  ui->lineEdit));
    ui->lineEdit_2->setValidator( new QDoubleValidator(-1000, 1000, 5, ui->lineEdit_2));
    ui->lineEdit_3->setValidator( new QDoubleValidator(-1000, 1000, 5, ui->lineEdit_3));
    ui->lineEdit_4->setValidator( new QDoubleValidator(-1000, 1000, 5, ui->lineEdit_4));
    ui->lineEdit_5->setValidator( new QDoubleValidator(-1000, 1000, 5, ui->lineEdit_5));
    ui->lineEdit_6->setValidator( new QDoubleValidator(-1000, 1000, 5, ui->lineEdit_6));
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    // соединяем слот и сигнал посылки инфы о графике
    connect(t, SIGNAL(sendInterval(int,int)), w, SLOT(getInterval(int,int)));
    // соединяем слот и и сигнал посылки части просчитанного вектора
    connect(t, SIGNAL(sendVectorPart(std::vector<cordsPair>)), w, SLOT(getCordsVector(std::vector<cordsPair>)));
    // соединяем сигнал нажатия на график с открытием окна сохранения
    connect(w, SIGNAL(mouseIsPressed()), this, SLOT(saveAction()));
    // соединяем посылку процентов
    connect(t, SIGNAL(sendProcent(int)), this, SLOT(getProcent(int)));
    // посылаем график для сохранения
    connect(t, SIGNAL(sendCordsToSave(std::vector<cordsPair>&)), saveWin, SLOT(getVectorToSave(std::vector<cordsPair>&)));
    // пересылка информации о открытом графике и сам вектор с графиком
    connect(saveWin, SIGNAL(vectorLoaded(int,double,double,double,double,double,double,std::vector<cordsPair>)), \
            t,SLOT(loadCords(int,double,double,double,double,double,double,std::vector<cordsPair>)));
    //
    connect(saveWin,SIGNAL(graphLoaded()),this, SLOT(graphLoaded()));
    // добавляем наш виджет с графиком на сетку
    ui->gridLayout->addWidget(w);




}

void MainWin::updateUi()
{

}

MainWin::~MainWin()
{
    delete ui;
}
// открываем окно с координатами для сохранения
void MainWin::saveAction()
{
    if (ui->pushButton_2->isEnabled() && (ui->pushButton_2->text() == "Pause") )
        on_pushButton_2_clicked();
    t->requestCords();
    saveWin->show();
}

//нажата кнопка Старт
void MainWin::on_pushButton_clicked()
{
//обработка случая после завершения просчёта или нажатия кнопки брейк
    if (ui->pushButton->text() == "New")
    {
        makeActive();
        ui->pushButton->setText("Start");

    }
    else
    if(!inProgress)
    {
        bool ok;
        QLocale locale;
        locale.setDefault(QLocale::German); // Decimal point is Comma: 000.000,00
        locale.setNumberOptions(QLocale::OmitGroupSeparator);
        func = ui->comboBox->currentIndex();
        // получаем данные с ячеек ввода
        A = locale.toDouble(ui->lineEdit->text(),&ok);
        B = locale.toDouble(ui->lineEdit_2->text(),&ok);
        C = locale.toDouble(ui->lineEdit_3->text(),&ok);
        start = locale.toDouble(ui->lineEdit_4->text(),&ok);
        finish = locale.toDouble(ui->lineEdit_5->text(),&ok);
        step = locale.toDouble(ui->lineEdit_6->text(),&ok);
        //задаём функцию
        t->setFunction(func, A, B, C, start, finish, step);
//        t->setFunction(ui->comboBox->currentIndex(), 1, 1, 1, -100 , 100, 0.0001);
//        t->setFunction(func, 1, 1, 1, -100 , 100, 1);
        // если первый раз запусскаем поток
        if (!t->isRunning())
            t->start(QThread::LowPriority);
        // отжимаем мьютекс
        t->startStop(false);
        // переключаем кнопочки интерфейса
        makeDesactive();
        inProgress = true;
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
    }
}

// меняем размер виджета если изменился размер окна
void MainWin::resizeEvent(QResizeEvent* event)
{
    QSize a = this->size();


    ui->gridFrame->setGeometry(270, 10, a.width() - (20 + 270), a.height() - 83);


}


// нажата кнопка паузы
void MainWin::on_pushButton_2_clicked()
{

    if (ui->pushButton_2->text() == "Pause")
    {
        t->startStop(true);
        ui->pushButton_2->setText("Continue");
    }
    else
    {
        t->startStop(false);
        ui->pushButton_2->setText("Pause");
    }
}

// слот выводящий проценты прогресса
void MainWin::getProcent(int procent)
{
    if(inProgress)
    {
        ui->pushButton->setText("Progress:" + QString::number(procent) + "%");
        // по достижению 100% меняет интерфейс
        if (procent == 100)
        {
            ui->pushButton->setText("New");
            ui->pushButton->setEnabled(true);
            ui->pushButton_2->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            inProgress = false;
        }
    }
}

//активируем интерфейс
void MainWin::makeActive()
{
    ui->comboBox->setEnabled(true);
    ui->lineEdit->setEnabled(true);
    ui->lineEdit_2->setEnabled(true);
    ui->lineEdit_3->setEnabled(true);
    ui->lineEdit_4->setEnabled(true);
    ui->lineEdit_5->setEnabled(true);
    ui->lineEdit_6->setEnabled(true);
}

//выключаем интерфейс
void MainWin::makeDesactive()
{
    ui->comboBox->setEnabled(false);
    ui->lineEdit->setEnabled(false);
    ui->lineEdit_2->setEnabled(false);
    ui->lineEdit_3->setEnabled(false);
    ui->lineEdit_4->setEnabled(false);
    ui->lineEdit_5->setEnabled(false);
    ui->lineEdit_6->setEnabled(false);
}

// нажата кнопка брейк
void MainWin::on_pushButton_3_clicked()
{
    // останавливаем поток лоча мьютекс
    t->startStop(true);
    inProgress = false;
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_2->setText("Pause");
    ui->pushButton->setText("New");
}

// переключаем интерфейс в режим загруженного графика
void MainWin::graphLoaded()
{
    makeDesactive();
    ui->pushButton_2->setText("Continue");
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
}
