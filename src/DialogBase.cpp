#include "DialogBase.hpp"

namespace Gecon
{
    DialogBase::DialogBase(QWidget *parent) :
        QDialog(parent)
    {
    }

    void DialogBase::accept()
    {
        beforeClose_();
        QDialog::accept();
    }

    void DialogBase::reject()
    {
        beforeClose_();
        QDialog::reject();
    }

    void DialogBase::closeEvent(QCloseEvent *)
    {
        beforeClose_();
    }
} // namespace Gecon
