/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QComboBox>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#if QT_CONFIG(printpreviewdialog)
#include <QPrintPreviewDialog>
#endif
#endif
#endif

#include "mainwindow.h"
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include "testmodel.h"
#include "FSP.h"


using std::string;
using std::filesystem::directory_iterator;

TestModel *PhoneBookModel;
MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
{
    setupUi(this);

    sampleSizes << 32 << 24 << 16 << 14 << 12 << 8 << 4 << 2 << 1;
    markedCount = 0;
    setupFontTree();

    connect(quitAction, &QAction::triggered,
            qApp, &QApplication::quit);
    connect(fontTree, &QTreeWidget::currentItemChanged,
            this, &MainWindow::showFont);
    connect(fontTree, &QTreeWidget::itemChanged,
            this, &MainWindow::updateStyles);
    connect(pushButton,SIGNAL(clicked()),this, SLOT(clickedSlot()));
    connect(pushButton_2,SIGNAL(clicked()),this, SLOT(clickedClear()));

    fontTree->topLevelItem(0)->setSelected(true);
    showFont(fontTree->topLevelItem(0));
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

M_code MainWindow::hashit (std::string const& inString) {
    if (inString == "Branch and Bound") return eBB;
    if (inString == "Parallel Branch and Bound") return epBB;
    if (inString == "CDS") return eCDS;
    if (inString == "NEH") return eNEH;
    if (inString == "Chen") return eCHEN;
    if (inString == "Recuit Simule") return eRS;
    if (inString == "Recherche Tabou") return eRT;
    if (inString == "genetique (evol)") return eG;

}
void MainWindow::setupFontTree()
{
    fontTree->setColumnCount(1);
    fontTree->setHeaderLabels({ tr("Methods") });

    const QStringList Methods = {"Exact","Heuristique","Meta-heuristique"};
    for (const QString &family : Methods) {
        QStringList styles;
        if(family=="Exact")
        {
            styles = {"Branch and Bound","Parallel Branch and Bound"};
        }
        if(family=="Heuristique")
        {
             styles = {"CDS","Chen","NEH"};
        }
        if(family=="Meta-heuristique")
        {
             styles = {"Recuit Simule","Recherche Tabou","genetique (evol)"};
        }


        QTreeWidgetItem *familyItem = new QTreeWidgetItem(fontTree);
        familyItem->setText(0, family);
        familyItem->setCheckState(0, Qt::Unchecked);
        familyItem->setFlags(familyItem->flags() | Qt::ItemIsAutoTristate);

        for (const QString &style : styles) {
            QTreeWidgetItem *styleItem = new QTreeWidgetItem(familyItem);
            styleItem->setText(0, style);
            styleItem->setCheckState(0, Qt::Unchecked);
            styleItem->setData(0, Qt::UserRole, QVariant(QFontDatabase::weight(family, style)));
            styleItem->setData(0, Qt::UserRole + 1, QVariant(QFontDatabase::italic(family, style)));
        }
    }


    string path = "../fontsampler/benchmarks";
     QStringList benchs;

    for (const auto & file : directory_iterator(path))
        benchs.emplace_back(QString::fromStdString(file.path().filename().string()));
        //std::cout << file.path() << std::endl;

    comboBox->addItems(benchs);


    //
        QList<QString> Mnames;
        QList<QString> ben;
        QList<QString> cm;
        QList<QString> t;

        // Create some data that is tabular in nature:



        // Create model:
        PhoneBookModel = new TestModel(this);

        // Populate model with data:
        PhoneBookModel->populateData(Mnames,ben,cm,t);


        // Connect model to table view:
        tableView->setModel(PhoneBookModel);
        //PhoneBookModel->Add_row("BB","2job5mechyines","256","5745522");

        // Make table header visible and display table:
        tableView->horizontalHeader()->setVisible(true);
        tableView->show();
}

void MainWindow::on_clearAction_triggered()
{
    const QList<QTreeWidgetItem *> items = fontTree->selectedItems();
    for (QTreeWidgetItem *item : items)
        item->setSelected(false);
    fontTree->currentItem()->setSelected(true);
}

void MainWindow::on_markAction_triggered()
{
    markUnmarkFonts(Qt::Checked);
}

void MainWindow::on_unmarkAction_triggered()
{
    markUnmarkFonts(Qt::Unchecked);
}

void MainWindow::markUnmarkFonts(Qt::CheckState state)
{
    const QList<QTreeWidgetItem *> items = fontTree->selectedItems();
    for (QTreeWidgetItem *item : items) {
        if (item->checkState(0) != state)
            item->setCheckState(0, state);
    }
}

void MainWindow::showFont(QTreeWidgetItem *item)
{

}

void MainWindow::updateStyles(QTreeWidgetItem *item, int column)
{
    if (!item || column != 0)
        return;

    Qt::CheckState state = item->checkState(0);
    QTreeWidgetItem *parent = item->parent();

    if (parent) {
        // Only count style items.
        if (state == Qt::Checked)
            ++markedCount;
        else
            --markedCount;
    }

    printAction->setEnabled(markedCount > 0);
    printPreviewAction->setEnabled(markedCount > 0);
}

QMap<QString, StyleItems> MainWindow::currentPageMap()
{
    QMap<QString, StyleItems> pageMap;

    for (int row = 0; row < fontTree->topLevelItemCount(); ++row) {
        QTreeWidgetItem *familyItem = fontTree->topLevelItem(row);
        QString family;

        if (familyItem->checkState(0) == Qt::Checked) {
            family = familyItem->text(0);
            pageMap[family] = StyleItems();
        }

        for (int childRow = 0; childRow < familyItem->childCount(); ++childRow) {
            QTreeWidgetItem *styleItem = familyItem->child(childRow);
            if (styleItem->checkState(0) == Qt::Checked)
                pageMap[family].append(styleItem);
        }
    }

    return pageMap;
}

void MainWindow::on_printAction_triggered()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    pageMap = currentPageMap();

    if (pageMap.count() == 0)
        return;

    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    int from = printer.fromPage();
    int to = printer.toPage();
    if (from <= 0 && to <= 0)
        printer.setFromTo(1, pageMap.keys().count());

    printDocument(&printer);
#endif
}

