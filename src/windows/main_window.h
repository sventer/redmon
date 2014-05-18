
#ifndef WINDOWS_MAIN_WINDOW_H_
#define WINDOWS_MAIN_WINDOW_H_

#include <QWidget>

class QListView;
class QNetworkReply;
class QPushButton;

class MainWindow : public QWidget {
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = 0);
  virtual ~MainWindow();

  void updateIssues();

private slots:
  void onUpdateButtonClicked();
  void onNetworkReply(QNetworkReply* reply);

private:
  QPushButton* m_updateButton;
  QListView* m_issuesList;
};

#endif  // WINDOWS_MAIN_WINDOW_H_
