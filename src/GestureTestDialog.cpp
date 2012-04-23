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

#include "GestureTestDialog.hpp"
#include "ui_GestureTestDialog.hpp"

#include <QtConcurrentRun>
#include <QScrollBar>

#include "GestureModel.hpp"
#include "ObjectModel.hpp"

namespace Gecon
{
    GestureTestDialog::GestureTestDialog(GestureModel* gestureModel, ObjectModel* objectModel, QWidget *parent) :
        QDialog(parent),
        gestureModel_(gestureModel),
        objectModel_(objectModel),
        objectsStatesModel_(new QStandardItemModel(this)),
        ui_(new Ui::GestureTestDialog)
    {
        ui_->setupUi(this);

        ui_->objectsStates->setModel(objectsStatesModel_);

        connect(ui_->buttonBox, SIGNAL(rejected()), this, SLOT(close_()));
    }
    
    GestureTestDialog::~GestureTestDialog()
    {
        delete ui_;
    }

    int GestureTestDialog::exec()
    {
        return QDialog::Rejected;
    }

    void GestureTestDialog::logEvent_(GestureWrapper *gesture, const QString& eventDescription)
    {
        QTextCursor cursor = ui_->eventLog->textCursor();

        if(! cursor.atStart())
        {
            cursor.insertHtml("<br />");
        }

        cursor.insertHtml(tr("Objects:"));
        cursor.insertText(" ");

        bool firstObject = true;
        GestureWrapper::Objects objects = gesture->objects();
        for(GestureWrapper::Object* object : objects)
        {
            QImage img(12, 12, QImage::Format_RGB888);
            img.fill(object->color());

            if(! firstObject)
            {
                cursor.insertText(", ");
            }

            cursor.insertImage(img);
            cursor.insertText(" ");
            cursor.insertHtml(tr("<strong>%1</strong>").arg(object->name()));

            firstObject = false;
        }

        cursor.insertText("\t\t");
        cursor.insertHtml(tr("Gesture: <strong>%1</strong>").arg(gesture->name()));
        cursor.insertText("\t\t");
        cursor.insertHtml(tr("Event: <strong>%1</strong>").arg(eventDescription));

        ui_->eventLog->verticalScrollBar()->setValue(ui_->eventLog->verticalScrollBar()->maximum());
    }

    void GestureTestDialog::includeGesture_(StateGestureWrapper* stateGesture)
    {
        StateGestureWrapper::RawGesture* rawGesture = stateGesture->rawGesture();
        gestures_.insert(rawGesture);

        Event::Trigger* stateEnterTrigger = new Event::Trigger([=](){ emit eventOccured(stateGesture, "State enter"); });
        Event::Trigger* stateLeaveTrigger = new Event::Trigger([=](){ emit eventOccured(stateGesture, "State leave"); });

        stateEnterTrigger->addSwitch(rawGesture->stateEnterEvent());
        stateLeaveTrigger->addSwitch(rawGesture->stateLeaveEvent());

        triggers_.push_back(stateEnterTrigger);
        triggers_.push_back(stateLeaveTrigger);
    }

    void GestureTestDialog::includeGesture_(RelationGestureWrapper* relationGesture)
    {
        RelationGestureWrapper::RawGesture* rawGesture = relationGesture->rawGesture();
        gestures_.insert(rawGesture);

        Event::Trigger* relationEnterTrigger = new Event::Trigger([=](){ emit eventOccured(relationGesture, "Relation enter"); });
        Event::Trigger* relationLeaveTrigger = new Event::Trigger([=](){ emit eventOccured(relationGesture, "Relation leave"); });

        relationEnterTrigger->addSwitch(rawGesture->relationEnterEvent());
        relationLeaveTrigger->addSwitch(rawGesture->relationLeaveEvent());

        triggers_.push_back(relationEnterTrigger);
        triggers_.push_back(relationLeaveTrigger);
    }

    void GestureTestDialog::includeGesture_(MotionGestureWrapper* motionGesture)
    {
        MotionGestureWrapper::RawGesture* rawGesture = motionGesture->rawGesture();
        gestures_.insert(rawGesture);

        Event::Trigger* motionDoneTrigger = new Event::Trigger([=](){ emit eventOccured(motionGesture, "Motion done"); });
        motionDoneTrigger->addSwitch(rawGesture->motionDoneEvent());

        triggers_.push_back(motionDoneTrigger);
    }

