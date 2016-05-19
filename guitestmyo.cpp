#include "guitestmyo.h"
#include "ui_GUImyo.h"

#include <QString>
#include <QVector>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;


GUItestMyo::GUItestMyo(TestMyo *prec, QWidget *parent) : QWidget(parent), ui(new Ui::GUImyo), _pGestureRec(prec){

    ui->setupUi(this);

    ui->chart1->addGraph();
    ui->chart2->addGraph();
    ui->chart3->addGraph();
    ui->chart4->addGraph();
    ui->chart5->addGraph();
    ui->chart6->addGraph();
    ui->chart7->addGraph();
    ui->chart8->addGraph();

    ui->chartOr->addGraph();
    ui->chartOr->addGraph();
    ui->chartOr->addGraph();
    ui->chartOr->graph(0)->setPen(QPen(Qt::blue));
    ui->chartOr->graph(1)->setPen(QPen(Qt::red));
    ui->chartOr->graph(2)->setPen(QPen(Qt::green));
    //ui->chart1->graph(0)->setPen(QPen(Qt::blue));

    //General setting chart
    ui->chart1->xAxis->setRange(0, 800);
    ui->chart1->yAxis->setRange(-100, 100);

    ui->chart2->xAxis->setRange(0, 800);
    ui->chart2->yAxis->setRange(-100, 100);

    ui->chart3->xAxis->setRange(0, 800);
    ui->chart3->yAxis->setRange(-100, 100);

    ui->chart4->xAxis->setRange(0, 800);
    ui->chart4->yAxis->setRange(-100, 100);

    ui->chart5->xAxis->setRange(0, 800);
    ui->chart5->yAxis->setRange(-100, 100);

    ui->chart6->xAxis->setRange(0, 800);
    ui->chart6->yAxis->setRange(-100, 100);

    ui->chart7->xAxis->setRange(0, 800);
    ui->chart7->yAxis->setRange(-100, 100);

    ui->chart8->xAxis->setRange(0, 800);
    ui->chart8->yAxis->setRange(-100, 100);

    ui->chartOr->xAxis->setRange(0, 800);
    ui->chartOr->yAxis->setRange(-25, 25);

//    ui->chart1->xAxis->setLabel("Frames");
//    ui->chart1->yAxis->setLabel("Muscle 1");
//    ui->chart1->legend->setVisible(true);

    ui->chart1->yAxis->setVisible(false);
    ui->chart2->yAxis->setVisible(false);
    ui->chart3->yAxis->setVisible(false);
    ui->chart4->yAxis->setVisible(false);
    ui->chart5->yAxis->setVisible(false);
    ui->chart6->yAxis->setVisible(false);
    ui->chart7->yAxis->setVisible(false);
    ui->chart8->yAxis->setVisible(false);
    ui->chartOr->yAxis->setVisible(false);


    connect(_pGestureRec, SIGNAL(FrameAcquired()), this, SLOT(UpdateInterface()));

}


void GUItestMyo::UpdateInterface(){

    QString str = QString::number(_pGestureRec->_frame.myoPitch);
    ui->label->setText(str);

    if(numFrames > 800){

        numFrames = 0;

    ui->chart1->graph(0)->clearData();
    ui->chart2->graph(0)->clearData();
    ui->chart3->graph(0)->clearData();
    ui->chart4->graph(0)->clearData();
    ui->chart5->graph(0)->clearData();
    ui->chart6->graph(0)->clearData();
    ui->chart7->graph(0)->clearData();
    ui->chart8->graph(0)->clearData();

    ui->chartOr->graph(0)->clearData();
    ui->chartOr->graph(1)->clearData();
    ui->chartOr->graph(2)->clearData();

    m1.clear();
    m2.clear();
    m3.clear();
    m4.clear();
    m5.clear();
    m6.clear();
    m7.clear();
    m8.clear();

    pitch.clear();
    roll.clear();
    yaw.clear();

    }

    numFrames++;

    frames.push_back(numFrames);

    m1.push_back(_pGestureRec->_frame.muscles[0]);
    m2.push_back(_pGestureRec->_frame.muscles[1]);
    m3.push_back(_pGestureRec->_frame.muscles[2]);
    m4.push_back(_pGestureRec->_frame.muscles[3]);
    m5.push_back(_pGestureRec->_frame.muscles[4]);
    m6.push_back(_pGestureRec->_frame.muscles[5]);
    m7.push_back(_pGestureRec->_frame.muscles[6]);
    m8.push_back(_pGestureRec->_frame.muscles[7]);

    pitch.push_back(_pGestureRec->_frame.myoPitch);
    roll.push_back(_pGestureRec->_frame.myoRoll);
    yaw.push_back(_pGestureRec->_frame.myoYaw);



    ui->chart1->graph(0)->setData(frames, m1);
    ui->chart1->graph(0)->setName("EMG Data");
    ui->chart2->graph(0)->setData(frames, m2);
    ui->chart2->graph(0)->setName("EMG Data");
    ui->chart3->graph(0)->setData(frames, m3);
    ui->chart3->graph(0)->setName("EMG Data");
    ui->chart4->graph(0)->setData(frames, m4);
    ui->chart4->graph(0)->setName("EMG Data");
    ui->chart5->graph(0)->setData(frames, m5);
    ui->chart5->graph(0)->setName("EMG Data");
    ui->chart6->graph(0)->setData(frames, m6);
    ui->chart6->graph(0)->setName("EMG Data");
    ui->chart7->graph(0)->setData(frames, m7);
    ui->chart7->graph(0)->setName("EMG Data");
    ui->chart8->graph(0)->setData(frames, m8);
    ui->chart8->graph(0)->setName("EMG Data");

    ui->chartOr->graph(0)->setData(frames, pitch);
    ui->chartOr->graph(1)->setData(frames, roll);
    ui->chartOr->graph(2)->setData(frames, yaw);


    // For all charts
    ui->chart1->replot();
    ui->chart2->replot();
    ui->chart3->replot();
    ui->chart4->replot();
    ui->chart5->replot();
    ui->chart6->replot();
    ui->chart7->replot();
    ui->chart8->replot();
    ui->chartOr->replot();

}
