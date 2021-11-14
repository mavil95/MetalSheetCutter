#include "cuttingdialog.h"
#include "ui_cuttingdialog.h"



CuttingDialog::CuttingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CuttingDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    m_vObjects = new vector<Object>;
    m_pScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(m_pScene);
    m_vRects = new vector<QGraphicsRectItem *>;
    m_vRectsSubgroup = new vector<vector<QGraphicsItemGroup *>>;
    m_vRectsSubgroup->push_back(*new vector<QGraphicsItemGroup *>);
    m_sSheetTrueSize = new QSize;
    m_pMessage = new QMessageBox(this);
    m_pMessage->setDefaultButton(QMessageBox::Ok);
    m_vObjectGroupDistributor = new vector<int>;

    m_pRects = new QGraphicsItemGroup;
    m_pRects->setParentItem(new QGraphicsRectItem);
    m_pScene->addItem(m_pRects);


    m_pSheetPixmap = new QPixmap;
    m_pSheetPixmap->load("D:/Programming/PROJECTS/QT/Metal_sheet_cutter/metal_sheet.jpg");

    sheetBrush = new QBrush(QPixmap(*m_pSheetPixmap));
    objectBrush = new QBrush(Qt::lightGray);
    pen = new QPen();
    pen->setWidth(0);
    pen->setBrush(Qt::red);

    QPalette sheetPalette;
    sheetPalette.setBrush(QPalette::Background, *sheetBrush);

    m_pMetalSheet = new QLabel();
    m_pMetalSheet->move(0, 0);
    m_pMetalSheet->setFixedWidth(ui->graphicsView->width());
    m_pMetalSheet->setScaledContents(true);
    m_pMetalSheet->setPalette(sheetPalette);
}

CuttingDialog::~CuttingDialog()
{
    delete ui;
}

void CuttingDialog::receive_SheetSize(int length, int width)
{
    // set scale and size of metal sheet
    int bigger, smaller;
    length > width ? (bigger = length, smaller = width) : (bigger = width, smaller = length);
    m_sSheetTrueSize->setWidth(bigger);
    m_sSheetTrueSize->setHeight(smaller);
    m_dScale = double(m_pMetalSheet->width()) / bigger;
    m_pMetalSheet->setFixedHeight(smaller * m_dScale);

    if (length == bigger)
    {
        ui->DimSheetLabel->setText("length");
    }
    else
    {
        ui->DimSheetLabel->setText("width");
    }

    // move interface
    ui->graphicsView->setFixedHeight(m_pMetalSheet->height() + 2);
    this->setFixedHeight(40 /* start of sheet widget */ + m_pMetalSheet->height() /* sheet height */ + 60 /* place for buttons */);
    ui->BackButton->move(ui->BackButton->pos().x(), 40 /* start of sheet widget */ + m_pMetalSheet->height() /* sheet height */ + 20);
    ui->PlaceObjectButton->move(ui->PlaceObjectButton->pos().x(), 40 /* start of sheet widget */ + m_pMetalSheet->height() /* sheet height */ + 20);

    m_pScene->addWidget(m_pMetalSheet);

    qDebug() << m_pMetalSheet->width();
    qDebug() << m_pMetalSheet->height();
    qDebug() << m_dScale;
}

void CuttingDialog::receive_ObjectVector(vector<Object> vector, int firstTypeCount)
{
    m_vObjects->clear();
    for (auto it = vector.begin(); it != vector.end(); it++)
    {
        m_vObjects->push_back(*it);
    }
    sort(m_vObjects->begin(), m_vObjects->end());
    for (auto it = m_vObjects->begin(); it != m_vObjects->end(); it++)
    {
        qDebug() << "object width+: " << it->width() << endl
                 << "object length+: " << it->height() << endl
                 << "object area+: " << it->getArea() << endl;
    }
    m_nObjectsLeft = int(m_vObjects->size());
    ui->CountSpinBox->setValue(m_nObjectsLeft);
    m_nFirstTypeCount = firstTypeCount;

}

void CuttingDialog::on_PlaceObjectButton_clicked()
{
    if (m_nObjectsLeft)
    {
        if (m_vObjects->at(m_nObjectsLeft - 1).height() > m_sSheetTrueSize->height() ||
            m_vObjects->at(m_nObjectsLeft - 1).width() > m_sSheetTrueSize->width())
        {
            qDebug() << "Warning";
            m_pMessage->setText("This object doen't fit to the metall sheet! Proceed to the next object.");
            m_pMessage->show();
        }
        else
        {
            qDebug() << "Drawing";

            calculateNextPoint();
            QRect CurRect(QPoint(m_vObjects->at(m_nObjectsLeft - 1).x(), m_vObjects->at(m_nObjectsLeft - 1).y()),
                        QSize(m_vObjects->at(m_nObjectsLeft - 1).width() * m_dScale, m_vObjects->at(m_nObjectsLeft - 1).height() * m_dScale));
            m_vRects->push_back(m_pScene->addRect(CurRect, *pen, *objectBrush));
        }
        m_nObjectsLeft--;
        ui->CountSpinBox->setValue(m_nObjectsLeft);
        if (!m_nObjectsLeft)
        {
            qDebug() << "Finish";
            for (auto it = m_vRects->begin(); it != m_vRects->end(); it++)
            {
                m_pRects->addToGroup(*it);
            }
            QString resultString = "Finish of cutting process! You will spend ~ ";
            resultString.push_back(QString::number(m_pRects->boundingRect().width() / m_dScale));
            resultString.push_back(" linear centimeters of material.");
            m_pMessage->setText(resultString);
            m_pMessage->show();
            for (auto it = m_vRects->begin(); it != m_vRects->end(); it++)
            {
                m_pRects->removeFromGroup(*it);
            }
        }
    }
    else
    {
        qDebug() << "No more objects";
        m_pMessage->setText("There is no more objects to place");
        m_pMessage->show();
    }
}

void CuttingDialog::closeEvent(QCloseEvent *)
{
    qDebug() << "Closing";
    for (auto it = m_vRects->begin(); it != m_vRects->end(); it++)
    {
        m_pScene->removeItem(*it);
    }
    m_vRects->clear();
}

void CuttingDialog::calculateNextPoint()
{
    if (!m_vRects)
        return;
    for (auto it = m_vRects->begin(); it != m_vRects->end(); it++)
    {
        m_pRects->addToGroup(*it);
    }
    qDebug() << "Bounding rect width: " << m_pRects->boundingRect().width();
    qDebug() << "Bounding rect height: " << m_pRects->boundingRect().height();
    if (m_pRects->boundingRect().height() + m_vObjects->at(m_nObjectsLeft - 1).height() > m_pMetalSheet->height())
    {
        m_vObjects->at(m_nObjectsLeft - 1).moveTo(m_pRects->boundingRect().width(), 0);
    }
    else
    {
        m_vObjects->at(m_nObjectsLeft - 1).moveTo(0, m_pRects->boundingRect().height());
    }
    qDebug() << "point x: " << m_vObjects->at(m_nObjectsLeft - 1).x();
    qDebug() << "point y: " << m_vObjects->at(m_nObjectsLeft - 1).y();
    for (auto it = m_vRects->begin(); it != m_vRects->end(); it++)
    {
        m_pRects->removeFromGroup(*it);
    }
}


