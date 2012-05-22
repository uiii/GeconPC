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
#include "ObjectWrapper.hpp"

namespace Gecon
{
    ObjectDialog::ObjectDialog(ObjectModel* objectModel, QWidget *parent):
        DialogBase(parent),
        rawObject_(0),
        editedObject_(0),
        objectModel_(objectModel),
        ui_(new Ui::ObjectDialog)
    {
        ui_->setupUi(this);

        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(addObject_()));
        connect(ui_->deleteButton, SIGNAL(clicked()), this, SLOT(deleteObject_()));

        reset_();
    }

    ObjectDialog::~ObjectDialog()
    {
        delete ui_;
    }

    int ObjectDialog::exec()
    {
        return QDialog::Rejected;
    }

    void ObjectDialog::beforeClose_()
    {
        stopCapture_();
        reset_();
    }

    int ObjectDialog::newObject()
    {
        startCapture_();

        return QDialog::exec();
    }

    int ObjectDialog::editObject(ObjectWrapper *object)
    {
        editedObject_ = object;
        rawObject_ = editedObject_->rawObject();

        ui_->objectName->setText(object->name());
        ui_->deleteButton->setVisible(true);

        disconnect(ui_->buttonBox, SIGNAL(accepted()), this, 0);
        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(updateObject_()));

        startCapture_();

        return QDialog::exec();
    }

    void ObjectDialog::addObject_()
    {
        if(! rawObject_)
        {
            QMessageBox::critical(this, "Color object error", "No object selected!", QMessageBox::Ok);
            return;
        }

        try
        {
            objectModel_->addObject(ui_->objectName->text(), rawObject_->color());

            accept();
        }
        catch(const std::exception& e)
        {
            QMessageBox::critical(this, "Color object error", e.what(), QMessageBox::Ok);
        }
    }

    void ObjectDialog::updateObject_()
    {
        try
        {
            QModelIndex index = objectModel_->index(editedObject_);
            objectModel_->editObject(index, ui_->objectName->text(), rawObject_->color());

            accept();
        }
        catch(const std::exception& e)
        {
            QMessageBox::critical(this, "Color object error", e.what(), QMessageBox::Ok);
        }
    }

    void ObjectDialog::deleteObject_()
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, tr("Delete question"),
            tr("Do you really want to delete object '%1'").arg(editedObject_->name()), QMessageBox::Ok | QMessageBox::Cancel);

        if(button == QMessageBox::Ok)
        {
            stopCapture_();

            QModelIndex index = objectModel_->index(editedObject_);
            if(objectModel_->removeObject(index))
            {
                accept();
            }
        }
    }

    void ObjectDialog::setDevice(ObjectDialog::DeviceAdapter device)
    {
        control_.setDevice(device);
    }

    void ObjectDialog::reset_()
    {
        if(! editedObject_)
        {
            delete rawObject_;
        }

        rawObject_ = 0;
        editedObject_ = 0;

        rawImage_ = Image();

        ui_->objectName->setText(QString());
        ui_->deleteButton->setVisible(false);

        disconnect(ui_->buttonBox, SIGNAL(accepted()), this, 0);
        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(addObject_()));
    }

    void ObjectDialog::startCapture_()
    {
        connect(control_.objectPolicySignaler(), SIGNAL(objectsRecognized(Image,Image,Objects)), this, SLOT(displayImage_(Image,Image,Objects)), Qt::BlockingQueuedConnection);
        connect(ui_->display, SIGNAL(imageDisplayed()), this, SLOT(firstImageDisplayed_()));

        ui_->display->reset();

        ControlInfo::Objects objects;
        if(rawObject_)
        {
            objects.insert(rawObject_);
        }

        control_.prepareObjects(objects);
        control_.start();
    }

    void ObjectDialog::stopCapture_()
    {
        if(! control_.isRunning())
        {
            return;
        }

        disconnect(ui_->display, SIGNAL(clicked(QPoint)), this, SLOT(grabColor_(QPoint)));
        disconnect(control_.objectPolicySignaler(), SIGNAL(objectsRecognized(Image,Image,Objects)), this, SLOT(displayImage_(Image,Image,Objects)));
        QCoreApplication::processEvents();

        control_.stop();
    }

    void ObjectDialog::displayImage_(const Image &original, const Image &segmented, const ControlInfo::Objects &objects)
    {
        rawImage_ = original;

        ui_->display->displayImage(rawImage_, objects);
        //ui_->display->displayImage(segmented, objects);
    }

    void ObjectDialog::firstImageDisplayed_()
    {
        disconnect(ui_->display, SIGNAL(imageDisplayed()), this, SLOT(firstImageDisplayed_()));
        connect(ui_->display, SIGNAL(clicked(QPoint)), this, SLOT(grabColor_(QPoint)));
    }

    void ObjectDialog::grabColor_(QPoint position)
    {
        ControlInfo::Object::Color objectColor = rawImage_.at(position.x(), position.y());
        rawObject_ = new ControlInfo::Object(objectColor);

        ControlInfo::Objects objects;
        objects.insert(rawObject_);
        control_.prepareObjects(objects);

        QtConcurrent::run(&control_, &ControlInfo::Control::restart);
    }
} // namespace Gecon
