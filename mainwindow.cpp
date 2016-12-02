#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "segmenter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->image->setScaledContents(true);

    QImage image;
    image.load("Images/rural1.jpg");

    ui->image->setPixmap(QPixmap::fromImage(image));

    ui->image->show();

    Segmenter segmenter("Images/rural1.jpg", 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_imageNumber_activated(const QString &num)
{
    QImage image;
    image.load("Images/rural" + num + ".jpg");

    ui->image->setPixmap(QPixmap::fromImage(image));

    ui->image->show();

    Segmenter segmenter("Images/rural" + num.toStdString() + ".jpg", num.toInt());
}
