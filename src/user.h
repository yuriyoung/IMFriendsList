#ifndef USER_H
#define USER_H

#include <QWidget>

class UserChatDialog;
class QTreeWidgetItem;

namespace Ui {
class User;
}

/**
 *  为每一个用户都制定一个聊天窗口
 *  在每户类中声明一个聊天窗口
 *  通过用户指针,可以找到自己的聊天窗口
 */
class User : public QWidget
{
    Q_OBJECT
    
public:
    explicit User(QTreeWidgetItem *group, QTreeWidgetItem *item, QWidget *parent = 0);
    ~User();
    
    /*设置和获取用户名称*/
    void    setUserName( const QString &username);
    QString userName() const { return m_username; }

    /*设置和获取用户的签名*/
    void    setSignature( const QString &signature );
    QString signature() const { return m_signature; }

    /*设置和获取用户的头像,返回头像所在的图片位置*/
    void    setAvatar( const QString &avatar );
    QString avatar() const { return m_avatar; }

public slots:
    void doubleClicked(QTreeWidgetItem*, int);

private:
    Ui::User            *ui;
    UserChatDialog      *m_chatDlg;
    QTreeWidgetItem     *m_onItem;
    QTreeWidgetItem     *m_groupItem;

    QString             m_username;
    QString             m_signature;
    QString             m_avatar;       // user QIcon?
};

#endif // USER_H
