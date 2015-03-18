#include "LoginWidget.h"
#include "ui_LoginWidget.h"

// Qt
#include <QTimer>
#include <QMovie>

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    ui->lb_loginAjax->setMovie(new QMovie(":/icon/ajax-loader.gif"));
    ui->lb_loginAjax->hide();

    connect(ui->loginBtn, SIGNAL(clicked()), this, SLOT(clickedLogin()));
    connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(clickedCancel()));

}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::showLoginState(const QString &str)
{
    ui->statuesLb->setText("<b><span style=\" color:#ff0000;\">" + str + "</span</b>");
}

void LoginWidget::lockLogin()
{
    ui->lb_loginAjax->show();
    ui->lb_loginAjax->movie()->start();
    ui->usernameEdt->setEnabled(false);
    ui->passwordEdt->setEnabled(false);
    ui->loginBtn->setEnabled(false);
    ui->cancelBtn->setEnabled(true);
}

void LoginWidget::unlockLogin()
{
    ui->lb_loginAjax->hide();
    ui->usernameEdt->setEnabled(true);
    ui->passwordEdt->setEnabled(true);
    ui->loginBtn->setEnabled(true);
    ui->cancelBtn->setEnabled(false);
}

void LoginWidget::clickedLogin()
{
    if (ui->usernameEdt->hasAcceptableInput())
    {
        lockLogin();
        emit login();
    }
    else
    {
        showLoginState("<b>Notice !</b> Login Faile!");
    }
}

void LoginWidget::clickedCancel()
{
    emit cancelLogin();
    unlockLogin();
}

void LoginWidget::readData(Preferences *pref)
{
    ui->usernameEdt->setText(pref->username);
    ui->passwordEdt->setText(pref->password);
}

void LoginWidget::writeData(Preferences *pref)
{
    pref->username = ui->usernameEdt->text();
    pref->password = ui->passwordEdt->text();
    pref->jid = ui->usernameEdt->text() + pref->domain;
}

