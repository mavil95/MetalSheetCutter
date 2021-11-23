#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    m_vObjects = new vector<Object>;
    m_pCuttingWindow = new CuttingDialog();

    // create and connect Info Dialog
    m_pInfoWindow = new InfoDialog();
    connect(ui->HelpButton, SIGNAL(clicked()), m_pInfoWindow, SLOT(show()));

    // Provide data to Cutting dialog
    connect(this, SIGNAL(send_SheetSize(int, int)), m_pCuttingWindow, SLOT(receive_SheetSize(int, int)));
    connect(this, SIGNAL(send_ObjectVector(vector<Object>)), m_pCuttingWindow, SLOT(receive_ObjectVector(vector<Object>)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_StartCuttingButton_clicked()
{
    if (!m_pCuttingWindow->isVisible())
    {
        // fill object vector from input widgets
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

        // send all required data to cutting window and show it
        emit send_SheetSize(ui->SheetLengthSpinBox->value(), ui->SheetWidthSpinBox->value());
        emit send_ObjectVector(*m_vObjects);
        m_pCuttingWindow->show();
    }
}

void MainWindow::closeEvent(QCloseEvent *)
{
    delete m_pInfoWindow;
    delete m_pCuttingWindow;
    delete m_vObjects;
}
