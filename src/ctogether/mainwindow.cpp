#include "mainwindow.h"
#include"player.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   
    player = new ct_player();

    connect(player, SIGNAL(ct_player_emit_image(QImage)),
                         this, SLOT(ct_update_image(QImage)));

    //connect(player, SIGNAL(finished()),player, SLOT(deleteLater()));

    player->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete player;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush brush;
    brush.setColor(QColor(0x3, 0x47, 0xce, 255));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    // buttom color
    painter.drawRect(0, 0, this->width(), this->height());

    // scale the image into mainwindow's size
    // Image = QImage("D:\\Documents\\classInfoAssistant\\Ad\\icon.png");
    
    QImage image =  Image.scaled(this->size(), Qt::KeepAspectRatio,Qt::SmoothTransformation);
    
    // QSize size(Image.width(), Image.height());
    // this->resize(size);

    // center the image
    int x = (this->width() - image.width()) / 2;
    int y = (this->height() - image.height()) / 2;

    x /= 2; y /= 2;

    if (!image.isNull())
    {
        painter.drawImage(QPoint(x, y), image);
    }

   // qDebug()<<"DREW!!";
}

void MainWindow::ct_update_image(QImage img)
{
    Image = img;
    update();
}

