#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "medium.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *);
    Medium itsMedium;

public slots:
    void elektrostriction(int);
    void changeAxis();
private slots:
    void on_horizontalSlider_3_valueChanged(int value);
    void on_lineEdit_6_textEdited();
    void on_radioButton_2_clicked();

    void on_lineEdit_6_textEdited(const QString &arg1);
};

#endif // MAINWINDOW_H
