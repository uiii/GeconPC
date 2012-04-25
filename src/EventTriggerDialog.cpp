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

#include "EventTriggerDialog.hpp"
#include "ui_EventTriggerDialog.hpp"

#include <QStringListModel>
#include <QMessageBox>

#include "EventTriggerModel.hpp"
#include "GestureModel.hpp"
#include "ObjectModel.hpp"

#include "GestureEventDialog.hpp"
#include "GestureWrapper.hpp"
#include "ActionSettings.hpp"

namespace Gecon
{
    EventTriggerDialog::EventTriggerDialog(EventTriggerModel* eventTriggerModel, GestureModel* gestureModel, ObjectModel* objectModel, QWidget *parent) :
        QDialog(parent),
        eventTriggerModel_(eventTriggerModel),
        gestureModel_(gestureModel),
        objectModel_(objectModel),
        eventDialog_(new GestureEventDialog(gestureModel, this)),
        editedTrigger_(0),
        currentAction_(0),
        ui_(new Ui::EventTriggerDialog)
    {
        ui_->setupUi(this);

        actions_.push_back(new MouseMotionActionSettings(objectModel));
        actions_.push_back(new MouseButtonActionSettings());

        for(ActionSettings* action : actions_)
        {
            ui_->action->addItem(action->name());
            ui_->actionSettingsLayout->addWidget(action->widget());

            action->widget()->setVisible(false);
        }

        currentAction_ = actions_.front();
        currentAction_->widget()->setVisible(true);

        connect(ui_->action, SIGNAL(currentIndexChanged(int)), this, SLOT(setActionSettings_(int)));
        connect(ui_->addSwitchButton, SIGNAL(clicked()), this, SLOT(addSwitch_()));
        connect(ui_->removeSwitchButton, SIGNAL(clicked()), this, SLOT(removeSwitch_()));
        connect(ui_->deleteButton, SIGNAL(clicked()), this, SLOT(deleteTrigger_()));
    }

    EventTriggerDialog::~EventTriggerDialog()
    {
        delete ui_;
    }

    int EventTriggerDialog::newTrigger()
    {
        if(! gestureModel_->size())
        {
            QMessageBox::critical(parentWidget(), "Gesture error", "No gestures available", QMessageBox::Ok);

            return QDialog::Rejected;
        }

        reset_();

        disconnect(ui_->buttonBox, SIGNAL(accepted()), this, 0);
        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(addTrigger_()));

        return QDialog::exec();
    }

    int EventTriggerDialog::editTrigger(EventTriggerWrapper* trigger)
    {
        reset_();

        editedTrigger_ = trigger;

        for(int i = 0; i < editedTrigger_->onEvents().size(); ++i)
        {
            EventWrapper* onEvent = editedTrigger_->onEvents().at(i);
            EventWrapper* offEvent = editedTrigger_->offEvents().at(i);

            onEvents_.push_back(onEvent);
            offEvents_.push_back(offEvent);

            QString eventDescription = QString("ON: %1 [%2]").arg(onEvent->gesture()->name()).arg(onEvent->name());
            if(offEvent)
            {
                eventDescription += QString("\tOFF: %1 [%2]").arg(offEvent->gesture()->name()).arg(offEvent->name());
            }

            switchDescriptions_.push_back(eventDescription);
        }

        ui_->switchList->setModel(new QStringListModel(switchDescriptions_));

        QList<ActionSettings*>::iterator it = std::find_if(actions_.begin(), actions_.end(),
                [&](ActionSettings* action){
                    return action->widget() == editedTrigger_->action()->widget();
                }
        );

        ui_->action->setCurrentIndex(it - actions_.begin());
        ui_->actionName->setText(editedTrigger_->name());
        ui_->deleteButton->setVisible(true);

        editedTrigger_->action()->load();

        disconnect(ui_->buttonBox, SIGNAL(accepted()), this, 0);
        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(updateTrigger_()));

        return QDialog::exec();
    }

    int EventTriggerDialog::exec()
    {
        return QDialog::Rejected;
    }

    void EventTriggerDialog::addTrigger_()
    {
        currentAction_->save();

        try
        {
            eventTriggerModel_->addTrigger(
                ui_->actionName->text(),
                onEvents_,
                offEvents_,
                currentAction_
            );

            accept();
        }
        catch(...)
        {
            // TODO
        }
    }


    void EventTriggerDialog::updateTrigger_()
    {
        EventTriggerWrapper triggerBackup(*editedTrigger_);

        currentAction_->save();

        try
        {
            QModelIndex index = eventTriggerModel_->index(editedTrigger_);
            eventTriggerModel_->removeTrigger(index);
            eventTriggerModel_->addTrigger(
                ui_->actionName->text(),
                onEvents_,
                offEvents_,
                currentAction_
            );

            accept();
        }
        catch(...)
        {
            eventTriggerModel_->addTrigger(triggerBackup.name(), triggerBackup.onEvents(), triggerBackup.offEvents(), triggerBackup.action());

            // TODO
        }
    }

    void EventTriggerDialog::deleteTrigger_()
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, tr("Delete question"),
            tr("Do you really want to delete event trigger '%1'").arg(editedTrigger_->name()), QMessageBox::Ok | QMessageBox::Cancel);

        if(button == QMessageBox::Ok)
        {
            QModelIndex index = eventTriggerModel_->index(editedTrigger_);
            eventTriggerModel_->removeTrigger(index);

            accept();
        }
    }

    void EventTriggerDialog::addSwitch_()
    {
        if(eventDialog_->exec() == QDialog::Accepted)
        {
            EventWrapper* onEvent = eventDialog_->onEvent();
            EventWrapper* offEvent = eventDialog_->offEvent();

            onEvents_.push_back(onEvent);
            offEvents_.push_back(offEvent);

            QString eventDescription = QString("ON: %1 [%2]").arg(onEvent->gesture()->name()).arg(onEvent->name());
            if(offEvent)
            {
                eventDescription += QString("\tOFF: %1 [%2]").arg(offEvent->gesture()->name()).arg(offEvent->name());
            }

            switchDescriptions_.push_back(eventDescription);
        }

        ui_->switchList->setModel(new QStringListModel(switchDescriptions_, this));
    }

    void EventTriggerDialog::removeSwitch_()
    {
        QModelIndexList indexes = ui_->switchList->selectionModel()->selectedIndexes();
        if(indexes.size() > 0)
        {
            onEvents_.removeAt(indexes.front().row());
            offEvents_.removeAt(indexes.front().row());
            switchDescriptions_.removeAt(indexes.front().row());

            ui_->switchList->setModel(new QStringListModel(switchDescriptions_, this));
        }
    }

    void EventTriggerDialog::setActionSettings_(int index)
    {
        currentAction_->widget()->setVisible(false);

        currentAction_ = actions_.at(index);
        currentAction_->widget()->setVisible(true);
    }

    void EventTriggerDialog::reset_()
    {
        onEvents_.clear();
        offEvents_.clear();
        switchDescriptions_.clear();
        ui_->switchList->setModel(new QStringListModel);
        ui_->action->setCurrentIndex(0);
        ui_->actionName->setText(QString());
    }
}
