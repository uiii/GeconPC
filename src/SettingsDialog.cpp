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

#include "SettingsDialog.hpp"
#include "ui_SettingsDialog.hpp"

namespace Gecon
{
    SettingsDialog::SettingsDialog(ControlInfo::Control* control, QWidget *parent) :
        QDialog(parent),
        control_(control),
        ui(new Ui::SettingsDialog)
    {
        ui->setupUi(this);

        connect(ui->deviceList, SIGNAL(currentIndexChanged(int)), this, SLOT(selectDevice(int)));

        updateDeviceList();
    }

    SettingsDialog::~SettingsDialog()
    {
        delete ui;
    }

    int SettingsDialog::exec()
    {
        updateDeviceList();

        return QDialog::exec();
    }

    void SettingsDialog::updateDeviceList()
    {
        DeviceAdapter controlDevice = control_->device();
        ui->deviceList->clear();

        ControlInfo::DevicePolicy::DeviceAdapterList devices = devicePolicy_.getAvailableDevices();

        for(DeviceAdapter& device : devices)
        {
            ui->deviceList->addItem(device.name().c_str(), QVariant::fromValue(device));

            if(device == controlDevice)
            {
                ui->deviceList->setCurrentIndex(ui->deviceList->count() - 1);
            }
        }
    }

    void SettingsDialog::selectDevice(int index)
    {
        if(index != -1)
        {
            control_->setDevice(ui->deviceList->itemData(index).value<ControlInfo::DevicePolicy::DeviceAdapter>());
        }
    }
}
