#include "groundingsystemreport.h"
#include <groundingsystem.h>
#include "groundingsystemgraph.h"
#include "database.h"
#include <qcustomplot.h>
#include <QFile>

GroundingSystemReport::GroundingSystemReport()
{

}

void GroundingSystemReport::generateResources()
{
    QDir prev = QCoreApplication::applicationDirPath();
    prev.cd("../../assets/resultImages");
    QString path = prev.path() + "/";
    int gsCount = 0;
    for(GroundingSystemPtr gs : g_database->getGroundingSystems()) {
        gsCount++;
        bool hasCurrent = gs->getInjectedCurrent() > 0;
        if(!hasCurrent)
            continue;

        const QVector<SurfaceVoltageProfile<Vector3Dd>>& profiles = gs->getSurfaceVoltageProfiles();
        int profileCount = 0;
        for(const SurfaceVoltageProfile<Vector3Dd>& profile : profiles) {
            profileCount++;

            if(profile.step) {
                QString surfaceFilename = QString("groundingsystem_surface_%1_%2.png").arg(gsCount).arg(profileCount);
                saveSurfaceVoltagePlot(gs, profile, path + surfaceFilename);
            }

            if(profile.touch) {
                QString touchFilename = QString("groundingsystem_touch_%1_%2.png").arg(gsCount).arg(profileCount);
                saveTouchVoltagePlot(gs, profile, path + touchFilename);
            }
        }

        GroundingSystemGraph *groundingSystemGraph = new GroundingSystemGraph();
        groundingSystemGraph->setGroundingSystem(gs);
        groundingSystemGraph->exportImage(path+"graph_"+QString::number(gsCount)+".png");
    }
}

void GroundingSystemReport::saveSurfaceVoltagePlot(GroundingSystemPtr groundingSystem, const SurfaceVoltageProfile<Vector3Dd>& profile, const QString& filename)
{
    QFile::remove(filename);
    double min = 0;
    double max = 0;
    double limit = groundingSystem->getResistance() * groundingSystem->getInjectedCurrent();

    QVector<double> xList;
    QVector<double> ySurfaceVoltage;

    double x0 = -profile.stepSearchMargin;
    double x1 = profile.pi.distanceTo(profile.pf) + profile.stepSearchMargin;
    Vector3Dd dir = (profile.pf - profile.pi).normalized();

    for(double x = x0; x <= x1; x += profile.precision) {
        Vector3Dd point = profile.pi + dir * x;
        double v = groundingSystem->getSurfaceVoltage(point);
        xList.push_back(x);
        ySurfaceVoltage.push_back(v);

        if(x == x0) {
            min = v;
            max = v;
        }
        else {
            min = std::min(min, v);
            max = std::max(max, v);
        }
    }

    max = std::max(max, limit);
    min = std::min(min, limit);

    double yHeight = max - min;
    max += yHeight * 0.05;
    min -= yHeight * 0.05;

    QCustomPlot customPlot;
    customPlot.plotLayout()->insertRow(0);
    customPlot.plotLayout()->addElement(0, 0, new QCPPlotTitle(&customPlot, "Perfil de potencial superficial"));
    customPlot.xAxis->setLabel("Posição no perfil (m)");
    customPlot.yAxis->setLabel("Potencial (V)");
    customPlot.xAxis->setRange(xList.first(), xList.last());
    customPlot.yAxis->setRange(min, max);

    customPlot.replot();
    double tickStep = customPlot.yAxis->tickStep();
    double nMax = std::ceil(max / tickStep) * tickStep;
    double nMin = std::floor(min / tickStep) * tickStep;
    customPlot.yAxis->setRange(nMin, nMax);

    QCPGraph *surfaceVoltageGraph = customPlot.addGraph();
    surfaceVoltageGraph->setData(xList, ySurfaceVoltage);

    QCPGraph *graphLimit = customPlot.addGraph();
    graphLimit->setData({xList.first(), xList.last()}, {limit, limit});
    graphLimit->setPen(QPen(Qt::black, 0, Qt::DashDotLine));

    customPlot.yAxis2->setVisible(true);
    customPlot.yAxis2->setAutoTicks(false);
    customPlot.yAxis2->setAutoTickLabels(false);
    customPlot.yAxis2->setTickVector({ limit });
    customPlot.yAxis2->setTickVectorLabels({ QString("%1").arg(limit, 1, 'f', 1) });
    customPlot.yAxis2->setRange(nMin, nMax);

    double stepX1 = (profile.maxStepVoltagePos1 - profile.pi).dotProduct(dir);
    double stepX2 = (profile.maxStepVoltagePos2 - profile.pi).dotProduct(dir);

    QCPItemTracer *stepTracer1 = new QCPItemTracer(&customPlot);
    customPlot.addItem(stepTracer1);
    stepTracer1->setGraph(surfaceVoltageGraph);
    stepTracer1->setGraphKey(stepX1);
    stepTracer1->setInterpolating(true);
    stepTracer1->setStyle(QCPItemTracer::tsCircle);
    stepTracer1->setPen(QPen(Qt::black));
    stepTracer1->setBrush(Qt::black);
    stepTracer1->setSize(5);

    QCPItemTracer *stepTracer2 = new QCPItemTracer(&customPlot);
    customPlot.addItem(stepTracer2);
    stepTracer2->setGraph(surfaceVoltageGraph);
    stepTracer2->setGraphKey(stepX2);
    stepTracer2->setInterpolating(true);
    stepTracer2->setStyle(QCPItemTracer::tsCircle);
    stepTracer2->setPen(QPen(Qt::black));
    stepTracer2->setBrush(Qt::black);
    stepTracer2->setSize(5);

    customPlot.replot();
    customPlot.savePng(filename, 600, 300);
}

