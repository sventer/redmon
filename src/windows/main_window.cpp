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
#include <QDebug>
#include <QListView>
#include <QMoveEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QResizeEvent>
#include <QSettings>
#include <QTimer>
#include <QUrl>

#include "data/data.h"
#include "data/data_loader.h"
#include "models/issues_model.h"
#include "models/issue_list_item_delegate.h"
#include "windows/settings_window.h"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent), m_dataLoader(NULL) {
  // Position the window to the last place we stored it at.
  QSettings settings;
  QPoint mainWindowPos(settings.value("mainWindowPos").toPoint());
  QSize mainWindowSize(settings.value("mainWindowSize").toSize());
  if (mainWindowPos != QPoint(0, 0) && mainWindowSize != QSize(0, 0))
    setGeometry(QRect(mainWindowPos, mainWindowSize));

  // Create and link up the worker object that will load our issues for us.
  m_dataLoader = new DataLoader(this);
  connect(m_dataLoader, SIGNAL(finished()), this, SLOT(onDataLoaderFinished()));

  Issue issue;

  issue.id = 1;
  issue.subject = "This is a long subject";
  issue.projectName = "Project 1";
  Data::Get().issues.append(issue);

  issue.id = 2;
  issue.subject = "This is another long subject for testing purposes";
  issue.projectName = "Project 1";
  Data::Get().issues.append(issue);

  issue.id = 3;
  issue.subject = "This is another long subject";
  issue.projectName = "Another Project";
  Data::Get().issues.append(issue);

  // Create controls.

  m_issuesList = new QListView(this);
  m_issuesList->setFrameShape(QFrame::NoFrame);
  m_issuesList->setResizeMode(QListView::Adjust);
  m_issuesList->setItemDelegate(new IssueListItemDelegate);
  m_issuesList->setModel(new IssuesModel);

  m_settingsButton = new QPushButton("Settings");
  connect(m_settingsButton, SIGNAL(clicked()), this,
          SLOT(onSettingsButtonClicked()));

  m_updateButton = new QPushButton("Update");
  connect(m_updateButton, SIGNAL(clicked()), this,
          SLOT(onUpdateButtonClicked()));

  // Set up the layouts.

  QHBoxLayout* buttonsLayout = new QHBoxLayout;
  buttonsLayout->setMargin(5);
  buttonsLayout->setSpacing(5);
  buttonsLayout->addWidget(m_settingsButton);
  buttonsLayout->addWidget(m_updateButton);

  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->setMargin(0);
  mainLayout->setSpacing(0);
  mainLayout->addLayout(buttonsLayout);
  mainLayout->addWidget(m_issuesList);

  setLayout(mainLayout);

  m_issueListTimer = new QTimer(this);
  connect(m_issueListTimer, SIGNAL(timeout()), this,
          SLOT(onIssueListTimerTimeout()));
  startTimer();
}

MainWindow::~MainWindow() {}

void MainWindow::moveEvent(QMoveEvent* event) {
  QWidget::moveEvent(event);

  QSettings settings;
  settings.setValue("mainWindowPos", event->pos());
}

void MainWindow::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);

  QSettings settings;
  settings.setValue("mainWindowSize", event->size());
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

  // Start loading the issues.
  m_dataLoader->loadData();
}

void MainWindow::onDataLoaderFinished() {
  qDebug() << "MainWindow::onIssuesLoaded()";

  // Swap the data from the worker into the real issues data.
  m_dataLoader->swapIssues(&Data::Get().issues);

  // Reset the issues list to update it.
  m_issuesList->reset();

  // Set the update button back to an enabled state.
  m_updateButton->setText("Update");
  m_updateButton->setEnabled(true);

  // Start the update timer again.
  startTimer();
}

void MainWindow::onIssueListTimerTimeout() {
  onUpdateButtonClicked();
}

void MainWindow::startTimer() {
  QSettings settings;

  int interval = settings.value("issueListUpdateInterval").toInt();

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
