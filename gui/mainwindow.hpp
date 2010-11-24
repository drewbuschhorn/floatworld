#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtGui/QMainWindow>
#include <QtGui/QGraphicsView>
#include <QTimer>

#include "../src/misc.hpp"
#include "../src/pos.hpp"
#include "../src/matrix.hpp"
#include "../src/grid.hpp"
#include "../src/feeder.hpp"
#include "../src/shape.hpp"

#include "ui_mainwindow.h"
#include "widgets.hpp"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    ShapeFeeder feeder;
    Matrix adam;
    QTimer timer1;
    QTimer timer2;
    float speed;
    float stepper;
    float speed_multiplier;
    QActionGroup* action_group;

    MainWindow(QWidget *parent = 0);

private slots:
    void viewtype_set(QAction* action);
    void on_actionSlow_triggered();
    void on_actionPlay_triggered();
    void on_actionStop_triggered();
    void on_actionFast_triggered();
    void on_actionStep_triggered();
    void takeStep();
    void reportFPS();
};

#endif // MAINWINDOW_HPP