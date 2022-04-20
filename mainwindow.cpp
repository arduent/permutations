#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->resultsTable->setColumnCount(4);
    QStringList columns;
    columns << "Campaign" << "Ad Group" << "Keyword" << "Match Type";
    ui->resultsTable->setHorizontalHeaderLabels(columns);
    ui->resultsTable->horizontalHeader()->show();
//    ui->resultsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->resultsTable->horizontalHeader()->setStretchLastSection(true);
    ui->resultsTable->verticalHeader()->hide();
    ui->resultsTable->setShowGrid(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QStringList MainWindow::getLines(QString data)
{
    return (data.split(QRegExp("[\n]"),Qt::SkipEmptyParts));
}

QString MainWindow::addRow(QString campaign, QString adGroup, QString keyword, QString matchType)
{
    int row = ui->resultsTable->rowCount();
    ui->resultsTable->setRowCount(row+1);

    QTableWidgetItem* campaignItem = new QTableWidgetItem();
    QTableWidgetItem* agItem = new QTableWidgetItem();
    QTableWidgetItem* keyItem = new QTableWidgetItem();
    QTableWidgetItem* matchItem = new QTableWidgetItem();

    campaignItem->setText(campaign);
    agItem->setText(adGroup);
    keyItem->setText(keyword);
    matchItem->setText(matchType);

    ui->resultsTable->setItem(row, 0, campaignItem);
    ui->resultsTable->setItem(row, 1, agItem);
    ui->resultsTable->setItem(row, 2, keyItem);
    ui->resultsTable->setItem(row, 3, matchItem);
    QStringList fields;
    fields << "\"" + campaign + "\"" << "\"" + adGroup + "\"" << "\"" + keyword + "\"" << "\"" + matchType + "\"";
    return fields.join(",");
}

void MainWindow::on_runButton_clicked()
{
    std::list<QString> sections = { "C","S","N" };
    csvLines.clear();

    QStringList cities = getLines(ui->citiesList->toPlainText());
    if (cities.count()<1)
    {
        QMessageBox msgBox;
        msgBox.setText("Cities List is Empty!");
        msgBox.exec();
    } else {
        QStringList adgroups = getLines(ui->adGroupList->toPlainText());
        if (adgroups.count()<1)
        {
            QMessageBox msgBox;
            msgBox.setText("Ad Group is Empty!");
            msgBox.exec();
        } else {

            /* permutate */

            for (int i=0;i<cities.count();i++)
            {
                for (const QString & section : sections)
                {
                    csvLines.append(addRow(cities[i]+" Basic Terms ("+section+")", cities[i]+" Homes", cities[i].toLower()+" homes for sale", "phrase"));
                }

                /* Homes */
                for (const QString & section : sections)
                {
                    for (int j=0;j<adgroups.count();j++)
                    {
                        csvLines.append(addRow(cities[i]+" Subdivisions ("+section+")", adgroups[i], adgroups[i].toLower()+" for sale", "phrase"));
                    }
                }

                /* Condos */
                for (const QString & section : sections)
                {
                    for (int j=0;j<adgroups.count();j++)
                    {
                        QString condo = adgroups[i];
                        condo.replace(" Homes"," Condos");
                        csvLines.append(addRow(cities[i]+" Subdivisions ("+section+")", condo, condo.toLower()+" for sale", "phrase"));
                    }
                }

                /* Townhouses */
                for (const QString & section : sections)
                {
                    for (int j=0;j<adgroups.count();j++)
                    {
                        QString townhouses = adgroups[i];
                        townhouses.replace(" Homes"," Townhouses"); // or is it "Townhomes?"
                        csvLines.append(addRow(cities[i]+" Subdivisions ("+section+")", townhouses, townhouses.toLower()+" for sale", "phrase"));
                    }
                }

                for (const QString & section : sections)
                {
                    csvLines.append(addRow(cities[i]+" Anti-Branding ("+section+")", cities[i]+" Zillow", "homes for sale "+cities[i].toLower()+" zillow", "phrase"));
                    csvLines.append(addRow(cities[i]+" Anti-Branding ("+section+")", cities[i]+" Realtor", "homes for sale "+cities[i].toLower()+" realtor", "phrase"));
                    csvLines.append(addRow(cities[i]+" Anti-Branding ("+section+")", cities[i]+" Redfin", "homes for sale "+cities[i].toLower()+" redfin", "phrase"));
                }
            }

        }
    }
}


void MainWindow::on_exportButton_clicked()
{
    if (csvLines.count()<1)
    {
        QMessageBox msgBox;
        msgBox.setText("Must Run Report First!");
        msgBox.exec();
    } else {
        QString fileName = QFileDialog::getSaveFileName(this, "Save CSV File",
                                   "output.csv",
                                   tr("CSV Files (*.csv *.txt *.*)"));
        if( !fileName.isNull() )
        {
            try {
                QFile file(fileName);
                file.open(QIODevice::WriteOnly);
                QTextStream qout(&file);
                for (int i = 0; i<csvLines.count(); i++)
                {
                    qout << csvLines[i] << "\r\n";
                }
                file.close();
            } catch (...)
            {
                QMessageBox msgBox;
                msgBox.setText("Could not save file!");
                msgBox.exec();
            }
        }
    }
}

