#ifndef CUTTINGDIALOG_H
#define CUTTINGDIALOG_H

#include <QDialog>
#include <QRect>
#include <QDebug>
#include <QLabel>
#include <QPalette>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QMessageBox>
#include <object.h>

using namespace std;

#ifndef CURRENT_OBJECT
#define CURRENT_OBJECT m_vObjects->at(m_nObjectsLeft - 1)
#endif

namespace Ui {
class CuttingDialog;
}

class CuttingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CuttingDialog(QWidget *parent = nullptr);
    ~CuttingDialog();

public slots:
    void receive_SheetSize(int length, int width);
    void receive_ObjectVector(vector<Object> vector);

private slots:
    void on_PlaceObjectButton_clicked();
    void on_ShowResultButton_clicked();

private:
    void closeEvent(QCloseEvent *) override;
    void calculateNextPoint();
    void showWarningMessage();
    void showResultMessage();

    Ui::CuttingDialog *ui;
    QGraphicsScene *m_pScene;

    QLabel *m_pMetalSheet;
    QSize *m_sSheetTrueSize;
    QPixmap *m_pSheetPixmap;
    QBrush *sheetBrush;
    QPen *pen;
    QBrush *objectBrush;

    vector<Object> *m_vObjects;
    vector<QGraphicsRectItem *> *m_vRects;
    QGraphicsItemGroup *m_pRects;
    QMessageBox *m_pMessage;

    double m_dScale {0.0};
    int m_nObjectsLeft {0};
};

#endif // CUTTINGDIALOG_H
