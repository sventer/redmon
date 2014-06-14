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
#include "models/issues_table_model.h"
#include "models/issue_table_item_delegate.h"
#include "models/time_activities_model.h"
#include "views/issues_table_view.h"
#include "views/active_issue_widget.h"
#include "windows/settings_window.h"

MainWindow::MainWindow(QWidget* parent)
  : QWidget(parent) {
  TimeEntryActivitiesModel someModel;

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

  // Create the active issue widget.

  m_activeIssueWidget = new ActiveIssueWidget;
  m_activeIssueWidget->hide();
  connect(m_activeIssueWidget, SIGNAL(trackingCompleted()), this,
          SLOT(onActiveIssueWidgetTrackingCompleted()));

  // Create the progress bar.

  m_progressBar = new QProgressBar;
  m_progressBar->setMinimum(0);
  m_progressBar->setMaximum(100);
  m_progressBar->setValue(50);
  m_progressBar->setTextVisible(false);
  m_progressBar->setVisible(false);

  // Create the issues model that we use in the issues table.

  m_issuesTableModel = new IssuesTableModel(this);

  // Create the issues table.

  m_issuesTable = new IssuesTableView;
  m_issuesTable->setFrameShape(QFrame::NoFrame);
  m_issuesTable->setItemDelegate(new IssueTableItemDelegate);
  m_issuesTable->setModel(m_issuesTableModel);
  connect(m_issuesTable, SIGNAL(clicked(QModelIndex)), this,
          SLOT(onSelectIssue(QModelIndex)));

  // Set up the layouts.
  QHBoxLayout* buttonsLayout = new QHBoxLayout;
  buttonsLayout->setMargin(5);
  buttonsLayout->setSpacing(5);
  buttonsLayout->addWidget(m_settingsButton);
  buttonsLayout->addWidget(m_updateButton);
  buttonsLayout->addWidget(m_startButton);

  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->setMargin(0);
  mainLayout->setSpacing(0);
  mainLayout->addLayout(buttonsLayout);
  mainLayout->addWidget(m_activeIssueWidget);
  mainLayout->addWidget(m_issuesTable);
  mainLayout->addWidget(m_progressBar);

  setLayout(mainLayout);

  m_issueListTimer = new QTimer(this);
  connect(m_issueListTimer, SIGNAL(timeout()), this,
          SLOT(onUpdateButtonClicked()));

  // Do an update.
  onUpdateButtonClicked();
}

MainWindow::~MainWindow() {}

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
  // Disable the start button.
  m_startButton->setEnabled(false);

  // Get the selected issue.
  QItemSelectionModel* selection = m_issuesTable->selectionModel();
  const Issue& issue =
      m_issuesTableModel->issue(selection->selectedRows().first());

  // Start tracking the issue.
  m_activeIssueWidget->startTrackingIssue(issue);
  m_activeIssueWidget->show();
}

void MainWindow::onDataLoaderProgress(int current, int max) {
  m_progressBar->setValue(current);
  m_progressBar->setMaximum(max);
}

void MainWindow::onDataLoaderFinished() {
  m_issuesTableModel->insertIssues(m_dataLoader->issues());

  // Set the update button back to an enabled state.
  m_updateButton->setText("Update");
  m_updateButton->setEnabled(true);

  // Hide the progress bar again.
  m_progressBar->setVisible(false);

  // Start the update timer again.
  startTimer();
}

void MainWindow::onActiveIssueWidgetTrackingCompleted() {
  // Enable the start button again.
  m_startButton->setEnabled(true);

  // Hide the active issue widget.
  m_activeIssueWidget->hide();
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
