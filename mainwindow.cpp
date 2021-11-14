#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    m_vObjects = new vector<Object>;

    // create and connect Info Dialog
    m_pInfoWindow = new InfoDialog();
    connect(ui->HelpButton, SIGNAL(clicked()), m_pInfoWindow, SLOT(show()));

    // create and connect Cutting Dialog
    m_pCuttingWindow = new CuttingDialog();
    //connect(ui->StartCuttingButton, SIGNAL(clicked()), this, SLOT(on_StartCuttingButton_clicked()));

    // Provide data to Cutting dialog
    connect(this, SIGNAL(send_SheetSize(int, int)), m_pCuttingWindow, SLOT(receive_SheetSize(int, int)));
    connect(this, SIGNAL(send_ObjectVector(vector<Object>, int)), m_pCuttingWindow, SLOT(receive_ObjectVector(vector<Object>, int)));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_StartCuttingButton_clicked()
{
    if (!m_pCuttingWindow->isVisible())
    {
        m_vObjects->clear();
        for (int i = 0; i < ui->FirstObjectCountSpinBox->value(); i++)
        {
            m_vObjects->insert(m_vObjects->end(),
                               *new Object(QPoint(0, 0),
                               QSize(ui->FirstObjectLengthSpinBox->value(),
                                     ui->FirstObjectWidthSpinBox->value())));
        }
        for (int i = 0; i < ui->SecondObjectCountSpinBox->value(); i++)
        {
            m_vObjects->insert(m_vObjects->end(),
                               *new Object(QPoint(0, 0),
                               QSize(ui->SecondObjectLengthSpinBox->value(),
                                     ui->SecondObjectWidthSpinBox->value())));
        }

        qDebug() << "m_vObjects->size() " << m_vObjects->size();
        for (auto it = m_vObjects->begin(); it != m_vObjects->end(); it++)
        {
            qDebug() << "object width: " << it->width() << endl
                     << "object length: " << it->height() << endl
                     << "object area: " << it->getArea() << endl;
        }

        m_pCuttingWindow->show();
        emit send_SheetSize(ui->SheetLengthSpinBox->value(), ui->SheetWidthSpinBox->value());
        emit send_ObjectVector(*m_vObjects, ui->FirstObjectCountSpinBox->value());
    }
}
