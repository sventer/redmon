#include "windows/main_window.h"
#include "ui_main_window.h"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QDomElement>

#include <QStandardItemModel>

#include "models/tasks_model.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_updateButton_clicked()
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(onNetworkReply(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("http://redmine.playsafesa.com/issues.xml?key=aad507500298994cd06c224d649f1453fbb44c78")));
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

        issues.append(issue);
    }

    delete ui->taskList->model();
    ui->taskList->setModel(new TasksModel(issues, this));
}
