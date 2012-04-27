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

#include "TestDialog.hpp"
#include "ui_TestDialog.hpp"

#include <QtConcurrentRun>
#include <QScrollBar>

#include "GestureModel.hpp"
#include "ObjectModel.hpp"
#include "ActionTriggerModel.hpp"

#include <FakeInput/mouse.hpp>

namespace Gecon
{
    TestDialog::TestDialog(ObjectModel* objectModel, GestureModel* gestureModel, ActionTriggerModel* actionTriggerModel, QWidget *parent) :
        QDialog(parent),
        objectModel_(objectModel),
        gestureModel_(gestureModel),
        actionTriggerModel_(actionTriggerModel),
        objectsStatesModel_(new QStandardItemModel(this)),
        ui_(new Ui::TestDialog)
    {
        ui_->setupUi(this);

        ui_->objectsStates->setModel(objectsStatesModel_);
    }
    
    TestDialog::~TestDialog()
    {
        delete ui_;
    }

    void TestDialog::testActionTrigger(ActionTriggerWrapper* trigger)
    {
        includeActionTrigger_(trigger, true);
    }

    int TestDialog::exec()
    {
        return QDialog::Rejected;
    }

    void TestDialog::logGestureEvent_(GestureWrapper *gesture, const QString& eventDescription, bool isTestedGesture)
    {
        bool shallLog = ui_->showGestureEvents->isChecked() || isTestedGesture;
        if(! shallLog)
        {
            return;
        }

        QTextCursor cursor = ui_->log->textCursor();

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

        ui_->log->verticalScrollBar()->setValue(ui_->log->verticalScrollBar()->maximum());
    }

    void TestDialog::logAction_(const QString &triggerName, bool isTestedTrigger)
    {
        qDebug("log action");
        bool shallLog = ui_->showActions->isChecked() || isTestedTrigger;
        if(! shallLog)
        {
            return;
        }

        QTextCursor cursor = ui_->log->textCursor();

        if(! cursor.atStart())
        {
            cursor.insertHtml("<br />");
        }

        cursor.insertHtml(tr("ActionTrigger: <strong>%1</strong>").arg(triggerName));

        ui_->log->verticalScrollBar()->setValue(ui_->log->verticalScrollBar()->maximum());

    }

    void TestDialog::includeGesture_(StateGestureWrapper* stateGesture, bool isTestedGesture)
    {
        StateGestureWrapper::RawGesture* rawGesture = stateGesture->rawGesture();
        gestures_.insert(rawGesture);

        ActionTrigger* stateEnterTrigger = new ActionTrigger([=](){ emit gestureEventOccured(stateGesture, "State enter", isTestedGesture); });
        ActionTrigger* stateLeaveTrigger = new ActionTrigger([=](){ emit gestureEventOccured(stateGesture, "State leave", isTestedGesture); });

        stateEnterTrigger->addSwitch(rawGesture->stateEnterEvent());
        stateLeaveTrigger->addSwitch(rawGesture->stateLeaveEvent());

        triggers_.insert(stateEnterTrigger);
        triggers_.insert(stateLeaveTrigger);
    }

    void TestDialog::includeGesture_(RelationGestureWrapper* relationGesture, bool isTestedGesture)
    {
        RelationGestureWrapper::RawGesture* rawGesture = relationGesture->rawGesture();
        gestures_.insert(rawGesture);

        ActionTrigger* relationEnterTrigger = new ActionTrigger([=](){ emit gestureEventOccured(relationGesture, "Relation enter", isTestedGesture); });
        ActionTrigger* relationLeaveTrigger = new ActionTrigger([=](){ emit gestureEventOccured(relationGesture, "Relation leave", isTestedGesture); });

        relationEnterTrigger->addSwitch(rawGesture->relationEnterEvent());
        relationLeaveTrigger->addSwitch(rawGesture->relationLeaveEvent());

        triggers_.insert(relationEnterTrigger);
        triggers_.insert(relationLeaveTrigger);
    }

    void TestDialog::includeGesture_(MotionGestureWrapper* motionGesture, bool isTestedGesture)
    {
        MotionGestureWrapper::RawGesture* rawGesture = motionGesture->rawGesture();
        gestures_.insert(rawGesture);

        ActionTrigger* motionDoneTrigger = new ActionTrigger([=](){ emit gestureEventOccured(motionGesture, "Motion done", isTestedGesture); });
        motionDoneTrigger->addSwitch(rawGesture->motionDoneEvent());

        triggers_.insert(motionDoneTrigger);
    }

