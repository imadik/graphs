#include "opensavewindow.h"
#include "ui_opensavewindow.h"

OpenSaveWindow::OpenSaveWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OpenSaveWindow)
{
    this->parent = (MainWin *)parent;
    ui->setupUi(this);
}

OpenSaveWindow::~OpenSaveWindow()
{
    delete ui;
}

//получаем вектор который будем сохранять
//,int funcR, double AR, double BR, double CR, double startR, double finishR, double stepR
void OpenSaveWindow::getVectorToSave(std::vector<cordsPair> &vectorTS)
{
    //vectorToSave = vectorTS;
    QStandardItemModel *model = new QStandardItemModel(vectorTS.size(), 2);
    QStringList labelList;
    labelList << "x" << "y";
    model->setHorizontalHeaderLabels(labelList);
    for (int i = 0; i < vectorTS.size(); i++)
    {
        QModelIndex idx;
        idx = model->index(i,0);
        model->setData(idx,vectorTS[i].x,Qt::EditRole);
        idx = model->index(i,1);
        model->setData(idx,vectorTS[i].y,Qt::EditRole);
    }
    ui->tableView->setModel(model);

}

//нажата кнопка сохарнить
void OpenSaveWindow::on_pushButton_save_clicked()
{
    QAbstractItemModel *model = ui->tableView->model();
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open XML File 1"), "/home", tr("XML Files (*.xml)"));

    QFile file(fileName);
        file.open(QIODevice::WriteOnly);
    if (file.isOpen())
    {
        //записываем данные в виде ХМЛ файла
        QXmlStreamWriter testXml(&file);
        testXml.setAutoFormatting(true);
        testXml.writeStartDocument();
        testXml.writeStartElement("save");

        testXml.writeTextElement("func", QString::number(parent->func));
        testXml.writeTextElement("A", QString::number(parent->A));
        testXml.writeTextElement("B", QString::number(parent->B));
        testXml.writeTextElement("C", QString::number(parent->C));
        testXml.writeTextElement("start", QString::number(parent->start));
        testXml.writeTextElement("finish", QString::number(parent->finish));
        testXml.writeTextElement("step", QString::number(parent->step));
        for (int i = 0; i < model->rowCount(); i++)
        {
            QModelIndex idx;
            idx = model->index(i,0);
            testXml.writeTextElement("cordx",model->data(idx).toString());
            idx = model->index(i,1);
            testXml.writeTextElement("cordy",model->data(idx).toString());
        }

        testXml.writeEndElement();
    }
}

//нажата кнопка открыть
void OpenSaveWindow::on_pushButton_2_save_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open XML File 1"), "/home", tr("XML Files (*.xml)"));

    QFile* file = new QFile(fileName);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    // используем стрим ридер, так как он быстрее всего парсит файл, а файл у нас достаточно простой
    QXmlStreamReader xml(file);
    std::vector<cordsPair> loadVector;
    cordsPair buff;
    while (!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement)
        {

            if (xml.name() == "func")
            {
                xml.readNext();
                parent->func = xml.text().toInt();
                continue;
            }
            if (xml.name() == "A")
            {
                xml.readNext();
                parent->A = xml.text().toDouble();
                continue;
            }
            if (xml.name() == "B")
            {
                xml.readNext();
                parent->B = xml.text().toDouble();
                continue;
            }
            if (xml.name() == "C")
            {
                xml.readNext();
                parent->C = xml.text().toDouble();
                continue;
            }
            if (xml.name() == "start")
            {
                xml.readNext();
                parent->start = xml.text().toDouble();
                continue;
            }
            if (xml.name() == "finish")
            {
                xml.readNext();
                parent->finish = xml.text().toDouble();
                continue;
            }
            if (xml.name() == "step")
            {
                xml.readNext();
                parent->step = xml.text().toDouble();
                continue;
            }
            if (xml.name() == "cordx")
            {
                xml.readNext();
                buff.x = xml.text().toDouble();
                continue;
            }
            if (xml.name() == "cordy")
            {
                xml.readNext();
                buff.y = xml.text().toDouble();
                loadVector.push_back(buff);
            }

        }
    }
    QStandardItemModel *model = new QStandardItemModel(loadVector.size(), 2);
    QStringList labelList;
    labelList << "x" << "y";
    model->setHorizontalHeaderLabels(labelList);
    for (int i = 0; i < loadVector.size(); i++)
    {
        QModelIndex idx;
        idx = model->index(i,0);
        model->setData(idx,loadVector[i].x,Qt::EditRole);
        idx = model->index(i,1);
        model->setData(idx,loadVector[i].y,Qt::EditRole);
    }
    ui->tableView->setModel(model);
    vectorLoaded(parent->func,parent->A,parent->B,parent->C,parent->start,parent->finish,parent->step,loadVector);
    graphLoaded();
}
