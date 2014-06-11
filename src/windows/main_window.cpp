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

#include "windows/main_window.h"

#include <QBoxLayout>
#include <QDateTime>
#include <QDebug>
#include <QListView>
#include <QMoveEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QResizeEvent>
#include <QSettings>
#include <QProgressBar>

#include <QTimer>
#include <QUrl>

#include <QHeaderView>

#include "config.h"
#include "data/data.h"
#include "data/data_loader.h"
#include "windows/settings_window.h"
#include "models/issues_table_model.h"
#include "models/issue_table_item_delegate.h"
#include "views/issues_table_view.h"
#include "dialogs/issue_activity_dialog.h"

MainWindow::MainWindow(QWidget* parent)
  : QWidget(parent), m_isTrackingTime(false), m_isInitializeDone(false) {
  m_issueActivityDialog = new IssueActivityDialog();
  m_issueActivityDialog->hide();

  // Position the window to the last place we stored it at.
  QPoint mainWindowPos(Config::Get().mainWindowPos());
  QSize mainWindowSize(Config::Get().mainWindowSize());
  if (mainWindowPos != QPoint(0, 0) && mainWindowSize != QSize(0, 0))
    setGeometry(QRect(mainWindowPos, mainWindowSize));

  // Create and link up the worker object that will load our issues for us.
  m_dataLoader = new DataLoader(this);
  connect(m_dataLoader, SIGNAL(progress(int, int)), this,
          SLOT(onDataLoaderProgress(int, int)));
  connect(m_dataLoader, SIGNAL(finished()), this, SLOT(onDataLoaderFinished()));

  // Create the main buttons.

  m_settingsButton = new QPushButton("Settings");
  connect(m_settingsButton, SIGNAL(clicked()), this,
          SLOT(onSettingsButtonClicked()));

  m_updateButton = new QPushButton("Update");
  connect(m_updateButton, SIGNAL(clicked()), this,
          SLOT(onUpdateButtonClicked()));

  m_startButton = new QPushButton("Start");
  connect(m_startButton, SIGNAL(clicked()), this, SLOT(onStartButtonClicked()));

  m_stopButton = new QPushButton("Stop");
  connect(m_stopButton, SIGNAL(clicked()), this, SLOT(onStopButtonClicked()));
  m_stopButton->hide();

  // Create the progress bar.

  m_progressBar = new QProgressBar;
  m_progressBar->setMinimum(0);
  m_progressBar->setMaximum(100);
  m_progressBar->setValue(50);
  m_progressBar->setTextVisible(false);
  m_progressBar->setVisible(false);

  // Create the issues table.

  m_issuesTable = new IssuesTableView;
  m_tableIssuesModel = new IssuesTableModel;
  m_issuesTable->setFrameShape(QFrame::NoFrame);
  m_issuesTable->setItemDelegate(new IssueTableItemDelegate);
  m_issuesTable->horizontalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);
  m_issuesTable->verticalHeader()->hide();
  m_issuesTable->setModel(m_tableIssuesModel);
  connect(m_issuesTable, SIGNAL(clicked(QModelIndex)), this,
          SLOT(onSelectIssue(QModelIndex)));
  connect(this, SIGNAL(onDataLoaded()), m_issuesTable,
          SLOT(setInitialSelection()));

  // Set up the layouts.
  QHBoxLayout* buttonsLayout = new QHBoxLayout;
  buttonsLayout->setMargin(5);
  buttonsLayout->setSpacing(5);
  buttonsLayout->addWidget(m_settingsButton);
  buttonsLayout->addWidget(m_updateButton);
  buttonsLayout->addWidget(m_startButton);
  buttonsLayout->addWidget(m_stopButton);

  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->setMargin(0);
  mainLayout->setSpacing(0);
  mainLayout->addLayout(buttonsLayout);
  mainLayout->addWidget(m_issuesTable);
  mainLayout->addWidget(m_progressBar);

  setLayout(mainLayout);

  m_issueListTimer = new QTimer(this);
  connect(m_issueListTimer, SIGNAL(timeout()), this,
          SLOT(onIssueListTimerTimeout()));
  startTimer();
}

MainWindow::~MainWindow() {
}

void MainWindow::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);

  if (event->spontaneous())
    return;

  // we return if we have performed the initial update request
  if (m_isInitializeDone)
    return;

  // request an initial issue update
  onUpdateButtonClicked();

  m_isInitializeDone = true;
}

void MainWindow::moveEvent(QMoveEvent* event) {
  QWidget::moveEvent(event);

  Config::Get().setMainWindowPos(event->pos());
}

