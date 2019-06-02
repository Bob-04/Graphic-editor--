#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mygraphicsscene.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_pen_clicked();
    void on_rubber_clicked();
    void on_spray_clicked();
    void on_pouring_clicked();

    void on_line_clicked();
    void on_rectangle_clicked();
    void on_elipse_clicked();
    void on_triangle_clicked();

    void on_size_actionTriggered(int action);
    void on_colors_clicked();

    void save_img();
    void open_img();
    void show_about();

private:
    Ui::MainWindow *ui;

    // For drawing scene:
    QGraphicsView *_view;
    MyGraphicsScene *_scene;

    // For menu:
    void createMenu();
    // For tooltips:
    void addToolTips();
};

#endif // MAINWINDOW_H
