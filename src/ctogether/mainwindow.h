#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<qimage.h>
#include<qpainter.h>
#include<qcolor.h>
#include"av_queues.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event)override;

public slots:
    void ct_update_image(QImage img);

private:
    Ui::MainWindow *ui;
    QImage Image;
};
#endif // MAINWINDOW_H
