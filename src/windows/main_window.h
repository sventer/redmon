#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QNetworkReply;
class TasksModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_updateButton_clicked();
    void onNetworkReply(QNetworkReply* reply);

private:
    Ui::MainWindow *ui;

    TasksModel* m_tasksModel;
};

#endif // MAINWINDOW_H
