#include "NewObjectDialog.hpp"
#include "ui_NewObjectDialog.h"

namespace Gecon {

NewObjectDialog::NewObjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewObjectDialog)
{
    ui->setupUi(this);
}

NewObjectDialog::~NewObjectDialog()
{
    delete ui;
}

} // namespace Gecon
