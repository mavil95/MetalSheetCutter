#ifndef CUTTINGDIALOG_H
#define CUTTINGDIALOG_H

#include <QDialog>
#include <object.h>
#include <QRect>
#include <QDebug>
#include <QLabel>
#include <QPalette>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QMessageBox>

using namespace std;

namespace Ui {
class CuttingDialog;
}

class CuttingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CuttingDialog(QWidget *parent = nullptr);
    ~CuttingDialog();

    friend class MainWindow;

public slots:
    void receive_SheetSize(int length, int width);
    void receive_ObjectVector(vector<Object> vector, int firstTypeCount);

private slots:
    void on_PlaceObjectButton_clicked();

private:
    void closeEvent(QCloseEvent *) override;
    void calculateNextPoint();

    Ui::CuttingDialog *ui;
    QPainter *m_pPainter;
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
    vector<vector<QGraphicsItemGroup *>> *m_vRectsSubgroup;
    vector<int> *m_vObjectGroupDistributor;
    QMessageBox *m_pMessage;

    double m_dScale {0.0};
    int m_nObjectsLeft {0};
    int m_nFirstTypeCount {0};

};

#endif // CUTTINGDIALOG_H
