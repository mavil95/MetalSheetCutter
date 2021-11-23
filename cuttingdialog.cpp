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
    m_sSheetTrueSize = new QSize;
    m_pMessage = new QMessageBox(this);
    m_pMessage->setDefaultButton(QMessageBox::Ok);

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
    delete m_pMetalSheet;
    delete m_sSheetTrueSize;
    delete m_pSheetPixmap;
    delete sheetBrush;
    delete pen;
    delete objectBrush;
    delete m_vObjects;
    delete m_vRects;
    delete m_pRects;
    delete m_pMessage;
    delete m_pScene;
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

    // set dimension label text
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
    int buttonsYpos = 40 /* start of sheet widget */ + m_pMetalSheet->height() /* sheet height */ + 20;
    this->setFixedHeight(buttonsYpos + 40 /* place for buttons and bottom space */);
    ui->BackButton->move(ui->BackButton->pos().x(), buttonsYpos);
    ui->PlaceObjectButton->move(ui->PlaceObjectButton->pos().x(), buttonsYpos);
    ui->ShowResultButton->move(ui->ShowResultButton->pos().x(), buttonsYpos);

    // add metal sheet on scene
    m_pScene->addWidget(m_pMetalSheet);
}

void CuttingDialog::receive_ObjectVector(vector<Object> vector)
{
    // clear current object vector and copy a new one from main window
    m_vObjects->clear();
    for (auto it = vector.begin(); it != vector.end(); it++)
    {
        m_vObjects->push_back(*it);
    }
    sort(m_vObjects->begin(), m_vObjects->end());

    // set objects left
    m_nObjectsLeft = int(m_vObjects->size());
    ui->CountSpinBox->setValue(m_nObjectsLeft);
}

void CuttingDialog::on_PlaceObjectButton_clicked()
{
    if (m_nObjectsLeft)
    {
        // check that object fit in the metal sheet
        if (CURRENT_OBJECT.height() > m_sSheetTrueSize->height() ||
            CURRENT_OBJECT.width() > m_sSheetTrueSize->width())
        {
            showWarningMessage();
        }

        // move current object and place it, if it fits to the metal sheet
        calculateNextPoint();
        if (CURRENT_OBJECT.topRight().x() > m_pMetalSheet->width())
        {
            showWarningMessage();
            CURRENT_OBJECT.moveTo(CURRENT_OBJECT.x() - CURRENT_OBJECT.width() * m_dScale,
                                  CURRENT_OBJECT.y() - CURRENT_OBJECT.width() * m_dScale);
        }
        else
        {
            QRect CurRect(QPoint(CURRENT_OBJECT.x(), CURRENT_OBJECT.y()),
                        QSize(CURRENT_OBJECT.width() * m_dScale, CURRENT_OBJECT.height() * m_dScale));
            m_vRects->push_back(m_pScene->addRect(CurRect, *pen, *objectBrush));
        }

        // count down object left
        m_nObjectsLeft--;
        ui->CountSpinBox->setValue(m_nObjectsLeft);

        // show result if last object was placed
        if (!m_nObjectsLeft)
        {
            showResultMessage();
            ui->ShowResultButton->setEnabled(true);
        }
    }
    else
    {
        m_pMessage->setText("There is no more objects to place");
        m_pMessage->show();
    }
}

void CuttingDialog::on_ShowResultButton_clicked()
{
    showResultMessage();
}

void CuttingDialog::closeEvent(QCloseEvent *)
{
    for (auto it = m_vRects->begin(); it != m_vRects->end(); it++)
    {
        m_pScene->removeItem(*it);
    }
    m_vRects->clear();
    ui->ShowResultButton->setEnabled(false);
}

void CuttingDialog::calculateNextPoint()
{
    /// This function moves current object to next available space
    /// Available space defined by intersecting with another objects

    for (int it = m_vObjects->size() - 1; it >= 0; it--)
    {
        if(it == m_nObjectsLeft - 1)
        {
            break;
        }
        QRect tempRect(m_vObjects->at(it));
        tempRect.setHeight(tempRect.height() * m_dScale);
        tempRect.setWidth(tempRect.width() * m_dScale);
        while (CURRENT_OBJECT.intersects(tempRect))
        {
            if (CURRENT_OBJECT.y() + (CURRENT_OBJECT.height() * m_dScale) > m_pMetalSheet->height())
            {
                CURRENT_OBJECT.moveTo(CURRENT_OBJECT.x() + 1, 0);
                calculateNextPoint();
            }
            else
            {
                CURRENT_OBJECT.moveTo(CURRENT_OBJECT.x(), CURRENT_OBJECT.y() + 1);
            }
        }
    }
}

void CuttingDialog::showWarningMessage()
{
    m_pMessage->setText("This object doesn't fit to the metal sheet! Proceed to the next object.");
    m_pMessage->show();
}

void CuttingDialog::showResultMessage()
{
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
