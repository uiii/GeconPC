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

#ifndef GECON_MAINWINDOW_HPP
#define GECON_MAINWINDOW_HPP

#include <QMainWindow>

#include "ControlInfo.hpp"

#include "SettingsDialog.hpp"
#include "NewObjectDialog.hpp"

namespace Gecon
{
    namespace Ui
    {
        class MainWindow;
    }

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public slots:
        void newObject();

    private:
        ControlInfo::Control control_;

        SettingsDialog* settingsDialog_;
        NewObjectDialog* newObjectDialog_;

        Ui::MainWindow *ui;
    };
} // namespace Gecon
#endif // GECON_MAINWINDOW_HPP
