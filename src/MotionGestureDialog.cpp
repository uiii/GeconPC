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

#include "MotionGestureDialog.hpp"
#include "ui_MotionGestureDialog.hpp"

namespace Gecon
{
    MotionGestureDialog::MotionGestureDialog(QWidget *parent) :
        QDialog(parent),
        ui_(new Ui::MotionGestureDialog)
    {
        ui_->setupUi(this);

        captureTimer_.setSingleShot(30);
        countdownTimer_.setInterval(1000);

        connect(&captureTimer_, SIGNAL(timeout()), &capture_, SLOT(captureImage()));
        connect(&capture_, SIGNAL(finished()), this, SLOT(displayImage()));

        connect(&countdownTimer_, SIGNAL(timeout()), this, SLOT(countdown()));

        connect(ui_->recordMotionButton, SIGNAL(clicked()), this, SLOT(startCapture()));
    }
    
    MotionGestureDialog::~MotionGestureDialog()
    {
        delete ui_;
    }

    void MotionGestureDialog::startCapture()
    {
        capture_.reset();

        capture_.start();
        capture_.captureImage();
        capture_.wait();

        countdownCount_ = 3;
        countdownTimer_.start();
    }

    void MotionGestureDialog::stopCapture()
    {
        captureTimer_.stop();
        capture_.stop();
    }

    void MotionGestureDialog::displayImage()
    {
        ui_->display->displayImage(capture_.image());

        if(isVisible())
        {
            captureTimer_.start();
        }
    }

    void MotionGestureDialog::countdown()
    {
        /*ui_->display->
        countdownCount_*/
    }

    int MotionGestureDialog::newGesture(DeviceAdapter device)
    {
        capture_.setDevice(device);

        return QDialog::exec();
    }

    int MotionGestureDialog::exec()
    {
        return QDialog::Rejected;
    }
} // namespace Gecon
