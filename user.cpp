#include "user.h"
#include "ui_user.h"
#include "UserChatDialog.h"

#include <QTreeWidgetItem>

User::User(QTreeWidgetItem *group, QTreeWidgetItem *item, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::User),
    m_groupItem(group),
    m_onItem(item),
    m_chatDlg(new UserChatDialog)
{
    ui->setupUi(this);
}

User::~User()
{
    delete ui;
}

void User::setUserName(const QString &username)
{
    ui->lb_username->setText( username );
    ui->lb_username->setToolTip(username);
    m_username = username;
}

void User::setSignature(const QString &signature)
{
    ui->lb_signature->setText( signature );
    ui->lb_signature->setToolTip( signature);
    m_signature = signature;
}

void User::setAvatar(const QString &avatar)
{
    QPixmap pix(avatar);
    ui->lb_avatar->setPixmap(pix);
    ui->lb_avatar->setToolTip( m_username );
    m_avatar = avatar;
}

void User::doubleClicked(QTreeWidgetItem *item, int)
{
    if( m_onItem != item )
        return;
    if( !m_chatDlg )
        m_chatDlg = new UserChatDialog;
    m_chatDlg->setWindowTitle( m_username );

    if( !m_chatDlg->isHidden() )
    {
        m_chatDlg->raise();
        m_chatDlg->activateWindow();
        return;
    }

    m_chatDlg->show();
}
