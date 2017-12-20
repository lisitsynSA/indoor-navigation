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

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QPushButton>
#include <QtWidgets/QMessageBox>
#include <QDoubleSpinBox>

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
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);

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

    QDoubleSpinBox *XvalueBox = new QDoubleSpinBox(widget);
    XvalueBox->setValue(X0);
    QDoubleSpinBox *YvalueBox = new QDoubleSpinBox(widget);
    YvalueBox->setValue(Y0);
    QDoubleSpinBox *ZvalueBox = new QDoubleSpinBox(widget);
    ZvalueBox->setValue(Z0);
    QPushButton *updateButton = new QPushButton(widget);
    updateButton->setText(QStringLiteral("Update"));


    vLayout->addWidget(rangeButton, 1, Qt::AlignTop);
    vLayout->addWidget(startButton, 1, Qt::AlignTop);
    vLayout->addWidget(stopButton, 1, Qt::AlignTop);
    vLayout->addWidget(EndValueBox, 1, Qt::AlignTop);
    vLayout->addWidget(StartValueBox, 1, Qt::AlignTop);

    vLayout->addSpacing(10);
    vLayout->addWidget(XvalueBox, 1, Qt::AlignBottom);
    vLayout->addWidget(YvalueBox, 1, Qt::AlignBottom);
    vLayout->addWidget(ZvalueBox, 1, Qt::AlignBottom);
    vLayout->addWidget(updateButton, 1, Qt::AlignBottom);

    widget->setWindowTitle(QStringLiteral("Input Handling for Axes"));

    Data *graphData = new Data(graph);
    Calc *calc = new Calc(graphData);

    QObject::connect(rangeButton, &QPushButton::clicked, graphData, &Data::toggleRanges);
    QObject::connect(startButton, &QPushButton::clicked, calc, &Calc::start);
    QObject::connect(stopButton, &QPushButton::clicked, calc, &Calc::stop);
    QObject::connect(EndValueBox, SIGNAL(valueChanged(double)), calc, SLOT(showFieldEnd(double)));
    QObject::connect(StartValueBox, SIGNAL(valueChanged(double)), calc, SLOT(showFieldStart(double)));
    QObject::connect(XvalueBox, SIGNAL(valueChanged(double)), calc, SLOT(setX(double)));
    QObject::connect(YvalueBox, SIGNAL(valueChanged(double)), calc, SLOT(setY(double)));
    QObject::connect(ZvalueBox, SIGNAL(valueChanged(double)), calc, SLOT(setZ(double)));
    QObject::connect(updateButton, &QPushButton::clicked, calc, &Calc::updateSource);


    widget->show();
    return app.exec();
}
