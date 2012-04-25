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
#include <FakeInput/actions/actionsequence.hpp>

#include <iostream>

Q_DECLARE_METATYPE(FakeInput::MouseButton)

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

    void ActionSettings::addSwitches_(Event::Trigger *trigger, const ActionSettings::Events &onEvents, const ActionSettings::Events &offEvents) const
    {
        for(int i = 0; i < onEvents.size(); ++i)
        {
            qDebug("add switch");
            Event* rawOnEvent = onEvents.at(i)->rawEvent();
            Event* rawOffEvent = 0;
            if(offEvents.at(i))
            {
                rawOffEvent = offEvents.at(i)->rawEvent();
            }

            std::cout << rawOnEvent << std::endl;
            std::cout << rawOffEvent << std::endl;

            trigger->addSwitch(rawOnEvent, rawOffEvent);
        }
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
            qDebug("mouse move");
            FakeInput::Mouse::moveTo(object.position().x, object.position().y);
        }, object_->rawObject());

        addSwitches_(trigger, onEvents, offEvents);

        return trigger;
    }

    ActionSettings* MouseMotionActionSettings::clone() const
    {
        return new MouseMotionActionSettings(*this);
    }

    MouseButtonActionSettings::Widget::Widget():
        button(new QComboBox)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(button);

        button->addItem("Left button", QVariant::fromValue(FakeInput::Mouse_Left));
        button->addItem("Middle button", QVariant::fromValue(FakeInput::Mouse_Middle));
        button->addItem("Right button", QVariant::fromValue(FakeInput::Mouse_Right));
    }

    MouseButtonActionSettings::MouseButtonActionSettings():
        ActionSettings("mouse button", new Widget),
        widget_(dynamic_cast<Widget*>(widget()))
    {
    }

    void MouseButtonActionSettings::load()
    {
        widget_->button->setCurrentIndex(buttonIndex_);
    }

    void MouseButtonActionSettings::save()
    {
        buttonIndex_ = widget_->button->currentIndex();
    }

    void MouseButtonActionSettings::reset()
    {
        widget_->button->setCurrentIndex(0);
    }

    Event::Trigger *MouseButtonActionSettings::toTrigger(const ActionSettings::Events &onEvents, const ActionSettings::Events &offEvents) const
    {
        FakeInput::MouseButton button = widget_->button->itemData(buttonIndex_).value<FakeInput::MouseButton>();
        Event::Trigger* trigger = new Event::Trigger([=](){
                qDebug("click");
                FakeInput::ActionSequence().press(button).release(button).send();
        });

        addSwitches_(trigger, onEvents, offEvents);

        return trigger;
    }

    ActionSettings *MouseButtonActionSettings::clone() const
    {
        return new MouseButtonActionSettings(*this);
    }
} // namespace Gecon
