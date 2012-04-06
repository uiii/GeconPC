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

#include "NewObjectDialog.hpp"
#include "ui_NewObjectDialog.hpp"

#include <QPainter>
#include <QMessageBox>

namespace Gecon
{
    NewObjectDialog::NewObjectDialog(ControlInfo::Control* control, QWidget *parent):
        QDialog(parent),
        object_(0),
        control_(control),
        ui_(new Ui::NewObjectDialog)
    {
        ui_->setupUi(this);
        setFixedSize(size());

        captureTimer_.setSingleShot(30);

        connect(this, SIGNAL(finished(int)), this, SLOT(stopCapture()));
        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(newObject()));

        connect(&captureTimer_, SIGNAL(timeout()), &capture_, SLOT(captureImage()));
        connect(&capture_, SIGNAL(finished()), this, SLOT(displayImage()));

        connect(ui_->display, SIGNAL(clicked(QMouseEvent*)), this, SLOT(grabObject(QMouseEvent*)));
    }

    NewObjectDialog::~NewObjectDialog()
    {
        delete ui_;
    }

    int NewObjectDialog::exec()
    {
        startCapture();

        return QDialog::exec();
    }

    void NewObjectDialog::newObject()
    {
        // TODO

        accept();
    }

    void NewObjectDialog::startCapture()
    {
        capture_.setDevice(control_->device());
        capture_.setObject(0);

        capture_.start();
        capture_.captureImage();
        capture_.wait();
    }

    void NewObjectDialog::stopCapture()
    {
        captureTimer_.stop();
        capture_.stop();
    }

    void NewObjectDialog::displayImage()
    {
        rawImage_ = capture_.rawImage();
        ui_->display->displayImage(capture_.image());

        if(isVisible())
        {
            captureTimer_.start();
        }
    }

    void NewObjectDialog::grabObject(QMouseEvent *event)
    {
        object_ = new Object(rawImage_.at(event->pos().x(), event->pos().y()));

        capture_.setObject(object_);
    }
} // namespace Gecon