void MainWindow::printDocument(QPrinter *printer)
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    printer->setFromTo(1, pageMap.count());

    QProgressDialog progress(tr("Preparing font samples..."), tr("&Cancel"),
                             0, pageMap.count(), this);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setWindowTitle(tr("Font Sampler"));
    progress.setMinimum(printer->fromPage() - 1);
    progress.setMaximum(printer->toPage());

    QPainter painter;
    painter.begin(printer);
    bool firstPage = true;

    for (int page = printer->fromPage(); page <= printer->toPage(); ++page) {

        if (!firstPage)
            printer->newPage();

        qApp->processEvents();
        if (progress.wasCanceled())
            break;

        printPage(page - 1, &painter, printer);
        progress.setValue(page);
        firstPage = false;
    }

    painter.end();
#endif
}

void MainWindow::on_printPreviewAction_triggered()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printpreviewdialog)
    pageMap = currentPageMap();

    if (pageMap.count() == 0)
        return;

    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, &QPrintPreviewDialog::paintRequested,
            this, &MainWindow::printDocument);
    preview.exec();
#endif
}

void MainWindow::printPage(int index, QPainter *painter, QPrinter *printer)
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    const QString family = std::next(pageMap.begin(), index).key();
    const StyleItems items = pageMap.value(family);

    // Find the dimensions of the text on each page.
    qreal width = 0.0;
    qreal height = 0.0;
    for (const QTreeWidgetItem *item : items) {
        QString style = item->text(0);
        int weight = item->data(0, Qt::UserRole).toInt();
        bool italic = item->data(0, Qt::UserRole + 1).toBool();

        // Calculate the maximum width and total height of the text.
        for (int size : qAsConst(sampleSizes)) {
            QFont font(family, size, weight, italic);
            font.setStyleName(style);
            font = QFont(font, painter->device());
            QFontMetricsF fontMetrics(font);
            QRectF rect = fontMetrics.boundingRect(
            QString("%1 %2").arg(family).arg(style));
            width = qMax(rect.width(), width);
            height += rect.height();
        }
    }

    qreal xScale = printer->pageRect(QPrinter::DevicePixel).width() / width;
    qreal yScale = printer->pageRect(QPrinter::DevicePixel).height() / height;
    qreal scale = qMin(xScale, yScale);

    qreal remainingHeight = printer->pageRect(QPrinter::DevicePixel).height()/scale - height;
    qreal spaceHeight = (remainingHeight / 4.0) / (items.count() + 1);
    qreal interLineHeight = (remainingHeight / 4.0) / (sampleSizes.count() * items.count());

    painter->save();
    painter->translate(printer->pageRect(QPrinter::DevicePixel).width() / 2.0, printer->pageRect(QPrinter::DevicePixel).height() / 2.0);
    painter->scale(scale, scale);
    painter->setBrush(QBrush(Qt::black));

    qreal x = -width / 2.0;
    qreal y = -height / 2.0 - remainingHeight / 4.0 + spaceHeight;

    for (const QTreeWidgetItem *item : items) {
        QString style = item->text(0);
        int weight = item->data(0, Qt::UserRole).toInt();
        bool italic = item->data(0, Qt::UserRole + 1).toBool();

        // Draw each line of text.
        for (int size : qAsConst(sampleSizes)) {
            QFont font(family, size, weight, italic);
            font.setStyleName(style);
            font = QFont(font, painter->device());
            QFontMetricsF fontMetrics(font);
            QRectF rect = fontMetrics.boundingRect(QString("%1 %2").arg(
                          font.family()).arg(style));
            y += rect.height();
            painter->setFont(font);
            painter->drawText(QPointF(x, y), QString("%1 %2").arg(family).arg(style));
            y += interLineHeight;
        }
        y += spaceHeight;
    }

    painter->restore();
