
#ifndef WINDOWS_MAIN_WINDOW_H_
#define WINDOWS_MAIN_WINDOW_H_

#include <QWidget>

class QNetworkReply;
class QTableView;
class TasksModel;

class MainWindow : public QWidget {
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = 0);
  virtual ~MainWindow();

  void updateIssues();

private slots:
  void onNetworkReply(QNetworkReply* reply);

private:
  QTableView* m_issuesList;

  TasksModel* m_tasksModel;
};

#endif  // WINDOWS_MAIN_WINDOW_H_
