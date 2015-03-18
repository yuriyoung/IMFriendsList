#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QXmpp/QXmppClient.h>
#include <QXmpp/QXmppVCardIq.h>

class Preferences;
class LoginWidget;
class UserChatDialog;

class QXmppMessage;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void readPreferences();
    void writePreferences();
    void login();
    void logout();
    void cancelLogin();
    void quit();
    void clientDisconnected();
    void clientConnected();
    void clientError(QXmppClient::Error);
    void updateVCard(const QString& bareJid);
    void messageReceived(const QXmppMessage& msg);

private:
    void initialize();
    UserChatDialog*     userChatDialog(const QString& bareJid);

    Ui::MainWindow *ui;

    Preferences*        m_preferences;
    QXmppClient*        m_xmppClient;
    LoginWidget*        m_loginWidget;
};

#endif // MAINWINDOW_H
