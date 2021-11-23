#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <infodialog.h>
#include <cuttingdialog.h>
#include <object.h>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
    class CuttingDialog;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void send_SheetSize(int legth, int width);
    void send_ObjectVector(vector<Object> vector);

public slots:
    void on_StartCuttingButton_clicked();

private:
    void closeEvent(QCloseEvent *) override;

    Ui::MainWindow *ui;
    InfoDialog *m_pInfoWindow;
    CuttingDialog *m_pCuttingWindow;
    vector<Object> *m_vObjects;
};

#endif // MAINWINDOW_H
