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

#include "ActionSettings.hpp"

#include <QHBoxLayout>

#include <FakeInput/mouse.hpp>

namespace Gecon
{
    ActionSettings::ActionSettings(const QString &name, QWidget *widget):
        name_(name),
        widget_(widget)
    {
    }

    ActionSettings::~ActionSettings()
    {
    }

    const QString &ActionSettings::name() const
    {
        return name_;
    }

    QWidget* ActionSettings::widget() const
    {
        return widget_;
    }

    MouseMotionActionSettings::Widget::Widget():
        object(new QComboBox(this))
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(object);
    }

    MouseMotionActionSettings::MouseMotionActionSettings(ObjectModel *objectModel):
        ActionSettings("mouse motion", new Widget),
        objectModel_(objectModel),
        widget_(dynamic_cast<Widget*>(widget()))
    {
        widget_->object->setModel(objectModel_);
    }

    void MouseMotionActionSettings::load()
    {
        widget_->object->setCurrentIndex(objectModel_->index(object_).row());
    }

    void MouseMotionActionSettings::save()
    {
        object_ = widget_->object->itemData(widget_->object->currentIndex()).value<ObjectWrapper*>();
    }

    void MouseMotionActionSettings::reset()
    {
        widget_->object->setCurrentIndex(0);
    }

    Event::Trigger *MouseMotionActionSettings::toTrigger(const Events &onEvents, const Events &offEvents) const
    {
        Event::Trigger* trigger = new Event::Trigger([](const ObjectWrapper::RawObject& object){
            FakeInput::Mouse::moveTo(object.position().x, object.position().y);
        }, object_->rawObject());

        for(int i = 0; i < onEvents.size(); ++i)
        {
            Event* rawOnEvent = onEvents.at(i)->rawEvent();
            Event* rawOffEvent = 0;
            if(offEvents.at(i))
            {
                rawOffEvent = offEvents.at(i)->rawEvent();
            }

            trigger->addSwitch(rawOnEvent, rawOffEvent);
        }

    return trigger;
    }

    ActionSettings* MouseMotionActionSettings::clone() const
    {
        return new MouseMotionActionSettings(*this);
    }
} // namespace Gecon
