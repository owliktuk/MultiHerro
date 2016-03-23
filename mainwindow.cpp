#include "mainwindow.h"
#include <cmath>
#include "ui_mainwindow.h"
#include <QPainter>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_series_data.h>
#include "medium.cpp"
#include "countMagnetisationInPlane.cpp"
#include "countMagnetisation.cpp"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    double E[6][6] = {  2.465E11,   1.473E11,   1.473E11,   0,          0,          0,
                        1.473E11,   2.465E11,   1.473E11,   0,          0,          0,
                        1.473E11,   1.473E11,   2.465E11,   0,          0,          0,
                        0,          0,          0,          1.247E11,   0,          0,
                        0,          0,          0,          0,          1.247E11,   0,
                        0,          0,          0,          0,          0,          1.247E11 };

    double MD[3] = {0.6,0.6,0};
    double LC[3] = {3.53,3.53,3.53};
    double ME[2] = {9.2E6, 10.2E6};
    double Anis[2] = {-0.45E4+ME[0]*ME[0]*E[0][0]/((E[0][0]-E[0][1])*(E[0][0]+2*E[0][1]))+ME[1]*ME[1]/2/E[3][3], -0.23E4};

    double PE[3][3][3];
    PE[2][0][0] = -1000E-12;
    PE[2][1][1] = PE[2][0][0];
    PE[2][2][2] = 2000E-12;
    double LC2[3] = {3.5465,3.5465,3.5465};

    FerromagneticFilm FMF(E, 4.85E5, MD, LC, Anis, ME);
    Ferroelectric FE(PE, LC2);

    itsMedium.setFerromagneticFilm(FMF);
    itsMedium.setFerroelectric(FE);

    ui->qwtPlot->setAxisScale(0, -1, 1, 0);
    ui->qwtPlot->setAxisScale(2, -50000, 50000, 0);
    ui->qwtPlot->setAxisTitle(0, "Magnetisation cosines");
    ui->qwtPlot->setAxisTitle(2, "Electric field [V/cm]");
    ui->qwtPlot->replot();

    //ui->qwtPlot_2->setAxisScale(2, 0, 3.14159, 0);
    ui->qwtPlot_2->setAxisTitle(0, "Free Energy [a.u.]");
    ui->qwtPlot_2->setAxisTitle(2, "Angle [rad]");
    ui->qwtPlot_2->replot();


    connect(ui->lineEdit_6, SIGNAL(textEdited(QString)), this, SLOT(on_lineEdit_6_textEdited()));
    connect(ui->lineEdit_7, SIGNAL(textEdited(QString)), this, SLOT(on_lineEdit_6_textEdited()));
    connect(ui->lineEdit_8, SIGNAL(textEdited(QString)), this, SLOT(on_lineEdit_6_textEdited()));
    connect(ui->radioButton, SIGNAL(clicked()), this, SLOT(on_radioButton_2_clicked()));
    connect(ui->radioButton_3, SIGNAL(clicked()), this, SLOT(on_radioButton_2_clicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{

}

void MainWindow::elektrostriction(int ElectricField)
{
    QPainter painter(this);
    painter.setPen(Qt::blue);
    painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
    painter.drawRect(50, 50, ElectricField, 100);
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    //wybieranie sposobu przylozenia pola elektrycznego
    if(ui->radioButton->isChecked())                    //out of plane [001]
    {
        itsMedium.countMagnetisation(0,0,value);
        ui->qwtPlot->setAxisScale(2, -50000, 50000, 0);
        ui->horizontalSlider_3->setMaximum(50000);
        ui->horizontalSlider_3->setMinimum(-50000);
        ui->qwtPlot->setAxisScale(0, -1, 1, 0);
    }
    if(ui->radioButton_2->isChecked())                  //in plane [010]
    {
        itsMedium.countMagnetisation(value,0,0);
        ui->qwtPlot->setAxisScale(2, -50000, 50000, 0);
        ui->horizontalSlider_3->setMaximum(50000);
        ui->horizontalSlider_3->setMinimum(-50000);
        ui->qwtPlot->setAxisScale(0, -1, 1, 0);
    }
    if(ui->radioButton_3->isChecked())                  //in plane [110]
    {
        itsMedium.countMagnetisationInPlane110(value/2);
        ui->qwtPlot->setAxisScale(2, -3000, 3000, 0);
        ui->horizontalSlider_3->setMaximum(3000);
        ui->horizontalSlider_3->setMinimum(-3000);
        ui->qwtPlot->setAxisScale(0, -1, 1, 0);
    }
    double m[3];
    itsMedium.getMagDirection(m);

    double Def[6];
    itsMedium.getDeformation(Def);
    ui->lineEdit->setText(QString::number(Def[0]*100));
    ui->lineEdit_2->setText(QString::number(Def[1]*100));
    ui->lineEdit_6->setText(QString::number(m[0]));
    ui->lineEdit_7->setText(QString::number(m[1]));

    if(!(ui->radioButton_3->isChecked()))
    {
        ui->lineEdit_8->setText(QString::number(m[2]));         // wykres magnetyzacji m3
        QwtPlotMarker* mkr = new QwtPlotMarker();
        mkr->setSymbol(new QwtSymbol(QwtSymbol::Diamond, Qt::red, QPen(Qt::red), QSize( 10, 10 ) ) );
        mkr->setValue( QPointF( value, m[2] ) );
        mkr->attach( ui->qwtPlot );
    }

    QwtPlotMarker* mkr2 = new QwtPlotMarker();                  //wykres magnetyzacji m2
    mkr2->setSymbol(new QwtSymbol( QwtSymbol::Diamond, Qt::blue, QPen(Qt::blue), QSize( 10, 10 ) ) );
    mkr2->setValue( QPointF( value, m[1] ) );
    mkr2->attach( ui->qwtPlot );
    QwtPlotMarker* mkr3 = new QwtPlotMarker();                  //wykres magnetyzacji m1
    mkr3->setSymbol(new QwtSymbol( QwtSymbol::Diamond, Qt::darkGreen, QPen(Qt::darkGreen), QSize( 10, 10 ) ) );
    mkr3->setValue( QPointF( value, m[0] ) );
    mkr3->attach( ui->qwtPlot );
    ui->qwtPlot->replot();


    //wykres energii
    int s = itsMedium.getEnergySize();

    //kat theta
    QwtPlotCurve curve("Sine");
    QwtPlotMarker mkr4;
    if(!(ui->radioButton_3->isChecked())) // jesli nie in-plane [110]
    {
        // Energia od kata theta
        double xs[s];
        double ys[s];
        for (int i = 0; i < s; i++)
        {
            xs[i] = itsMedium.getEnergy(1, i);
            ys[i] = itsMedium.getEnergy(3, i);
        }
        curve.setPen(QPen(Qt::darkYellow, 3));
        curve.setStyle( QwtPlotCurve::Lines );
        curve.setVisible(true);
        curve.setSamples(xs, ys, s);
        curve.attach(ui->qwtPlot_2);

        //znacznik minimum energii dla theta
        mkr4.setSymbol(new QwtSymbol(QwtSymbol::Diamond, Qt::darkYellow, QPen(Qt::black), QSize( 20, 20 ) ) );
        mkr4.setValue( QPointF(  itsMedium.getEnergy(0, 1), itsMedium.getEnergy(3, itsMedium.getEnergy(0, 1)) ) );
        mkr4.attach( ui->qwtPlot_2 );
    }

    QwtPlotCurve curve2("Sine2");               // Energia od kata phi
    double xs2[s];
    double ys2[s];
    for (int i = 0; i < s; i++)
    {
        xs2[i] = itsMedium.getEnergy(2, i);
        ys2[i] = itsMedium.getEnergy(4, i);
    }
    curve2.setPen(QPen(Qt::yellow, 3));
    curve2.setStyle( QwtPlotCurve::Lines );
    curve2.setVisible(true);
    curve2.setSamples(xs2, ys2, s);
    curve2.attach(ui->qwtPlot_2);

    QwtPlotMarker mkr5;                         //znacznik minimum energii dla phi
    mkr5.setSymbol(new QwtSymbol( QwtSymbol::Diamond, Qt::yellow, QPen(Qt::red), QSize( 20, 20 ) ) );
    mkr5.setValue( QPointF( itsMedium.getEnergy(0, 0), itsMedium.getEnergy(4, itsMedium.getEnergy(0, 0)) ) );
    mkr5.attach( ui->qwtPlot_2 );

    ui->qwtPlot_2->replot();
    this->repaint();

}

void MainWindow::on_lineEdit_6_textEdited()
{

}

void MainWindow::on_radioButton_2_clicked()
{
    ui->qwtPlot->detachItems(QwtPlotItem::Rtti_PlotItem);
    ui->horizontalSlider_3->setValue(0);
}

void MainWindow::changeAxis()
{
    ui->horizontalSlider_3->setEnabled(true);

    if(ui->radioButton_3->isChecked())                  //in plane [110]
    {
        ui->qwtPlot->setAxisScale(0, -1, 1, 0);
        ui->qwtPlot->setAxisScale(2, -2500, 2500, 0);
        ui->qwtPlot_2->setAxisScale(2, 0, 2*3.14159, 0);
    }
    else {
        ui->qwtPlot->setAxisScale(0, 0, 1, 0);
        ui->qwtPlot_2->setAxisScale(2, 0, 3.14159, 0);
    }
}

void MainWindow::on_lineEdit_6_textEdited(const QString &arg1)
{
    double m[] = {ui->lineEdit_6->text().toDouble(), ui->lineEdit_7->text().toDouble(), ui->lineEdit_8->text().toDouble()};
    itsMedium.setMagDirection(m);
}

