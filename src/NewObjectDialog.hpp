#ifndef GECON_NEWOBJECTDIALOG_HPP
#define GECON_NEWOBJECTDIALOG_HPP

#include <QDialog>

namespace Gecon {

namespace Ui {
class NewObjectDialog;
}

class NewObjectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewObjectDialog(QWidget *parent = 0);
    ~NewObjectDialog();
    
private:
    Ui::NewObjectDialog *ui;
};


} // namespace Gecon
#endif // GECON_NEWOBJECTDIALOG_HPP
