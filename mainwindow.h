#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_runButton_clicked();

    void on_exportButton_clicked();

private:
    Ui::MainWindow *ui;
    QStringList csvLines;
    QStringList getLines(QString);
    QString addRow(QString campaign, QString adGroup, QString keyword, QString matchType);
};
#endif // MAINWINDOW_H
