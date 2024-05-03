#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "srp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pbPokreni_clicked();

    void on_pbLoad_clicked();

    void on_pbLoadM_clicked();

    void on_pbClear_clicked();

    void on_pbLoadDone_clicked();


private:
    Ui::MainWindow *ui;
    SRP* m_srp;
};
#endif // MAINWINDOW_H
