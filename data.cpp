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
#include "const.h"

#include <QtDataVisualization/QScatterDataProxy>
#include <QtDataVisualization/Q3DScene>
#include <QtDataVisualization/Q3DCamera>
#include <QtDataVisualization/QScatter3DSeries>
#include <QtDataVisualization/Q3DTheme>

using namespace QtDataVisualization;

const int itemCount = 3000;

Data::Data(Q3DScatter *scatter)
    : m_graph(scatter),
      //! [1]
      m_inputHandler(new AxesInputHandler(scatter)),
      //! [1]
      m_autoAdjust(false)
{
    m_graph->activeTheme()->setType(Q3DTheme::ThemeEbony);
    m_graph->activeTheme()->setLabelBorderEnabled(true);
    m_graph->activeTheme()->setLabelBackgroundColor(QColor(QRgb(0x151550)));
    m_graph->activeTheme()->setLabelTextColor(Qt::lightGray);
    m_graph->activeTheme()->setFont(QFont("Arial Black", 30));
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualityMedium);
    m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetIsometricRight);

    m_graph->axisX()->setRange(0.0f, SIZE_X);
    m_graph->axisY()->setRange(0.0f, SIZE_Y);
    m_graph->axisZ()->setRange(0.0f, SIZE_Z);

    //! [0]
    // Give ownership of the handler to the graph and make it the active handler
    m_graph->setActiveInputHandler(m_inputHandler);
    //! [0]

    //! [2]
    // Give our axes to the input handler
    m_inputHandler->setAxes(m_graph->axisX(), m_graph->axisZ(), m_graph->axisY());
    //! [2]

    addData();
}

Data::~Data()
{
    delete m_graph;
}

void Data::toggleRanges()
{
    if (!m_autoAdjust) {
        m_graph->axisX()->setAutoAdjustRange(true);
        m_graph->axisZ()->setAutoAdjustRange(true);
        m_graph->axisY()->setAutoAdjustRange(true);
        m_inputHandler->setDragSpeedModifier(1.5f);
        m_autoAdjust = true;
    } else {
        m_graph->axisX()->setRange(0.0f, SIZE_X);
        m_graph->axisY()->setRange(0.0f, SIZE_Y);
        m_graph->axisZ()->setRange(0.0f, SIZE_Z);
        m_inputHandler->setDragSpeedModifier(15.0f);
        m_autoAdjust = false;
    }
}

void Data::addData()
{
    // Labels
    QScatter3DSeries *series = new QScatter3DSeries;
    series->setMesh(QAbstract3DSeries::MeshCube);
    series->setMeshSmooth(true);
    series->setItemSize(0.3);
    m_graph->addSeries(series);

    // Source
    QScatter3DSeries *series2 = new QScatter3DSeries;
    series2->setMesh(QAbstract3DSeries::MeshMinimal);
    series2->setMeshSmooth(true);
    series2->setItemSize(0.3);
    m_graph->addSeries(series2);

    // Direction of diagram
    QScatter3DSeries *series3 = new QScatter3DSeries;
    series3->setMesh(QAbstract3DSeries::MeshSphere);
    series3->setMeshSmooth(true);
    series3->setItemSize(0.23);
    m_graph->addSeries(series3);

    // Room
    QScatter3DSeries *series4 = new QScatter3DSeries;
    series4->setMesh(QAbstract3DSeries::MeshBevelCube);
    series4->setMeshSmooth(true);
    series4->setItemSize(0.001);
    m_graph->addSeries(series4);

    // Points
    QScatter3DSeries *series5 = new QScatter3DSeries;
    series5->setMesh(QAbstract3DSeries::MeshSphere);
    series5->setItemSize(0.07);
    m_graph->addSeries(series5);
}

void Data::updateData(int series, QScatterDataArray *dataArray)
{
    m_graph->seriesList().at(series)->dataProxy()->resetArray(dataArray);
}
