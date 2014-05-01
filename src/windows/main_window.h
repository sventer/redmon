
#ifndef WINDOWS_MAIN_WINDOW_H_
#define WINDOWS_MAIN_WINDOW_H_

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QNetworkReply;
class TasksModel;

class MainWindow : public QMainWindow {
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

#endif  // WINDOWS_MAIN_WINDOW_H_
