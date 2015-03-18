#ifndef ROSTERWIDGETITEM_H
#define ROSTERWIDGETITEM_H

#include <QWidget>
#include <QXmpp/QXmppPresence.h>
#include <QXmpp/QXmppClient.h>

class UserChatDialog;
class QTreeWidgetItem;

namespace Ui {
class RosterWidgetItem;
}

/**
 *  为每一个用户都制定一个聊天窗口
 *  在每户类中声明一个聊天窗口
 *  通过用户指针,可以找到自己的聊天窗口
 */
class RosterWidgetItem : public QWidget
{
    Q_OBJECT
    
public:
    explicit RosterWidgetItem(QTreeWidgetItem *group, QTreeWidgetItem *item, QWidget *parent = 0);
    ~RosterWidgetItem();
    
    /*设置和获取用户名称*/
    void    setName( const QString &username);
    QString name() const { return m_username; }

    void    setBareJid(const QString &jid);
    QString bareJid()const { return m_bareJid; }

    /*设置和获取用户的签名*/
    void    setSignature( const QString &signature );
    QString signature() const { return m_signature; }

    /*设置和获取用户的头像,返回头像所在的图片位置*/
    void    setAvatar( const QString &avatar );
    void    setAvatar(const QImage& image);
    QImage  avatar() const { return m_avatar; }
    void    grayAvatar();

    void    setStatusIcon(const QString &statusIcon, const QString &statusTip);

    // 是否在线
    void setPresence(const QXmppPresence &presence);

    RosterWidgetItem*   rosterItem(const QString &barejid);
    UserChatDialog*     chatDialog(const QString &barejid);
    void                setQXmppClient(QXmppClient* client);
public slots:
    void doubleClicked(QTreeWidgetItem*, int);

private:
    Ui::RosterWidgetItem*ui;
    QXmppClient*         m_client;
    UserChatDialog      *m_chatDlg;
    QTreeWidgetItem     *m_onItem;
    QTreeWidgetItem     *m_groupItem;

    QString             m_username;
    QString             m_bareJid;
    QString             m_signature;
    QImage              m_avatar;
};

#endif // ROSTERWIDGETITEM_H
