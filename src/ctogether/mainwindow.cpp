#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"decode.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ct_decode decoder = ct_decode();
    
    connect(&decoder,&ct_decode::ct_image_decoded,
                        this,&MainWindow::ct_update_image);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush brush;
    brush.setColor(QColor(33, 34, 35, 0));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    // buttom color
    painter.drawRect(0, 0, this->width(), this->height());

   // QImage Image = 

    // scale the image into mainwindow's size
    //Image = queue->image_queue->ct_pop_front();
    QImage image = Image.scaled(this->size(), Qt::KeepAspectRatio);

    // center the image
    int x = (this->width() - image.width()) / 2;
    int y = (this->height() - image.height()) / 2;

    x /= 2; y /= 2;

    painter.drawImage(QPoint(x, y), image);
    qDebug()<<"DREW!!";
}

void MainWindow::ct_update_image(QImage img)
{
    Image = img;
    update();
}

