
#include "windows/main_window.h"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QDomElement>
#include <QTableView>
#include <QBoxLayout>
#include <QPushButton>

#include "models/issues_model.h"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
  // Create controls.
  m_issuesList = new QTableView(this);
  QPushButton* updateButton = new QPushButton("Update");
  connect(updateButton, SIGNAL(clicked()), this, SLOT(onUpdateButtonClicked()));

  // Set up the main layout.

  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(m_issuesList);
  mainLayout->addWidget(updateButton);

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

void MainWindow::onUpdateButtonClicked() { updateIssues(); }

void MainWindow::onNetworkReply(QNetworkReply* reply) {
  QByteArray data = reply->read(reply->bytesAvailable());

  QDomDocument document;
  document.setContent(data);

  IssuesModel::ListType issues;

  QDomElement root = document.firstChildElement("issues");
  for (QDomElement issueElem = root.firstChildElement("issue");
       !issueElem.isNull(); issueElem = issueElem.nextSiblingElement("issue")) {
    Issue issue;
    QDomElement subject = issueElem.firstChildElement("subject");
    if (!subject.isNull()) {
      issue.setSubject(subject.text());
    }

    QDomElement assignedTo = issueElem.firstChildElement("assigned_to");
    if (!assignedTo.isNull()) {
      QDomAttr nameAttr = assignedTo.attributeNode("name");
      if (!nameAttr.isNull()) {
        issue.setAssignedTo(nameAttr.value());
      }
    }

    issues.append(issue);
  }

  delete m_issuesList->model();
  m_issuesList->setModel(new IssuesModel(issues, this));
}
