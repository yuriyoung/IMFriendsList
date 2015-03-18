#ifndef USERCHAT_DIALOG_H
#define USERCHAT_DIALOG_H

#include <QDialog>
#include <QKeyEvent>

#include <QXmpp/QXmppClient.h>

namespace Ui {
class UserChatDialog;
}

class UserChatDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit UserChatDialog(QWidget *parent = 0);
    ~UserChatDialog();

    void        setBareJid(const QString &jid);
    QString     bareJid() const;
    void        setDisplayName(const QString &name);
    QString     displayName() const;
    void        setQXmppClient(QXmppClient* client);
    void        messageReceived(const QString &msg);

private slots:
    void        sendMessage();

protected:
    void keyPressEvent(QKeyEvent*);


private:
    Ui::UserChatDialog *ui;

    QXmppClient*        m_client;
    QString             m_bareJid;
    QString             m_displayName;
};

#endif // USERCHAT_DIALOG_H
