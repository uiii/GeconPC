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
#include <QtConcurrentRun>
#include <QtTest/QTest>

#include "ObjectModel.hpp"

namespace Gecon
{
    ObjectDialog::ObjectDialog(ObjectModel* objectModel, QWidget *parent):
        QDialog(parent),
        colorGrabbed_(false),
        objectModel_(objectModel),
        ui_(new Ui::ObjectDialog)
    {
        ui_->setupUi(this);

        connect(this, SIGNAL(finished(int)), this, SLOT(stopCapture()));
        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(addObject()));

        connect(control_.signaler().get(), SIGNAL(objectsRecognized(Image,Image,ObjectSet)), this, SLOT(displayImage(Image,Image,ObjectSet)), Qt::BlockingQueuedConnection);
        connect(ui_->display, SIGNAL(imageDisplayed()), this, SLOT(firstImageDisplayed()));
    }

    ObjectDialog::~ObjectDialog()
    {
        delete ui_;
    }

    int ObjectDialog::exec()
    {
        return QDialog::Rejected;
    }

    int ObjectDialog::newObject()
    {
        reset();

        startCapture();

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
            objectModel_->addObject(ui_->objectName->text(), objectColor_);

            accept();
        }
        catch(...)
        {
            // TODO
        }
    }

    void ObjectDialog::setDevice(ObjectDialog::DeviceAdapter device)
    {
        control_.setDevice(device);
    }

    void ObjectDialog::reset()
    {
        colorGrabbed_ = false;
        ui_->objectName->setText(QString());
    }

    void ObjectDialog::startCapture()
    {
        ui_->display->reset();

        control_.prepareObjects(ObjectSet());
        control_.start();
    }

    void ObjectDialog::stopCapture()
    {
        QtConcurrent::run(&control_, &ControlInfo::Control::stop);
    }

    void ObjectDialog::displayImage(Image original, Image segmented, ObjectSet objects)
    {
        rawImage_ = original;

        ui_->display->displayImage(rawImage_, objects);
    }

    void ObjectDialog::firstImageDisplayed()
    {
        disconnect(ui_->display, SIGNAL(imageDisplayed()), this, SLOT(firstImageDisplayed()));
        connect(ui_->display, SIGNAL(clicked(QMouseEvent*)), this, SLOT(grabColor(QMouseEvent*)));
    }

    void ObjectDialog::grabColor(QMouseEvent *event)
    {
        objectColor_ = rawImage_.at(event->pos().x(), event->pos().y());

        ControlInfo::ObjectPolicy::ObjectSet objects;
        object_ = Object(objectColor_);
        objects.insert(&object_);
        control_.prepareObjects(objects);

        QtConcurrent::run(&control_, &ControlInfo::Control::restart);

        colorGrabbed_ = true;
    }
} // namespace Gecon
