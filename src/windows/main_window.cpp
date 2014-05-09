
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

#include <QStandardItemModel>

#include "models/tasks_model.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent) {
  m_taskList = new QTableView(this);

  setCentralWidget(m_taskList);

  on_updateButton_clicked();
}

MainWindow::~MainWindow() {
}

void MainWindow::on_updateButton_clicked() {
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(onNetworkReply(QNetworkReply*)));

    QString apiKey("aad507500298994cd06c224d649f1453fbb44c78");
    QString url("http://redmine.playsafesa.com/issues.xml?key=%1&limit=100");

    manager->get(QNetworkRequest(QUrl(url.arg(apiKey))));
}

void MainWindow::onNetworkReply(QNetworkReply* reply) {
    QByteArray data = reply->read(reply->bytesAvailable());

    QDomDocument document;
    document.setContent(data);

    TasksModel::ListType issues;

    QDomElement root = document.firstChildElement("issues");
    for (QDomElement issueElem = root.firstChildElement("issue"); !issueElem.isNull(); issueElem = issueElem.nextSiblingElement("issue")) {
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

        if (issue.assignedTo() == "Tiaan Louw")
        issues.append(issue);
    }

    delete m_taskList->model();
    m_taskList->setModel(new TasksModel(issues, this));
}
