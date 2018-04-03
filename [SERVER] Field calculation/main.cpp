/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Data Visualization module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "data.h"
#include "calc.h"
#include "qcustomplot.h"
#include "socketserver.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QComboBox>
#include <QPushButton>
#include <QtWidgets/QMessageBox>
#include <QDoubleSpinBox>
#include <QTextEdit>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Q3DScatter *graph = new Q3DScatter();
    QWidget *container = QWidget::createWindowContainer(graph);

    if (!graph->hasContext()) {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return -1;
    }

    container->setMinimumSize(800, 600);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    QGridLayout *gridLayout = new QGridLayout();
    hLayout->addWidget(container,1);
    hLayout->addLayout(vLayout);
    vLayout->addLayout(gridLayout);

    QPushButton *rangeButton = new QPushButton(widget);
    rangeButton->setText(QStringLiteral("Toggle axis ranges"));

    QPushButton *startButton = new QPushButton(widget);
    startButton->setText(QStringLiteral("Start"));

    QPushButton *stopButton = new QPushButton(widget);
    stopButton->setText(QStringLiteral("End"));

    QDoubleSpinBox *EndValueBox = new QDoubleSpinBox(widget);
    EndValueBox->setDecimals(5);
    QDoubleSpinBox *StartValueBox = new QDoubleSpinBox(widget);
    StartValueBox->setDecimals(5);

    QCustomPlot* plot = new QCustomPlot(widget);
    QCPColorMap* colorMap = new QCPColorMap(plot->xAxis, plot->yAxis);
    plot->addPlottable(colorMap);

    colorMap->setGradient(QCPColorGradient::gpThermal);
    QCPColorScale *colorScale = new QCPColorScale(plot);
    colorScale->setGradient(QCPColorGradient::gpThermal);
    colorScale->setDataRange(QCPRange(0, 1));
    plot->plotLayout()->addElement(0, 1, colorScale);

    plot->rescaleAxes();
    plot->replot();
    plot->setMinimumSize(500, 500);
    plot->setMaximumSize(500, 500);
    vLayout->addWidget(plot);

    QTextEdit *msgBrowser = new QTextEdit(widget);
    msgBrowser->setReadOnly(true);

    QSpinBox *valuePlotBox = new QSpinBox(widget);
    valuePlotBox->setValue(0);

    QComboBox *typePlotBox = new QComboBox(widget);
    typePlotBox->addItem(QString("X"));
    typePlotBox->addItem(QString("Y"));
    typePlotBox->addItem(QString("Z"));
    typePlotBox->setCurrentIndex(2);

    gridLayout->addWidget(rangeButton,0 ,0);
    gridLayout->addWidget(EndValueBox, 1, 0);
    gridLayout->addWidget(StartValueBox, 2, 0);
    gridLayout->addWidget(startButton, 3, 0);
    gridLayout->addWidget(stopButton, 4, 0);
    gridLayout->addWidget(valuePlotBox, 5, 0);
    gridLayout->addWidget(typePlotBox, 6, 0);
    gridLayout->addWidget(msgBrowser, 0, 1, 7, 1);

    widget->setWindowTitle(QStringLiteral("[SERVER] Field calculation"));

    Data *graphData = new Data(graph);
    Calc *calc = new Calc(graphData, plot, colorMap, colorScale, valuePlotBox);

    QObject::connect(rangeButton, &QPushButton::clicked, graphData, &Data::toggleRanges);
    QObject::connect(startButton, &QPushButton::clicked, calc, &Calc::start);
    QObject::connect(stopButton, &QPushButton::clicked, calc, &Calc::stop);
    QObject::connect(EndValueBox, SIGNAL(valueChanged(double)), calc, SLOT(showFieldEnd(double)));
    QObject::connect(StartValueBox, SIGNAL(valueChanged(double)), calc, SLOT(showFieldStart(double)));
    QObject::connect(valuePlotBox, SIGNAL(valueChanged(int)), calc, SLOT(setValuePlot()));
    QObject::connect(typePlotBox, SIGNAL(currentIndexChanged(int)), calc, SLOT(setTypePlot(int)));

    SocketServer* server = new SocketServer(1234, widget);

    QObject::connect(server, SIGNAL(browserPrint(QString)), msgBrowser, SLOT(setPlainText(QString)));
    QObject::connect(server, SIGNAL(sendMessage(Message)), calc, SLOT(onMessageRecieved(Message)));

    EndValueBox->setValue(1.0);
    widget->show();
    return app.exec();
}
