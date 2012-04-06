/**
 * This file is part of the Gecon PC project (https://github.com/uiii/GeconPC).
 *
 * Copyright (C) 2012 by Richard Jedlička <uiii.dev@gmail.com>
 *
 * Gecon PC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Gecon PC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Gecon PC. If not, see <http://www.gnu.org/licenses/>.
 */

#include "MainWindow.hpp"
#include "ui_MainWindow.hpp"

namespace Gecon
{
    MainWindow::MainWindow(QWidget *parent):
        QMainWindow(parent),
        settingsDialog_(new SettingsDialog(&control_)),
        newObjectDialog_(new NewObjectDialog(&control_)),
        ui(new Ui::MainWindow)
    {
        ui->setupUi(this);

        connect(ui->newObjectButton, SIGNAL(clicked()), this, SLOT(newObject()));
        connect(ui->actionSettings, SIGNAL(triggered()), settingsDialog_, SLOT(exec()));
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }

    void MainWindow::newObject()
    {
        int result = newObjectDialog_->exec();

        if(result == QDialog::Accepted)
        {
            // TODO newObjectDialog_->object();
        }
    }
} // namespace Gecon
