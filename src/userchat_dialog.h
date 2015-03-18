#ifndef USERCHAT_DIALOG_H
#define USERCHAT_DIALOG_H

#include <QDialog>

namespace Ui {
class UserChatDialog;
}

class UserChatDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit UserChatDialog(QWidget *parent = 0);
    ~UserChatDialog();
    
private:
    Ui::UserChatDialog *ui;
};

#endif // USERCHAT_DIALOG_H