void GroundingSystemReport::saveTouchVoltagePlot(GroundingSystemPtr groundingSystem, const SurfaceVoltageProfile<Vector3Dd>& profile, const QString& filename)
{
    QFile::remove(filename);
    double min = 0;
    double max = 0;
    double limit = groundingSystem->getResistance() * groundingSystem->getInjectedCurrent();

    QVector<double> xList;
    QVector<double> yTouchVoltage;

    double x0 = 0;
    double x1 = profile.pi.distanceTo(profile.pf);
    Vector3Dd dir = (profile.pf - profile.pi).normalized();

    for(double x = x0; x <= x1; x += profile.precision) {
        Vector3Dd point = profile.pi + dir * x;
        double v = limit - groundingSystem->getSurfaceVoltage(point);
        xList.push_back(x);
        yTouchVoltage.push_back(v);

        if(x == x0) {
            min = v;
            max = v;
        }
        else {
            min = std::min(min, v);
            max = std::max(max, v);
        }
    }

    double yHeight = max - min;
    max += yHeight * 0.05;
    min -= yHeight * 0.05;

    QCustomPlot customPlot;
    customPlot.plotLayout()->insertRow(0);
    customPlot.plotLayout()->addElement(0, 0, new QCPPlotTitle(&customPlot, "Perfil de potencial de toque"));
    customPlot.xAxis->setLabel("Posição no perfil (m)");
    customPlot.yAxis->setLabel("Potencial (V)");
    customPlot.xAxis->setRange(xList.first(), xList.last());
    customPlot.yAxis->setRange(min, max);

    customPlot.replot();
    double tickStep = customPlot.yAxis->tickStep();
    double nMax = std::ceil(max / tickStep) * tickStep;
    double nMin = std::floor(min / tickStep) * tickStep;
    customPlot.yAxis->setRange(nMin, nMax);

    QCPGraph *touchVoltageGraph = customPlot.addGraph();
    touchVoltageGraph->setData(xList, yTouchVoltage);
    touchVoltageGraph->setPen(QPen(Qt::red, 0));

    double touchX = (profile.maxTouchVoltagePos - profile.pi).dotProduct(dir);

    QCPItemTracer *touchTracer1 = new QCPItemTracer(&customPlot);
    customPlot.addItem(touchTracer1);
    touchTracer1->setGraph(touchVoltageGraph);
    touchTracer1->setGraphKey(touchX);
    touchTracer1->setInterpolating(true);
    touchTracer1->setStyle(QCPItemTracer::tsCircle);
    touchTracer1->setPen(QPen(Qt::black));
    touchTracer1->setBrush(Qt::black);
    touchTracer1->setSize(5);

    customPlot.replot();
    customPlot.savePng(filename, 600, 300);
}