    void TestDialog::includeAllGestures_()
    {
        for(StateGestureWrapper* stateGesture : gestureModel_->stateGestures())
        {
            includeGesture_(stateGesture, false);
        }

        for(RelationGestureWrapper* relationGesture : gestureModel_->relationGestures())
        {
            includeGesture_(relationGesture, false);
        }

        for(MotionGestureWrapper* motionGesture : gestureModel_->motionGestures())
        {
            includeGesture_(motionGesture, false);
        }
    }

    void TestDialog::includeActionTrigger_(ActionTriggerWrapper* trigger, bool isTestedTrigger)
    {
        ActionTrigger* testTrigger = new ActionTrigger([=](){ qDebug("emit action"); emit actionTriggered(trigger->name(), isTestedTrigger); });

        for(int i = 0; i < trigger->onEvents().size(); ++i)
        {
            EventWrapper::RawEvent* rawOnEvent = trigger->onEvents().at(i)->rawEvent();
            EventWrapper::RawEvent* rawOffEvent = 0;
            if(trigger->offEvents().at(i))
            {
                rawOffEvent = trigger->offEvents().at(i)->rawEvent();
            }

            testTrigger->addSwitch(rawOnEvent, rawOffEvent);
        }

        qDebug("insert trigger");
        triggers_.insert(testTrigger);
    }

    void TestDialog::includeAllActionTriggers_()
    {
        for(ActionTriggerWrapper* trigger : actionTriggerModel_->triggers())
        {
            includeActionTrigger_(trigger, false);
        }
    }

    void TestDialog::startCapture_()
    {
        connect(this, SIGNAL(gestureEventOccured(GestureWrapper*,QString,bool)), this, SLOT(logGestureEvent_(GestureWrapper*,QString,bool)), Qt::BlockingQueuedConnection);
        connect(this, SIGNAL(actionTriggered(QString,bool)), this, SLOT(logAction_(QString,bool)), Qt::BlockingQueuedConnection);
        connect(control_.signaler().get(), SIGNAL(objectsRecognized(Image,Image,ObjectSet)), this, SLOT(displayImage_(Image,Image,ObjectSet)), Qt::BlockingQueuedConnection);

        control_.prepareObjects(objectModel_->rawObjects());
        control_.prepareGestures(gestures_);
        control_.prepareActionTriggers(triggers_);

        control_.start();
    }

    void TestDialog::stopCapture_()
    {
        disconnect(control_.signaler().get(), SIGNAL(objectsRecognized(Image,Image,ObjectSet)), this, SLOT(displayImage_(Image,Image,ObjectSet)));
        disconnect(this, SIGNAL(actionTriggered(QString,bool)), this, SLOT(logAction_(QString,bool)));
        disconnect(this, SIGNAL(gestureEventOccured(GestureWrapper*,QString,bool)), this, SLOT(logGestureEvent_(GestureWrapper*,QString,bool)));
        QCoreApplication::processEvents();

        control_.stop();
    }

    void TestDialog::displayImage_(TestDialog::Image original, TestDialog::Image segmented, TestDialog::ObjectSet objects)
    {
        displayObjectsStates_();
        ui_->display->displayImage(original, objects);
    }

    void TestDialog::displayObjectsStates_()
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

    void TestDialog::close_()
    {
        stopCapture_();
        reset_();
    }

    void TestDialog::reset_()
    {
        ui_->showGestureEvents->setChecked(false);
        ui_->showActions->setChecked(false);

        gestures_.clear();
        for(ActionTrigger* trigger : triggers_)
        {
            delete trigger;
        }
        triggers_.clear();

        objectsStatesModel_->clear();

        ui_->display->reset();
        ui_->log->clear();
    }

    void TestDialog::reject()
    {
        close_();
        QDialog::reject();
    }

    void TestDialog::accept()
    {
        close_();
        QDialog::accept();
    }

    void TestDialog::setDevice(TestDialog::DeviceAdapter device)
    {
        control_.setDevice(device);
    }

    void TestDialog::testAll()
    {
        includeAllGestures_();
        includeAllActionTriggers_();

        ui_->showGestureEvents->setChecked(true);
        ui_->showActions->setChecked(true);

        startCapture_();

        QDialog::exec();
    }

    void TestDialog::closeEvent(QCloseEvent *)
    {
        close_();
    }
} // namespace Gecon
