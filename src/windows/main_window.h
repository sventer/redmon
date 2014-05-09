
#ifndef WINDOWS_MAIN_WINDOW_H_
#define WINDOWS_MAIN_WINDOW_H_

#include <QMainWindow>

class QNetworkReply;
class QTableView;
class TasksModel;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  virtual ~MainWindow();

private slots:
  void on_updateButton_clicked();
  void onNetworkReply(QNetworkReply* reply);

private:
  QTableView* m_taskList;

  TasksModel* m_tasksModel;
};

#endif  // WINDOWS_MAIN_WINDOW_H_
