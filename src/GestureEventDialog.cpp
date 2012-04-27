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

#include "GestureEventDialog.hpp"
#include "ui_GestureEventDialog.hpp"

#include "GestureModel.hpp"

namespace Gecon
{
    GestureEventDialog::GestureEventDialog(GestureModel *gestureModel, QWidget *parent) :
        QDialog(parent),
        gestureModel_(gestureModel),
        ui_(new Ui::GestureEventDialog)
    {
        ui_->setupUi(this);

        ui_->onEventGesture->setModel(gestureModel_);
        ui_->offEventGesture->setModel(gestureModel_);

        connect(ui_->onEventGesture, SIGNAL(currentIndexChanged(int)), this, SLOT(changeOnEvent_(int)));
        connect(ui_->offEventGesture, SIGNAL(currentIndexChanged(int)), this, SLOT(changeOffEvent_(int)));
        connect(ui_->offEventCheckbox, SIGNAL(toggled(bool)), ui_->offEventGesture, SLOT(setEnabled(bool)));
        connect(ui_->offEventCheckbox, SIGNAL(toggled(bool)), ui_->offEvent, SLOT(setEnabled(bool)));
    }
    
    GestureEventDialog::~GestureEventDialog()
    {
        delete ui_;
    }

    EventWrapper* GestureEventDialog::onEvent() const
    {
        GestureWrapper* gesture = ui_->onEventGesture->itemData(ui_->onEventGesture->currentIndex()).value<GestureWrapper*>();
        return gesture->events().at(ui_->onEvent->currentIndex());
    }

    EventWrapper* GestureEventDialog::offEvent() const
    {
        if(ui_->offEventCheckbox->isChecked())
        {
            GestureWrapper* gesture = ui_->offEventGesture->itemData(ui_->offEventGesture->currentIndex()).value<GestureWrapper*>();
            return gesture->events().at(ui_->offEvent->currentIndex());
        }
        else
        {
            return 0;
        }
    }

    int GestureEventDialog::exec()
    {
        reset_();

        return QDialog::exec();
    }

    void GestureEventDialog::changeOnEvent_(int index)
    {
        if(index == -1)
        {
            return;
        }

        GestureWrapper* gesture = ui_->onEventGesture->itemData(index).value<GestureWrapper*>();

        ui_->onEvent->clear();
        for(EventWrapper* event : gesture->events())
        {
            ui_->onEvent->addItem(event->name());
        }
    }

    void GestureEventDialog::changeOffEvent_(int index)
    {
        if(index == -1)
        {
            return;
        }

        GestureWrapper* gesture = ui_->offEventGesture->itemData(index).value<GestureWrapper*>();

        ui_->offEvent->clear();
        for(EventWrapper* event : gesture->events())
        {
            ui_->offEvent->addItem(event->name());
        }
    }

    void GestureEventDialog::reset_()
    {
        ui_->onEventGesture->setCurrentIndex(0);
        ui_->onEvent->setCurrentIndex(0);

        ui_->offEventGesture->setCurrentIndex(0);
        ui_->offEvent->setCurrentIndex(0);

        ui_->offEventCheckbox->setChecked(false);
        ui_->offEventGesture->setEnabled(false);
        ui_->offEvent->setEnabled(false);
    }
} // namespace Gecon
