#include "userchat_dialog.h"
#include "ui_userchatdialog.h"

UserChatDialog::UserChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserChatDialog)
{
    ui->setupUi(this);
}

UserChatDialog::~UserChatDialog()
{
    delete ui;
}
