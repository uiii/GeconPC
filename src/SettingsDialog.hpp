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

#ifndef GECON_SETTINGSDIALOG_HPP
#define GECON_SETTINGSDIALOG_HPP

#include <QDialog>
#include <QMetaType>

#include "ControlInfo.hpp"

namespace Gecon
{
    typedef V4L2VideoDevicePolicy<Image<RGB> > DevicePolicy;
}

Q_DECLARE_METATYPE(Gecon::DevicePolicy::DeviceAdapter)

namespace Gecon
{
    namespace Ui
    {
        class SettingsDialog;
    }

    class SettingsDialog : public QDialog
    {
        Q_OBJECT

        typedef ControlInfo::DevicePolicy::DeviceAdapter DeviceAdapter;

    public:
        explicit SettingsDialog(ControlInfo::Control* control, QWidget *parent = 0);
        ~SettingsDialog();

    public slots:
        int exec();
        void updateDeviceList();
        void selectDevice(int index);

    private:
        ControlInfo::Control* control_;

        ControlInfo::DevicePolicy devicePolicy_;

        Ui::SettingsDialog *ui;
    };
} // namespace Gecon

#endif // SETTINGSDIALOG_HPP