#endif
}
void  MainWindow::clickedSlot()
  {


    QList<string> itemList;
    for( int i = 0; i < fontTree->topLevelItemCount(); ++i )
        {
           QTreeWidgetItem *items = fontTree->topLevelItem(i);
           for( int j = 0; j < items->childCount(); ++j )
               {
               QTreeWidgetItem *item = items->child(j);
                QString str =item->text(0);
                if(item->checkState(0)==Qt::Checked)
                {
                    itemList.append(str.toStdString());
                }

          }
    }

int nbJobs, nbMachines, A[500][20];

string fichier=comboBox->currentText().toStdString();
       //load nbJobs, nbMachines and the matrix A
       string filepath = "../fontsampler/benchmarks/"+fichier;
       loader(filepath, &nbJobs, &nbMachines, A);


foreach(auto item,itemList)
{
    switch (hashit(item)) {
    case eBB:
    {
              auto debut= std::chrono::high_resolution_clock::now();
              //Method Here
              //int cmax=BB(nbJobs,nbMachines,A);
              int cmax=BB(nbJobs, nbMachines,A);
              auto fin= std::chrono::high_resolution_clock::now();
              auto temps=fin-debut;
              std::stringstream stream;
              stream << std::fixed << std::setprecision(3) << float((temps.count())/1000000000);
              std::string s = stream.str();
              PhoneBookModel->Add_row("BB",QString::fromStdString(fichier),QString::number(cmax),QString::fromStdString(s+" s"));
              break;
    }
    case epBB:
    {
              auto debut= std::chrono::high_resolution_clock::now();
              //Method Here
              //int cmax=BB(nbJobs,nbMachines,A);
              int cmax=Parallel_BB(nbJobs, nbMachines,A);
              auto fin= std::chrono::high_resolution_clock::now();
              auto temps=fin-debut;
              std::stringstream stream;
              stream << std::fixed << std::setprecision(3) << float((temps.count())/1000000000);
              std::string s = stream.str();
              PhoneBookModel->Add_row("Parrall BB",QString::fromStdString(fichier),QString::number(cmax),QString::fromStdString(s+" s"));
              break;
    }
    case eCDS:
    {

             auto debut= std::chrono::high_resolution_clock::now();
            //Method Here
            //CDS
             int cmax=745;
             vector<int> solution;
             CDS(A,nbJobs, nbMachines,cmax,solution);

            auto fin= std::chrono::high_resolution_clock::now();
            auto temps=fin-debut;
            std::stringstream stream;
            stream << std::fixed << std::setprecision(4) << float((temps.count())/1000000000);
            std::string s = stream.str();
            PhoneBookModel->Add_row("CDS",QString::fromStdString(fichier),QString::number(cmax),QString::fromStdString(s+" s"));

        break;
    }
       case eCHEN:
     {
        auto debut= std::chrono::high_resolution_clock::now();
       //Method Here
        int cmax=0;
        vector<int> solution;
       Chen(A,nbJobs, nbMachines,cmax,solution);

       auto fin= std::chrono::high_resolution_clock::now();
       auto temps=fin-debut;
       std::stringstream stream;
       stream << std::fixed << std::setprecision(4) << float((temps.count())/1000000000);
       std::string s = stream.str();
       PhoneBookModel->Add_row("chen",QString::fromStdString(fichier),QString::number(cmax),QString::fromStdString(s+" s"));
    break;
    }
    case eNEH:
    {
        auto debut= std::chrono::high_resolution_clock::now();
       //Method Here

       int cmax=0;
       vector<int> solution;
       NEH(A,nbJobs, nbMachines,cmax,solution);
       auto fin= std::chrono::high_resolution_clock::now();
       auto temps=fin-debut;
       std::stringstream stream;
       stream << std::fixed << std::setprecision(4) << float((temps.count())/1000000000);
       std::string s = stream.str();
       PhoneBookModel->Add_row("NEH",QString::fromStdString(fichier),QString::number(cmax),QString::fromStdString(s+" s"));
    break;
    }
    case eRS:
    {
        float T=100;
        float alpha=.8;
        int nb_it=20;
        int nb_palier=1000;
        if(!checkBox->isChecked())
        {
        paramater_rs(T,alpha,nb_it,nb_palier);
        }

        auto debut= std::chrono::high_resolution_clock::now();
       //Method Here
        int cmax=0;
        vector<int> solution,s0;
        NEH(A, nbJobs, nbMachines, cmax, s0);


        RS(A, nbJobs, nbMachines, s0, alpha, T, nb_it, nb_palier,solution, cmax);


       auto fin= std::chrono::high_resolution_clock::now();
       auto temps=fin-debut;
       std::stringstream stream;
       stream << std::fixed << std::setprecision(4) << float((temps.count())/1000000000);
       std::string s = stream.str();
       PhoneBookModel->Add_row("RS",QString::fromStdString(fichier),QString::number(cmax),QString::fromStdString(s+" s"));
    break;
    }
    case eRT:
    {
        int LT_MAX_SIZE=8;
        int stop=5;
        if(!checkBox->isChecked())
        {
         paramater_rt(LT_MAX_SIZE, stop);
        }


        auto debut= std::chrono::high_resolution_clock::now();

       int cmax=0;
       vector<int> sol;
        RT(A,nbJobs,nbMachines,'N',LT_MAX_SIZE,stop,cmax,sol);
       auto fin= std::chrono::high_resolution_clock::now();
       auto temps=fin-debut;
       std::stringstream stream;
       stream << std::fixed << std::setprecision(4) << float((temps.count())/1000000000);
       std::string s = stream.str();
       PhoneBookModel->Add_row("RT",QString::fromStdString(fichier),QString::number(cmax),QString::fromStdString(s+" s"));
    break;
    }
    case eG:
    {
         int taille_pop=500;
         char heur='1';
         double prob_crois=.8;
         double prob_mut=.1;
         char tech_mut='I';
         char tech_crois='2';
         int nb_gen=200;
         if(!checkBox->isChecked())
         {
         paramater_eg(taille_pop,prob_crois,prob_mut,nb_gen);
         }
        auto debut= std::chrono::high_resolution_clock::now();
       //Method Here

       int cmax=745;
       vector<int> solution;
       Algo_Gen_AG(A, nbJobs, nbMachines, taille_pop, heur, prob_crois, prob_mut, tech_mut, tech_crois, nb_gen, solution, cmax);
       auto fin= std::chrono::high_resolution_clock::now();
       auto temps=fin-debut;
       std::stringstream stream;
       cmax=Cmax(solution,A,nbMachines);
       stream << std::fixed << std::setprecision(4) << float((temps.count())/1000000000);
       std::string s = stream.str();
       PhoneBookModel->Add_row("Genetique",QString::fromStdString(fichier),QString::number(cmax),QString::fromStdString(s+" s"));
    break;
    }


        }
}


  }
