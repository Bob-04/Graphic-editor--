#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _view(new QGraphicsView()),
    _scene(new MyGraphicsScene(_view))
{
    ui->setupUi(this);
    this->setStyleSheet("background-color:#40e0d0;");

    createMenu();
    addToolTips();

    _view->setScene(_scene);
    _view->setCursor(Qt::CrossCursor);
    ui->gridLayout->addWidget(_view);

    //-----
    ui->toolsWidget->setStyleSheet("background-color:#40e0f0;");
    ui->toolsWidget->hide();
    ui->toolsWidget->raise();
    ui->colors->setStyleSheet("background-color: black");
    //-----
    this->setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);
    ui->menuIcon_path->setMouseTracking(true);
    ui->toolsWidget->setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _scene;
}

// EVENTS
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->pos().x() < 60) {
        ui->menuIcon_path->hide();
        ui->toolsWidget->show();
    }
    else if(event->pos().x() > 200) {
        ui->toolsWidget->hide();
        ui->menuIcon_path->show();
    }
}

// ACTIONS

// Instruments:
void MainWindow::on_pen_clicked()
{
    _scene->setDrawingStage(MyGraphicsScene::DrawingStage::Pen);
}
void MainWindow::on_rubber_clicked()
{
    _scene->setDrawingStage(MyGraphicsScene::DrawingStage::Rubber);
}
void MainWindow::on_spray_clicked()
{
    _scene->setDrawingStage(MyGraphicsScene::DrawingStage::Spray);
}
void MainWindow::on_pouring_clicked()
{
    _scene->setDrawingStage(MyGraphicsScene::DrawingStage::Pouring);
}

// Figures:
void MainWindow::on_line_clicked()
{
    _scene->setDrawingStage(MyGraphicsScene::DrawingStage::Line);
}
void MainWindow::on_rectangle_clicked()
{
    _scene->setDrawingStage(MyGraphicsScene::DrawingStage::Rectangle);
}
void MainWindow::on_elipse_clicked()
{
    _scene->setDrawingStage(MyGraphicsScene::DrawingStage::Ellipse);
}
void MainWindow::on_triangle_clicked()
{
    _scene->setDrawingStage(MyGraphicsScene::DrawingStage::Triangle);
}

// Size:
void MainWindow::on_size_actionTriggered(int action)
{
    action = 2;
    _scene->setPenSize(ui->size->value());
}
// Colors:
void MainWindow::on_colors_clicked()
{
    QColor color = QColorDialog::getColor();
    QString style = QString("background-color: %1").arg(color.name());
    ui->colors->setStyleSheet(style);
    _scene->setPenColor(color);
}

void MainWindow::addToolTips()
{
    // Instruments
    ui->pen->setToolTip("pen");
    ui->rubber->setToolTip("rubber");
    ui->spray->setToolTip("spray");
    ui->pouring->setToolTip("pouring");

    // Figures
    ui->line->setToolTip("draw line");
    ui->rectangle->setToolTip("draw rectangle");
    ui->elipse->setToolTip("draw elipse");
    ui->triangle->setToolTip("draw triangle");

    // Size
    ui->size->setToolTip("set size");

    // Color
    ui->colors->setToolTip("set color");
}

void MainWindow::createMenu()
{
    // FILE SUBMENU
    QMenu *fileSubmenu = this->menuBar()->addMenu(tr("File"));

    // Save image
    QAction *saveAsAction = new QAction(tr("Save as..."), this);
    saveAsAction->setShortcut(tr("Ctrl+S"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(save_img()));
    fileSubmenu->addAction(saveAsAction);

    // Open image
    QAction *openAsAction = new QAction(tr("Open"), this);
    openAsAction->setShortcut(tr("Ctrl+O"));
    connect(openAsAction, SIGNAL(triggered()), this, SLOT(open_img()));
    fileSubmenu->addAction(openAsAction);

    fileSubmenu->addSeparator();

    // Exit
    QAction *exitAction = new QAction(tr("Exit"), this);
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    fileSubmenu->addAction(exitAction);

    // HELP SUBMENU
    QMenu *helpSubmenu = this->menuBar()->addMenu("Help");

    // About
    QAction *aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(show_about()));
    helpSubmenu->addAction(aboutAction);
}

void MainWindow::save_img(){
    QString outName = QFileDialog::getSaveFileName(
                this, "Save file", tr("C:/Users/User/Desktop"), "Image (*.png)");
    QPixmap pixmap = QPixmap::grabWidget(_view);
    pixmap.save(outName);
}

void MainWindow::open_img(){
    QString inName = QFileDialog::getOpenFileName(
                this, "Open file", tr("C:/Users/User/Desktop"), "Image (*.png;*.jpg)");
    QImage img(inName);

    int widthDiff = img.width()/1110 + 1;
    int heightDiff = img.height()/580 + 1;
    int maxDiff = widthDiff > heightDiff ? widthDiff : heightDiff;

    img = img.scaled(img.width()/maxDiff, img.height()/maxDiff);
    _scene->addPixmap(QPixmap::fromImage(img));
}

void MainWindow::show_about(){
    QString textAbout = "Graphic Editor\n(C++, Qt)\n\nDate: 29.05.2019\nAuthor: Volodymyr Osadchuk";

    QMessageBox::information(this, "About", textAbout);
}
