#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<qimage.h>
#include<qpainter.h>
#include<qcolor.h>
#include<QThread>

class ct_player;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void ct_update_image(QImage img);

private:
    Ui::MainWindow *ui;
    QImage Image;
    ct_player* player;
};
#endif // MAINWINDOW_H
