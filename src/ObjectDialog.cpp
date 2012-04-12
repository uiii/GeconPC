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

#include "ObjectDialog.hpp"
#include "ui_ObjectDialog.hpp"

#include <QPainter>
#include <QMessageBox>

namespace Gecon
{
    ObjectDialog::ObjectDialog(ObjectModel* objectModel, QWidget *parent):
        QDialog(parent),
        colorGrabbed_(false),
        objectModel_(objectModel),
        ui_(new Ui::ObjectDialog)
    {
        ui_->setupUi(this);
        setFixedSize(size());

        captureTimer_.setSingleShot(30);

        connect(this, SIGNAL(finished(int)), this, SLOT(stopCapture()));
        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(addObject()));

        connect(&captureTimer_, SIGNAL(timeout()), &capture_, SLOT(captureImage()));
        connect(&capture_, SIGNAL(finished()), this, SLOT(displayImage()));

        connect(ui_->display, SIGNAL(clicked(QMouseEvent*)), this, SLOT(grabColor(QMouseEvent*)));
    }

    ObjectDialog::~ObjectDialog()
    {
        delete ui_;
    }

    int ObjectDialog::exec()
    {
        return QDialog::Rejected;
    }

    int ObjectDialog::newObject(ObjectDialog::DeviceAdapter device)
    {
        colorGrabbed_ = false;

        startCapture(device);

        return QDialog::exec();
    }

    void ObjectDialog::addObject()
    {
        if(! colorGrabbed_)
        {
            QMessageBox::critical(this, "Color object error", "No object selected!", QMessageBox::Ok);
            return;
        }

        try
        {
            QString name = ui_->objectName->text();
            if(name.isEmpty())
            {
                name = "Object";
            }

            ObjectWrapper object(name, objectColor_);

            objectModel_->addObject(object);

            accept();
        }
        catch(...)
        {
            // TODO
        }
    }

    void ObjectDialog::startCapture(ObjectDialog::DeviceAdapter device)
    {
        capture_.reset();
        capture_.setDevice(device);

        capture_.start();
        capture_.captureImage();
        capture_.wait();
    }

    void ObjectDialog::stopCapture()
    {
        captureTimer_.stop();
        capture_.stop();
    }

    void ObjectDialog::displayImage()
    {
        rawImage_ = capture_.rawImage();
        ui_->display->displayImage(capture_.image());

        if(isVisible())
        {
            captureTimer_.start();
        }
    }

    void ObjectDialog::grabColor(QMouseEvent *event)
    {
        objectColor_ = rawImage_.at(event->pos().x(), event->pos().y());

        capture_.setObjectColor(objectColor_);

        colorGrabbed_ = true;
    }
} // namespace Gecon
