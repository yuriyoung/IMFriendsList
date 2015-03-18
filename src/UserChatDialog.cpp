#include "UserChatDialog.h"
#include "ui_UserChatDialog.h"

#include <QFont>
#include <QFontMetrics>
#include <QDebug>

UserChatDialog::UserChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserChatDialog),
    m_client(0)
{
    ui->setupUi(this);
    this->setWindowFlags(windowFlags() &~Qt::WindowContextHelpButtonHint );

    ui->splitter->setStretchFactor(0, 80);
    ui->splitter->setStretchFactor(1, 20);

    connect(ui->btn_close, SIGNAL(clicked(bool)),
                         this, SLOT(close()));
    bool check = connect(ui->btn_send, SIGNAL(clicked(bool)),
                         this, SLOT(sendMessage()));
    Q_ASSERT(check);
    Q_UNUSED(check);
}

UserChatDialog::~UserChatDialog()
{
    delete ui;
}

void UserChatDialog::setBareJid(const QString &jid)
{
    m_bareJid = jid;
}

QString UserChatDialog::bareJid() const
{
    return m_bareJid;
}

void UserChatDialog::setDisplayName(const QString &name)
{
    m_displayName = name;

    this->setWindowTitle(QString("Chat with %1").arg(m_bareJid));
    QFont font;
    font.setBold(true);
    QFontMetrics fontMetrics(font);
    QRect rect = fontMetrics.boundingRect(m_displayName);
    int width = rect.width();
    ui->edt_recv->setFixedWidth(350 - width - 25);
}

QString UserChatDialog::displayName() const
{
    return m_displayName;
}

void UserChatDialog::setQXmppClient(QXmppClient *client)
{
    m_client = client;
}

void UserChatDialog::messageReceived(const QString &msg)
{
    ui->edt_recv->append(msg);
}

void UserChatDialog::sendMessage()
{
    if(m_client)
        m_client->sendMessage(bareJid(), ui->edt_send->toHtml());
    else
        qDebug() << "xmppclient is null";

    ui->edt_recv->append(ui->edt_send->toHtml());
    ui->edt_send->clear();
}

void UserChatDialog::keyPressEvent(QKeyEvent *e)
{
    ui->edt_send->setFocus();
//    ui->edt_send->event(e);
    if(e->key() == Qt::Key_Return)
    {
        ui->btn_send->click();
    }
    else if(e->key() == Qt::Key_Escape)
    {
        this->close();
    }
}