void MainWindow::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);

  Config::Get().setMainWindowSize(event->size());
}

void MainWindow::onSettingsButtonClicked() {
  // Stop the timer while we're in the settings window.
  stopTimer();

  SettingsWindow settingsWindow(this);
  settingsWindow.setModal(true);
  settingsWindow.exec();

  // Do a refresh.  We don't have to start the timer ourselves here, because
  // after the update the timer will start up again with the new interval value.
  onUpdateButtonClicked();
}

void MainWindow::onUpdateButtonClicked() {
  // If the update timer is running, stop it until the update is done.
  stopTimer();

  // Set the update button to a disabled state.
  m_updateButton->setText("Updating...");
  m_updateButton->setEnabled(false);

  // Show the progress bar.  We also assume that only one thing is going to be
  // done.
  m_progressBar->setVisible(true);
  m_progressBar->setMinimum(0);
  m_progressBar->setMaximum(1);
  m_progressBar->setValue(0);

  // Start loading the issues.
  m_dataLoader->loadData();
}

void MainWindow::onStartButtonClicked() {
  // If we are already tracking time, don't do anything.
  if (m_isTrackingTime)
    return;

  // If there is nothing selected in the issue list, don't do anything.
  if (m_issuesTable->selectedRow() < 0)
    return;

  m_issuesTable->setEnabled(false);

  // Swap the buttons out.
  m_startButton->hide();
  m_stopButton->show();

  // Get the issue.
  Issue issue;
  m_tableIssuesModel->getIssue(m_issuesTable->selectedRow(), &issue);

  // Start.
  startTrackingTime(issue.id);
}

void MainWindow::onStopButtonClicked() {
  // If we are not tracking time, don't do anything.
  if (!m_isTrackingTime)
    return;

  Issue issue;
  m_tableIssuesModel->getIssue(m_issuesTable->selectedRow(), &issue);

  stopTrackingTime();

  // m_tableIssuesModel->getIssue(0, &issue);
  m_issueActivityDialog->updateDetails(issue);

  float time = m_stopTime.toTime_t() - m_startTime.toTime_t();
  m_issueActivityDialog->updateTimeSpent(time);

  m_issueActivityDialog->show();

  // Swap the buttons out.
  m_startButton->show();
  m_stopButton->hide();

  m_issuesTable->setEnabled(true);
}

void MainWindow::onDataLoaderProgress(int current, int max) {
  m_progressBar->setValue(current);
  m_progressBar->setMaximum(max);
}

void MainWindow::onDataLoaderFinished() {
  qDebug() << "MainWindow::onIssuesLoaded()";

  // Swap the data from the worker into the real issues data.
  m_dataLoader->swapIssues(&Data::Get().issues);

  int totalIssues = Data::Get().issues.count();
  for (int idx = 0; idx < totalIssues; ++idx) {
    if (m_tableIssuesModel->insertIssue(Data::Get().issues.at(idx)))
      int i = 10;
  }

  if (totalIssues > 0) {
    emit onDataLoaded();
  }

  // Set the update button back to an enabled state.
  m_updateButton->setText("Update");
  m_updateButton->setEnabled(true);

  // Hide the progress bar again.
  m_progressBar->setVisible(false);

  // Start the update timer again.
  startTimer();
}

void MainWindow::onIssueListTimerTimeout() { onUpdateButtonClicked(); }

void MainWindow::onIssuesListCurrentChanged(const QModelIndex& current,
                                            const QModelIndex& previous) {
  qDebug() << current;
}

void MainWindow::onSelectIssue(const QModelIndex& selection) {
  qDebug() << selection.row();
  m_issuesTable->selectRow(selection.row());
}

void MainWindow::startTimer() {
  int interval = Config::Get().issueListUpdateInterval();

  if (interval >= 1) {
    qDebug() << "Starting timer:" << interval;
    // interval in minutes.
    m_issueListTimer->start(interval * 1000 * 60);
  }
}

void MainWindow::stopTimer() {
  if (m_issueListTimer->isActive()) {
    qDebug() << "Stopping timer";
    m_issueListTimer->stop();
  }
}

void MainWindow::startTrackingTime(int issueId) {
  qDebug() << "Tracking time for:" << issueId;

  m_startTime = QDateTime::currentDateTime();
  
  m_isTrackingTime = true;
}

void MainWindow::stopTrackingTime() { 
  qDebug() << "Stop tracking time"; 

  m_stopTime = QDateTime::currentDateTime();

  m_isTrackingTime = false;
}
