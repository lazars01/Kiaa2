#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "srp.h"
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <fstream>
#include <cstdlib>
#include <QTextStream>
#include <algorithm>
#include <random>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pbPokreni->setEnabled(false);
    ui->lbNotification->setVisible(false);
    ui->pbLoadDone->setVisible(false);
    ui->pbClear->setVisible(false);
    ui->teInput->setVisible(false);
    ui->lbFormat->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pbPokreni_clicked()
{
    std::cerr << "Kliknuto!\n";
    bool ima_resenja = m_srp->stable_roommate_algorithm();

    if(ima_resenja) {
        qDebug() << "Ima!";

        const char* desktopPath = std::getenv("HOME");

        if(desktopPath == nullptr) {
            ui->lbNotification->setVisible(true);
            ui->lbNotification->setText("There was an error writing result to file! :(");
            ui->lbNotification->setAlignment(Qt::AlignCenter);
            ui->lbNotification->setStyleSheet("color: red");
        }

        std::string filePath = desktopPath;
        filePath += "/Desktop/result.txt";

        m_srp->dumpToFile(filePath);
        ui->lbNotification->setVisible(true);
        ui->lbNotification->setText("There is a solution! Result is in 'result.txt' file on desktop.");
        ui->lbNotification->setAlignment(Qt::AlignCenter);
        ui->lbNotification->setStyleSheet("color: green");
    }
    else {
        qDebug() << "Nema";
        ui->lbNotification->setVisible(true);
        ui->lbNotification->setText("There isn't a solution for this instance! :(");
        ui->lbNotification->setAlignment(Qt::AlignCenter);
        ui->lbNotification->setStyleSheet("color: red");
    }
}

void MainWindow::on_pbLoad_clicked()
{
    ui->lbNotification->setVisible(false);

    std::string filePath = QFileDialog::getOpenFileName(this, "Choose File", QDir::homePath(), "").toStdString();
    std::cerr << filePath << std::endl;


    std::ifstream in(filePath);

    if(!in.is_open()) {
        qDebug() << "Nije otvoren!";
        return;
    }

    int n;
    in >> n;

    vector<vector<int>> preferences(n);
    for(int i = 0; i < n; i++)
        preferences[i].resize(n);

    vector<vector<int>> rankings(n);
    for(int i = 0; i < n; i++)
        rankings[i].resize(n);

    int pref;
    for(int i = 0; i < n; i++) {

        for(int j = 0; j < n - 1; j++) {
            in >> pref;
            preferences[i][j] = pref;
            rankings[i][pref] = j;
        }
        preferences[i][n-1] = i;
        rankings[i][i] = n-1;
    }

    m_srp = new SRP(preferences, rankings, n);
    ui->pbPokreni->setEnabled(true);

    in.close();
}

void MainWindow::on_pbLoadM_clicked()
{
    ui->lbNotification->setVisible(false);
    ui->pbLoadDone->setVisible(true);
    ui->pbClear->setVisible(true);
    ui->teInput->setVisible(true);
    ui->lbFormat->setVisible(true);
    ui->lbFormat->setStyleSheet("color: red");
}

void MainWindow::on_pbClear_clicked()
{
    ui->teInput->setText("");
}

void MainWindow::on_pbLoadDone_clicked()
{
    QString input = ui->teInput->toPlainText();

    QTextStream in(&input);

    int n;
    in >> n;

    vector<vector<int>> preferences(n);
    for(int i = 0; i < n; i++)
        preferences[i].resize(n);

    vector<vector<int>> rankings(n);
    for(int i = 0; i < n; i++)
        rankings[i].resize(n);

    int pref;
    for(int i = 0; i < n; i++) {

        for(int j = 0; j < n - 1; j++) {
            in >> pref;
            preferences[i][j] = pref;
            rankings[i][pref] = j;
        }
        preferences[i][n-1] = i;
        rankings[i][i] = n-1;
    }

    m_srp = new SRP(preferences, rankings, n);
    ui->pbPokreni->setEnabled(true);

    ui->pbLoadDone->setVisible(false);
    ui->pbClear->setVisible(false);
    ui->teInput->setVisible(false);
    ui->lbFormat->setVisible(false);
    ui->teInput->setText("");
}




