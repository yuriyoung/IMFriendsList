#include "RosterWidgetItem.h"
#include "ui_RosterWidgetItem.h"
#include "UserChatDialog.h"

#include <QTreeWidgetItem>

RosterWidgetItem::RosterWidgetItem(QTreeWidgetItem *group, QTreeWidgetItem *item, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RosterWidgetItem),
    m_groupItem(group),
    m_onItem(item),
    m_chatDlg(new UserChatDialog),
    m_client(0)
{
    ui->setupUi(this);
    setAvatar(":/icon/buddy-green.png");
}

RosterWidgetItem::~RosterWidgetItem()
{
    delete ui;
}

void RosterWidgetItem::setName(const QString &username)
{
    ui->lb_username->setText( username );
    ui->lb_username->setToolTip(username);
    m_username = username;
}

void RosterWidgetItem::setBareJid(const QString &jid)
{
    m_bareJid = jid;
}

void RosterWidgetItem::setSignature(const QString &signature)
{
    ui->lb_signature->setText( signature );
    ui->lb_signature->setToolTip( signature);
    m_signature = signature;
}

void RosterWidgetItem::setAvatar(const QString &avatar)
{
    QPixmap pix(avatar);
    ui->lb_avatar->setPixmap(pix);
    ui->lb_avatar->setToolTip( m_username );
    m_avatar = pix.toImage();
}

void RosterWidgetItem::setAvatar(const QImage &image)
{
    QPixmap pix(QPixmap::fromImage(image));
    ui->lb_avatar->setPixmap(pix);
    ui->lb_avatar->setToolTip( m_username );
    m_avatar = image;
}

// 头像变灰
void RosterWidgetItem::grayAvatar()
{
    int w = m_avatar.width();
    int h = m_avatar.height();
    QImage iGray(w,h,QImage::Format_ARGB32_Premultiplied);
    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            QRgb pixel = m_avatar.pixel(i, j);
            int gray = qGray(pixel);
            int alpha = qAlpha(pixel);
            QRgb grayPixel = qRgba(gray, gray, gray, alpha);
            iGray.setPixel(i, j, grayPixel);
        }
    }
    ui->lb_avatar->setPixmap( QPixmap::fromImage(iGray) );
}

void RosterWidgetItem::setStatusIcon(const QString &statusIcon,
                                     const QString &statusTip)
{
    QPixmap pix(statusIcon);
    ui->lb_status->setPixmap(pix);
    ui->lb_status->setToolTip( statusTip );
}

void RosterWidgetItem::setPresence(const QXmppPresence &presence)
{
    // 获取用户状态文本
    QString statusText = presence.statusText();
    if (statusText.isEmpty())
    {
        if(presence.type() == QXmppPresence::Available)
            statusText = "Available";
        else if(presence.type() == QXmppPresence::Unavailable)
            statusText = "Offline";
    }

    // 更新在线状态图标
    QString statusIcon, statusTip;
    if (presence.type() == QXmppPresence::Available)
    {
        setAvatar(m_avatar);
        switch (presence.availableStatusType())
        {
        case QXmppPresence::Online:
        case QXmppPresence::Chat:
            statusIcon = "status-green";
            statusTip = tr("Online");
            break;
        case QXmppPresence::Away:
        case QXmppPresence::XA:
            statusIcon = "status-orange";
            statusTip = tr("Away");
            break;
        case QXmppPresence::DND:
            statusIcon = "status-red";
            statusTip = tr("Don't disturb");
            break;
        case QXmppPresence::Invisible:
            statusIcon = "status-gray";
            statusTip = tr("Offline");
            break;
        }
    }
    else
    {
        statusIcon = "status-gray";
        grayAvatar();
    }

    if (!statusIcon.isEmpty())
        setStatusIcon(":/icon/"+statusIcon+".png", statusTip);

}

RosterWidgetItem *RosterWidgetItem::rosterItem(const QString &barejid)
{
    if( barejid == m_bareJid)
        return this;

    return 0;
}

UserChatDialog *RosterWidgetItem::chatDialog(const QString &barejid)
{
    if( barejid == m_bareJid)
        return this->m_chatDlg;

    return 0;
}

void RosterWidgetItem::setQXmppClient(QXmppClient *client)
{
    m_client = client;
    m_chatDlg->setQXmppClient(m_client);
    m_chatDlg->setBareJid(m_bareJid);
}

void RosterWidgetItem::doubleClicked(QTreeWidgetItem *item, int)
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
