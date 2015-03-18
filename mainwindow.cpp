#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Preferences.h"
#include "RosterTree.h"
#include "LoginWidget.h"
#include "UserChatDialog.h"
#include "RosterWidgetItem.h"

// XMPP
#include <QXmpp/QXmppConfiguration.h>
#include <QXmpp/QXmppRosterManager.h>
#include <QXmpp/QXmppVCardManager.h>
#include <QXmpp/QXmppMessage.h>
#include <QXmpp/QXmppUtils.h>

// Qt
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_preferences(new Preferences),
    m_xmppClient(new QXmppClient(this)),
    m_loginWidget(new LoginWidget(this))
{
    ui->setupUi(this);
    initialize();

    ui->treeWidget->setClient(m_xmppClient);

    // xmpp client
    connect(m_xmppClient, SIGNAL(connected()),
            this, SLOT(clientConnected()));
    connect(m_xmppClient, SIGNAL(disconnected()),
            this, SLOT(clientDisconnected()) );
    connect(m_xmppClient, SIGNAL(error(QXmppClient::Error)),
            this, SLOT(clientError(QXmppClient::Error)));
    bool check = connect(m_xmppClient, SIGNAL(messageReceived(QXmppMessage)),
                    this, SLOT(messageReceived(QXmppMessage)));
    Q_ASSERT(check);


    // qApp
    connect( qApp, SIGNAL(lastWindowClosed()), this, SLOT(quit()) );
    // login
    connect(m_loginWidget, SIGNAL(login()), this, SLOT(login()));
    connect(m_loginWidget, SIGNAL(cancelLogin()), this, SLOT(cancelLogin()));
}

MainWindow::~MainWindow()
{
    delete m_preferences;
    delete m_xmppClient;
    delete m_loginWidget;
    delete ui;
}

void MainWindow::initialize()
{
    readPreferences();
    ui->avatarLb->setHidden(true);
    ui->nameLb->setHidden(true);
    ui->signatureLb->setHidden(true);
    ui->stateCbox->setHidden(true);
    ui->treeWidget->setHidden(true);
    ui->gridLayout->addWidget(m_loginWidget, 0, 0, 1, 1);
}

void MainWindow::readPreferences()
{
    m_preferences->load();
    m_loginWidget->readData(m_preferences);
}

void MainWindow::writePreferences()
{
    m_preferences->save();
    m_loginWidget->writeData(m_preferences);

}

void MainWindow::login()
{
    m_loginWidget->lockLogin();
    m_loginWidget->writeData(m_preferences);
    m_loginWidget->showLoginState(tr("Login ..."));
    ui->treeWidget->clear();

    QXmppConfiguration conf;
    conf.setHost(m_preferences->host);
    conf.setDomain(m_preferences->domain);
    conf.setUser(m_preferences->username);
    conf.setJid(m_preferences->jid);
    conf.setPort(m_preferences->port);
    conf.setPassword(m_preferences->password);
    conf.setResource(m_preferences->jid);
    m_xmppClient->connectToServer(conf);
}

void MainWindow::logout()
{
   qDebug() << "Client Disconnectting.";

   m_xmppClient->disconnectFromServer();
   m_loginWidget->unlockLogin();
}

void MainWindow::cancelLogin()
{
    m_xmppClient->disconnectFromServer();
    m_loginWidget->showLoginState("Sign in cancelled");
}

void MainWindow::quit()
{
    qDebug() << "Logging out...";
    logout();

    writePreferences();
}

void MainWindow::clientDisconnected()
{
    qDebug() << "Disconnected successful.";

}

void MainWindow::clientConnected()
{
    qDebug() << "Connect successful";
    m_loginWidget->setHidden(true);
    ui->avatarLb->setHidden(false);
    ui->nameLb->setHidden(false);
    ui->signatureLb->setHidden(false);
    ui->stateCbox->setHidden(false);
    ui->treeWidget->setHidden(false);

    const QString bareJid = m_xmppClient->configuration().jidBare();
    // 初始化登录者信息
    updateVCard(bareJid);
}

void MainWindow::clientError(QXmppClient::Error error)
{
    QString m_errorString;
    switch ( error )
    {
    case QXmppClient::NoError:
        m_errorString = "No error";
        break;
    case QXmppClient::SocketError:
        m_errorString = tr("Socket error.<br>Can not connect server.");
        break;
    case QXmppClient::KeepAliveError:
        m_errorString = tr("No response.<br>Keep alive error");
        break;
    case QXmppClient::XmppStreamError:
        switch(m_xmppClient->xmppStreamError())
        {
        case QXmppStanza::Error::NotAuthorized:
            m_errorString = tr("Invalid password");
            break;
        case QXmppStanza::Error::ServiceUnavailable:
            m_errorString = tr("Server unavailable.");
            break;
        case QXmppStanza::Error::RemoteServerTimeout:
            m_errorString = tr("Remote server time out.");
            break;
        default:
            m_errorString = tr("Stream error");
            break;
        }
        break;
    default:
        m_errorString = tr("Unknow error!");
        break;
    }

    if( !m_errorString.isEmpty() )
    {
        qDebug() << m_errorString;
        m_loginWidget->showLoginState(m_errorString);
        m_loginWidget->unlockLogin();
        m_xmppClient->disconnectFromServer();

    }

}

void MainWindow::updateVCard(const QString &bareJid)
{
    // 获取全名
    const QXmppVCardIq vCard = m_xmppClient->vCardManager().clientVCard();
    QString fullName = vCard.fullName();
    if (fullName.isEmpty())
        fullName = bareJid;

    // 获取头像
    //...

    // 更新用户信息
    if (bareJid == m_xmppClient->configuration().jidBare())
    {
        // 更新自己的信息
        ui->nameLb->setText(bareJid);
        ui->signatureLb->setText(fullName);
    }
    else
    {
        // 更新其他用户信息
//        m_rosterItemModel.updateAvatar(bareJid, avatar);
//        m_rosterItemModel.updateName(bareJid, fullName);
    }
}


UserChatDialog *MainWindow::userChatDialog(const QString &bareJid)
{
    RosterWidgetItem* rosterItem = ui->treeWidget->rosterItemFromJid(bareJid);
    if(!rosterItem)
        return 0;

    UserChatDialog* chatDlg = rosterItem->chatDialog(bareJid);
    if(!chatDlg)
        return 0;

    return chatDlg;
}

void MainWindow::messageReceived(const QXmppMessage &msg)
{
//    qDebug() << msg.body();
    if (msg.body().isEmpty())
        return;

    UserChatDialog *dialog = userChatDialog(QXmppUtils::jidToBareJid(msg.from()));
    if (dialog)
    {
        dialog->show();
        dialog->messageReceived(msg.body());
    }
}

