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
#include <QListView>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QUrl>

#include "data/data.h"
#include "data/data_loader.h"
#include "models/issues_model.h"
#include "models/issue_list_item_delegate.h"
#include "windows/settings_window.h"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
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

void MainWindow::updateIssues() {
  QNetworkAccessManager* manager = new QNetworkAccessManager(this);
  connect(manager, SIGNAL(finished(QNetworkReply*)),
          SLOT(onNetworkReply(QNetworkReply*)));

  QString apiKey("aad507500298994cd06c224d649f1453fbb44c78");
  QString url("http://redmine.playsafesa.com/issues.xml?key=%1&limit=100");

  manager->get(QNetworkRequest(QUrl(url.arg(apiKey))));
}

void MainWindow::onSettingsButtonClicked() {
  SettingsWindow settingsWindow(this);
  settingsWindow.setModal(true);
  settingsWindow.exec();
}

void MainWindow::onUpdateButtonClicked() {
  // Set the update button to a disabled state.
  m_updateButton->setText("Updating...");
  m_updateButton->setEnabled(false);

  // Start the process to update the issues.
  updateIssues();
}

void MainWindow::onNetworkReply(QNetworkReply* reply) {
  QByteArray data = reply->read(reply->bytesAvailable());

  // Create the XML document.
  QDomDocument document;
  document.setContent(data);

  // Get the root element.
  QDomElement root = document.firstChildElement("issues");

  // Parse the XML into a temp vector.
  QVector<Issue> issues;
  parseIssues(root, &issues);
  Data::Get().issues.swap(issues);

  // Reset the issues list to update it.
  m_issuesList->reset();

  // Set the update button back to an enabled state.
  m_updateButton->setText("Update");
  m_updateButton->setEnabled(true);
}
