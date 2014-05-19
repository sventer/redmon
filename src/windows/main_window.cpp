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
#include <QUrl>

#include "data/data.h"
#include "data/data_loader.h"
#include "models/issues_model.h"
#include "models/issue_list_item_delegate.h"
#include "windows/settings_window.h"

const size_t kIssuesPerPage = 50;

MainWindow::MainWindow(QWidget* parent) : QWidget(parent), m_lastPage(0) {
  // Position the window to the last place we stored it at.
  QSettings settings;
  QPoint mainWindowPos(settings.value("mainWindowPos").toPoint());
  QSize mainWindowSize(settings.value("mainWindowSize").toSize());
  setGeometry(QRect(mainWindowPos, mainWindowSize));

  // Create the NAM that we will use to retrieve the issues.
  m_issuesManager = new QNetworkAccessManager(this);
  connect(m_issuesManager, SIGNAL(finished(QNetworkReply*)),
          SLOT(onNetworkReply(QNetworkReply*)));

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
}

MainWindow::~MainWindow() {}

void MainWindow::onSettingsButtonClicked() {
  SettingsWindow settingsWindow(this);
  settingsWindow.setModal(true);
  settingsWindow.exec();
}

void MainWindow::onUpdateButtonClicked() {
  // Set the update button to a disabled state.
  m_updateButton->setText("Updating...");
  m_updateButton->setEnabled(false);

  m_tempIssues.clear();

  // Start the process to update the issues.
  updateIssues();
}

void MainWindow::onNetworkReply(QNetworkReply* reply) {
  qDebug() << "onNetworkReply(...)";

  QByteArray data(reply->read(reply->bytesAvailable()));

  // Create the XML document.
  QDomDocument document;
  document.setContent(data);

  // Get the root element.
  QDomElement root(document.firstChildElement("issues"));

  // Parse the XML into a temp vector.
  size_t issuesBefore = m_tempIssues.size();
  parseIssues(root, &m_tempIssues);

  bool doNextPage = (m_tempIssues.size() - issuesBefore) == kIssuesPerPage;

  if (doNextPage) {
    updateIssues(m_lastPage + 1);
  } else {
    m_lastPage = 0;

    Data::Get().issues.swap(m_tempIssues);

    // Reset the issues list to update it.
    m_issuesList->reset();

    // Set the update button back to an enabled state.
    m_updateButton->setText("Update");
    m_updateButton->setEnabled(true);
  }
}

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

void MainWindow::updateIssues(int page, int assignedToId) {
  qDebug() << "updateIssues(" << page << ")";

  m_lastPage = page;

  QSettings settings;
  QString url(
      "http://%1/issues.xml?"
      "key=%2&limit=%3&page=%4&sort=priority:desc,id:desc");
  url = url.arg(settings.value("serverUrl").toString())
            .arg(settings.value("apiKey").toString())
            .arg(kIssuesPerPage)
            .arg(page);

  if (assignedToId == 1) {
    url.append("&assigned_to_id=me");
  } else if (assignedToId > 0) {
    url.append("&assigned_to_id=").append(assignedToId);
  }

  qDebug() << "Requesting URL:" << url;

  m_issuesManager->get(QNetworkRequest(QUrl(url)));
}