    void GestureTestDialog::includeAllGesture_()
    {
        for(StateGestureWrapper* stateGesture : gestureModel_->stateGestures())
        {
            includeGesture_(stateGesture);
        }

        for(RelationGestureWrapper* relationGesture : gestureModel_->relationGestures())
        {
            includeGesture_(relationGesture);
        }

        for(MotionGestureWrapper* motionGesture : gestureModel_->motionGestures())
        {
            includeGesture_(motionGesture);
        }

        ui_->includeCheckBox->setChecked(true);

        control_.prepareGestures(gestures_);
    }

    void GestureTestDialog::startCapture_()
    {
        connect(this, SIGNAL(eventOccured(GestureWrapper*,QString)), this, SLOT(logEvent_(GestureWrapper*,QString)), Qt::BlockingQueuedConnection);
        connect(control_.signaler().get(), SIGNAL(objectsRecognized(Image,Image,ObjectSet)), this, SLOT(displayImage_(Image,Image,ObjectSet)), Qt::BlockingQueuedConnection);

        control_.prepareObjects(objectModel_->rawObjects());
        control_.prepareGestures(gestures_);
        control_.start();
    }

    void GestureTestDialog::stopCapture_()
    {
        disconnect(control_.signaler().get(), SIGNAL(objectsRecognized(Image,Image,ObjectSet)), this, SLOT(displayImage_(Image,Image,ObjectSet)));
        disconnect(this, SIGNAL(eventOccured(GestureWrapper*,QString)), this, SLOT(logEvent_(GestureWrapper*,QString)));
        QCoreApplication::processEvents();

        control_.stop();
    }

    void GestureTestDialog::displayImage_(GestureTestDialog::Image original, GestureTestDialog::Image segmented, GestureTestDialog::ObjectSet objects)
    {
        displayObjectsStates_();
        ui_->display->displayImage(original, objects);
    }

    void GestureTestDialog::displayObjectsStates_()
    {
        objectsStatesModel_->clear();
        objectsStatesModel_->setColumnCount(2);

        QStandardItem* rootItem = objectsStatesModel_->invisibleRootItem();
        for(ObjectWrapper* object : objectModel_->objects())
        {
            ObjectWrapper::RawObject* rawObject = object->rawObject();

            QStandardItem* objectItem = new QStandardItem(object->name());
            objectItem->setData(object->color(), Qt::DecorationRole);

            objectItem->setChild(0, 0, new QStandardItem("visibility:"));
            objectItem->setChild(1, 0, new QStandardItem("position:"));
            objectItem->setChild(2, 0, new QStandardItem("angle:"));

            bool isVisible = rawObject->isVisible();
            objectItem->setChild(0, 1, new QStandardItem(isVisible ? "visible" : "hidden"));

            if(isVisible)
            {
                objectItem->setChild(1, 1, new QStandardItem(QString("[%1; %2]").arg((int)rawObject->position().x).arg((int)rawObject->position().y)));
                objectItem->setChild(2, 1, new QStandardItem(QString(tr("%1 degrees")).arg(rawObject->angle())));
            }
            else
            {
                objectItem->setChild(1, 1, new QStandardItem("?"));
                objectItem->setChild(2, 1, new QStandardItem("?"));

                QBrush brush(Qt::gray);
                objectItem->child(1, 0)->setData(brush, Qt::ForegroundRole);
                objectItem->child(1, 1)->setData(brush, Qt::ForegroundRole);
                objectItem->child(2, 0)->setData(brush, Qt::ForegroundRole);
                objectItem->child(2, 1)->setData(brush, Qt::ForegroundRole);
            }

            rootItem->appendRow(objectItem);
        }

        ui_->objectsStates->expandAll();
    }

    void GestureTestDialog::close_()
    {
        stopCapture_();
        reset_();

        reject();
    }

    void GestureTestDialog::reset_()
    {
        gestures_.clear();
        objectsStatesModel_->clear();

        for(Event::Trigger* trigger : triggers_)
        {
            delete trigger;
        }
        triggers_.clear();

        ui_->display->reset();
        ui_->eventLog->clear();
        ui_->includeCheckBox->setChecked(false);
    }

    void GestureTestDialog::setDevice(GestureTestDialog::DeviceAdapter device)
    {
        control_.setDevice(device);
    }
} // namespace Gecon
