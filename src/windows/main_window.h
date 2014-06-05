// RedMon
// Copyright (c) 2014 Tiaan Louw
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef WINDOWS_MAIN_WINDOW_H_
#define WINDOWS_MAIN_WINDOW_H_

#include <QWidget>

#include "data/issue.h"

class QListView;
class QListWidgetItem;
class QModeEvent;
class QNetworkAccessManager;
class QNetworkReply;
class QPushButton;
class QResizeEvent;
class QTimer;
class QTableView;

class DataLoader;
class IssuesTableModel;

class MainWindow : public QWidget {
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = 0);
  virtual ~MainWindow();

  virtual void showEvent(QShowEvent* event) override;

signals:
  void onDataLoaded();

protected:
  void moveEvent(QMoveEvent* event);
  void resizeEvent(QResizeEvent* event);

private slots:
  void onSettingsButtonClicked();
  void onUpdateButtonClicked();
  void onStartButtonClicked();
  void onStopButtonClicked();

  void onDataLoaderFinished();
  void onIssueListTimerTimeout();
  void onIssuesListCurrentChanged(const QModelIndex& current,
                                  const QModelIndex& previous);
  void onSelectIssue(const QModelIndex& slection);

private:
  void startTimer();
  void stopTimer();

  void startTrackingTime(int issueId);
  void stopTrackingTime();

  // Controls.
  QPushButton* m_settingsButton;
  QPushButton* m_updateButton;
  QPushButton* m_startButton;
  QPushButton* m_stopButton;

  QListView* m_issuesList;
  QTableView* m_issuesTable;
  IssuesTableModel* m_tableIssuesModel;

  // The worker object we use to load our issues from the server.
  DataLoader* m_dataLoader;

  // The timer we use to update the issue list.
  QTimer* m_issueListTimer;

  // Set to true if we are currently tracking time.
  bool m_isTrackingTime;

  // have we been shown before?
  bool m_isInitializeDone;

  Q_DISABLE_COPY(MainWindow)
};

#endif  // WINDOWS_MAIN_WINDOW_H_