void  MainWindow::clickedClear()
{
    PhoneBookModel->Clear();

}
void  MainWindow::paramater_rs(float &T,float &alpha,int &nb_it,int &nb_palier)
{
    // T: temperature et alpha
    // nb_it_pl: nombre d'itération par palier
    // nb_arret: nombre de palier totale, considéré comme critere d'arret
    T = QInputDialog::getDouble(this,"Temperature initiale","Temperature initiale",100.0);
    alpha = QInputDialog::getDouble(this,"alpha","alpha",.8);
    nb_it = QInputDialog::getInt(this,"nb_it_pl","nombre d'itération par palier",20);
    nb_palier = QInputDialog::getInt(this,"nb_arret"," nombre de palier totale, considéré comme critere d'arret",1000);


}
void  MainWindow::paramater_rt(int &LT_MAX_SIZE, int &stop)
{


    LT_MAX_SIZE= QInputDialog::getInt(this,"LT_MAX_SIZE","la taille max de memoire",8);
   stop = QInputDialog::getInt(this,"stop"," Pourcentage to stop the same score",5);

}
void  MainWindow::paramater_eg(int &taille_pop,double &prob_crois,double &prob_mut,int &nb_gen)
{
    taille_pop = QInputDialog::getInt(this,"taille_pop","La taille de la population",500);
    prob_crois = QInputDialog::getDouble(this,"prob_crois","probabilite de croisement",.8);
    prob_mut = QInputDialog::getDouble(this,"prob_mut","probabilite de mutation",.1);
    nb_gen= QInputDialog::getInt(this,"nb_gen","nombre de gen",200);




}
