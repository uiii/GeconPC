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
        DialogBase(parent),
        objectModel_(objectModel),
        gestureModel_(gestureModel),
        actionTriggerModel_(actionTriggerModel),
        objectsStatesModel_(new QStandardItemModel(this)),
        ui_(new Ui::TestDialog)
    {
        ui_->setupUi(this);

        ui_->objectsStates->setModel(objectsStatesModel_);

        ui_->performActions->setChecked(false);
        performActions(false);

        connect(ui_->performActions, SIGNAL(toggled(bool)), this, SLOT(performActions(bool)));
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
        for(ObjectWrapper* object : objects)
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
        ControlInfo::StateGesture* rawGesture = stateGesture->rawGesture();
        gestures_.insert(rawGesture);

        ControlInfo::ActionTrigger* stateEnterTrigger = new ControlInfo::ActionTrigger([=](){ emit gestureEventOccured(stateGesture, "State enter", isTestedGesture); });
        ControlInfo::ActionTrigger* stateLeaveTrigger = new ControlInfo::ActionTrigger([=](){ emit gestureEventOccured(stateGesture, "State leave", isTestedGesture); });

        stateEnterTrigger->addSwitch(rawGesture->stateEnterEvent());
        stateLeaveTrigger->addSwitch(rawGesture->stateLeaveEvent());

        logTriggers_.insert(stateEnterTrigger);
        logTriggers_.insert(stateLeaveTrigger);
    }

    void TestDialog::includeGesture_(RelationGestureWrapper* relationGesture, bool isTestedGesture)
    {
        ControlInfo::RelationGesture* rawGesture = relationGesture->rawGesture();
        gestures_.insert(rawGesture);

        ControlInfo::ActionTrigger* relationEnterTrigger = new ControlInfo::ActionTrigger([=](){ emit gestureEventOccured(relationGesture, "Relation enter", isTestedGesture); });
        ControlInfo::ActionTrigger* relationLeaveTrigger = new ControlInfo::ActionTrigger([=](){ emit gestureEventOccured(relationGesture, "Relation leave", isTestedGesture); });

        relationEnterTrigger->addSwitch(rawGesture->relationEnterEvent());
        relationLeaveTrigger->addSwitch(rawGesture->relationLeaveEvent());

        logTriggers_.insert(relationEnterTrigger);
        logTriggers_.insert(relationLeaveTrigger);
    }

    void TestDialog::includeGesture_(MotionGestureWrapper* motionGesture, bool isTestedGesture)
    {
        ControlInfo::MotionGesture* rawGesture = motionGesture->rawGesture();
        gestures_.insert(rawGesture);

        ControlInfo::ActionTrigger* motionDoneTrigger = new ControlInfo::ActionTrigger([=](){ emit gestureEventOccured(motionGesture, "Motion done", isTestedGesture); });
        motionDoneTrigger->addSwitch(rawGesture->motionDoneEvent());

        logTriggers_.insert(motionDoneTrigger);
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
        ControlInfo::ActionTrigger* testLogTrigger = new ControlInfo::ActionTrigger([=](){ qDebug("emit action"); emit actionTriggered(trigger->name(), isTestedTrigger); });

        for(int i = 0; i < trigger->onEvents().size(); ++i)
        {
            EventWrapper::RawEvent* rawOnEvent = trigger->onEvents().at(i)->rawEvent();
            EventWrapper::RawEvent* rawOffEvent = 0;
            if(trigger->offEvents().at(i))
            {
                rawOffEvent = trigger->offEvents().at(i)->rawEvent();
            }

            testLogTrigger->addSwitch(rawOnEvent, rawOffEvent);
        }

        std::cout << "insert trigger: " << testLogTrigger << std::endl;
        actionTriggers_.insert(trigger->rawTrigger());
        logTriggers_.insert(testLogTrigger);
    }

    void TestDialog::includeAllActionTriggers_()
    {
        for(ActionTriggerWrapper* trigger : actionTriggerModel_->triggers())
        {
            std::cout << "include: " << trigger << std::endl;
            includeActionTrigger_(trigger, false);
        }
    }

    void TestDialog::startCapture_()
    {
        connect(this, SIGNAL(gestureEventOccured(GestureWrapper*,QString,bool)), this, SLOT(logGestureEvent_(GestureWrapper*,QString,bool)), Qt::BlockingQueuedConnection);
        connect(this, SIGNAL(actionTriggered(QString,bool)), this, SLOT(logAction_(QString,bool)), Qt::BlockingQueuedConnection);
        connect(control_.objectPolicySignaler(), SIGNAL(objectsRecognized(Image,Image,Objects)), this, SLOT(displayImage_(Image,Image,Objects)), Qt::BlockingQueuedConnection);

        control_.prepareObjects(objectModel_->rawObjects());
        control_.prepareGestures(gestures_);
        control_.prepareActionTriggers(logTriggers_);

        control_.start();
    }

    void TestDialog::stopCapture_()
    {
        disconnect(control_.objectPolicySignaler(), SIGNAL(objectsRecognized(Image,Image,Objects)), this, SLOT(displayImage_(Image,Image,Objects)));
        disconnect(this, SIGNAL(actionTriggered(QString,bool)), this, SLOT(logAction_(QString,bool)));
        disconnect(this, SIGNAL(gestureEventOccured(GestureWrapper*,QString,bool)), this, SLOT(logGestureEvent_(GestureWrapper*,QString,bool)));
        QCoreApplication::processEvents();

        control_.stop();
    }

    void TestDialog::displayImage_(const Image &original, const Image &segmented, const ControlInfo::Objects& objects)
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
            objectItem->setChild(3, 0, new QStandardItem("area:"));
            objectItem->setChild(4, 0, new QStandardItem("aspect ratio:"));

            bool isVisible = rawObject->isVisible();
            objectItem->setChild(0, 1, new QStandardItem(isVisible ? "visible" : "hidden"));

            if(isVisible)
            {
                objectItem->setChild(1, 1, new QStandardItem(QString("[%1 \%; %2 \%]").arg(int(rawObject->position().x * 100)).arg(int(rawObject->position().y * 100))));
                objectItem->setChild(2, 1, new QStandardItem(QString(tr("%1 degrees")).arg(rawObject->angle())));
                objectItem->setChild(3, 1, new QStandardItem(QString(tr("1 / %1")).arg(rawObject->areaSize().denominator)));
                objectItem->setChild(4, 1, new QStandardItem(QString(tr("%1")).arg(QString::number(rawObject->aspectRatio(), 'f', 2))));
            }
            else
            {
                objectItem->setChild(1, 1, new QStandardItem("?"));
                objectItem->setChild(2, 1, new QStandardItem("?"));
                objectItem->setChild(3, 1, new QStandardItem("?"));
                objectItem->setChild(4, 1, new QStandardItem("?"));

                QBrush brush(Qt::gray);
                objectItem->child(1, 0)->setData(brush, Qt::ForegroundRole);
                objectItem->child(1, 1)->setData(brush, Qt::ForegroundRole);
                objectItem->child(2, 0)->setData(brush, Qt::ForegroundRole);
                objectItem->child(2, 1)->setData(brush, Qt::ForegroundRole);
                objectItem->child(3, 0)->setData(brush, Qt::ForegroundRole);
                objectItem->child(3, 1)->setData(brush, Qt::ForegroundRole);
                objectItem->child(4, 0)->setData(brush, Qt::ForegroundRole);
                objectItem->child(4, 1)->setData(brush, Qt::ForegroundRole);
            }

            rootItem->appendRow(objectItem);
        }

        ui_->objectsStates->expandAll();
    }

    void TestDialog::performActions(bool perform)
    {
        std::cout << "perform: " << perform << std::endl;

        ControlInfo::ActionTriggers triggers;
        triggers.insert(logTriggers_.begin(), logTriggers_.end());

        if(perform)
        {
            triggers.insert(actionTriggers_.begin(), actionTriggers_.end());
        }

        control_.prepareActionTriggers(triggers);

        QtConcurrent::run(&control_, &ControlInfo::Control::restart);
    }

    void TestDialog::beforeClose_()
    {
        stopCapture_();
        reset_();
    }

    void TestDialog::reset_()
    {
        ui_->showGestureEvents->setChecked(false);
        ui_->showActions->setChecked(false);

        gestures_.clear();
        for(ControlInfo::ActionTrigger* trigger : logTriggers_)
        {
            delete trigger;
        }
        logTriggers_.clear();
        actionTriggers_.clear();

        objectsStatesModel_->clear();

        ui_->display->reset();
        ui_->log->clear();
    }

    void TestDialog::setDevice(ControlInfo::DeviceAdapter device)
    {
        control_.setDevice(device);
    }

    void TestDialog::testAll()
    {
        includeAllGestures_();
        includeAllActionTriggers_();

        ui_->showGestureEvents->setChecked(true);
        ui_->showActions->setChecked(true);
        ui_->performActions->setChecked(false);

        startCapture_();

        QDialog::exec();
    }
} // namespace Gecon
