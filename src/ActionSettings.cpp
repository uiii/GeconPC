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

    void ActionSettings::addSwitches_(ActionTriggerWrapper::RawActionTrigger *trigger, const ActionSettings::Events &onEvents, const ActionSettings::Events &offEvents) const
    {
        for(int i = 0; i < onEvents.size(); ++i)
        {
            qDebug("add switch");
            EventWrapper::RawEvent* rawOnEvent = onEvents.at(i)->rawEvent();
            EventWrapper::RawEvent* rawOffEvent = 0;
            if(offEvents.at(i))
            {
                rawOffEvent = offEvents.at(i)->rawEvent();
            }

            std::cout << rawOnEvent << std::endl;
            std::cout << rawOffEvent << std::endl;

            trigger->addSwitch(rawOnEvent, rawOffEvent);
        }
    }

    config_variable<int> MouseMotionActionSettings::BUFFER_SIZE = 3;

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

    ActionTriggerWrapper::RawActionTrigger* MouseMotionActionSettings::toTrigger(const Events &onEvents, const Events &offEvents) const
    {
        qDebug("make mouse move trigger");
        ActionTriggerWrapper::RawActionTrigger* trigger = new ActionTriggerWrapper::RawActionTrigger([=](){
            static QList<Point> buffer;
            qDebug("mouse move");

            ObjectWrapper::RawObject* object = object_->rawObject();
            double x = object->position().x * 1.5 - 0.25;
            double y = object->position().y * 1.5 - 0.25;

            x = x * QApplication::desktop()->screenGeometry().width();
            y = y * QApplication::desktop()->screenGeometry().height();

            buffer.push_back(Point(x,y));
            if(buffer.size() > BUFFER_SIZE)
            {
                buffer.pop_front();
            }

            Point result(0,0);
            for(Point point : buffer)
            {
                result.x += point.x;
                result.y += point.y;
            }

            result.x /= buffer.size();
            result.y /= buffer.size();

            FakeInput::Mouse::moveTo(result.x, result.y);
        }, true);

        addSwitches_(trigger, onEvents, offEvents);

        return trigger;
    }

    ActionSettings* MouseMotionActionSettings::clone() const
    {
        return new MouseMotionActionSettings(*this);
    }

    MouseButtonActionSettings::Widget::Widget():
        buttonAction(new QComboBox),
        button(new QComboBox)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(buttonAction);
        layout->addWidget(button);

        buttonAction->addItem("press", QVariant::fromValue(1));
        buttonAction->addItem("release", QVariant::fromValue(2));
        buttonAction->addItem("press & release", QVariant::fromValue(3));

        button->addItem("left button", QVariant::fromValue(FakeInput::Mouse_Left));
        button->addItem("middle button", QVariant::fromValue(FakeInput::Mouse_Middle));
        button->addItem("right button", QVariant::fromValue(FakeInput::Mouse_Right));
    }

    MouseButtonActionSettings::MouseButtonActionSettings():
        ActionSettings("mouse button", new Widget),
        widget_(dynamic_cast<Widget*>(widget()))
    {
    }

    void MouseButtonActionSettings::load()
    {
        widget_->buttonAction->setCurrentIndex(buttonActionIndex_);
        widget_->button->setCurrentIndex(buttonIndex_);
    }

    void MouseButtonActionSettings::save()
    {
        buttonActionIndex_ = widget_->buttonAction->currentIndex();
        buttonIndex_ = widget_->button->currentIndex();
    }

    void MouseButtonActionSettings::reset()
    {
        widget_->buttonAction->setCurrentIndex(0);
        widget_->button->setCurrentIndex(0);
    }

    ActionTriggerWrapper::RawActionTrigger *MouseButtonActionSettings::toTrigger(const ActionSettings::Events &onEvents, const ActionSettings::Events &offEvents) const
    {
        int buttonAction = widget_->buttonAction->itemData(buttonActionIndex_).value<int>();
        FakeInput::MouseButton button = widget_->button->itemData(buttonIndex_).value<FakeInput::MouseButton>();

        FakeInput::ActionSequence action;
        if(buttonAction & 1)
        {
            action.press(button);
        }

        if(buttonAction & 2)
        {
            action.release(button);
        }

        ActionTriggerWrapper::RawActionTrigger* trigger = new ActionTriggerWrapper::RawActionTrigger([=](){
                qDebug("mouse button");
                action.send();
        });

        addSwitches_(trigger, onEvents, offEvents);

        return trigger;
    }

    ActionSettings *MouseButtonActionSettings::clone() const
    {
        return new MouseButtonActionSettings(*this);
    }

    MouseWheelActionSettings::Widget::Widget():
        wheelMove(new QComboBox)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(wheelMove);

        wheelMove->addItem("wheel up", QVariant::fromValue(FakeInput::Mouse_Left));
        wheelMove->addItem("wheel down", QVariant::fromValue(FakeInput::Mouse_Middle));
    }

    MouseWheelActionSettings::MouseWheelActionSettings():
        ActionSettings("mouse wheel", new Widget),
        widget_(dynamic_cast<Widget*>(widget()))
    {
    }

    void MouseWheelActionSettings::load()
    {
        widget_->wheelMove->setCurrentIndex(wheelMoveIndex_);
    }

    void MouseWheelActionSettings::save()
    {
        wheelMoveIndex_ = widget_->wheelMove->currentIndex();
    }

    void MouseWheelActionSettings::reset()
    {
        widget_->wheelMove->setCurrentIndex(0);
    }

    ActionTriggerWrapper::RawActionTrigger *MouseWheelActionSettings::toTrigger(const ActionSettings::Events &onEvents, const ActionSettings::Events &offEvents) const
    {
        FakeInput::ActionSequence action;
        if(wheelMoveIndex_ == 0)
        {
            action.wheelUp();
        }
        else
        {
            action.wheelDown();
        }

        ActionTriggerWrapper::RawActionTrigger* trigger = new ActionTriggerWrapper::RawActionTrigger([=](){
                qDebug("wheel");
                action.send();
        });

        addSwitches_(trigger, onEvents, offEvents);

        return trigger;
    }

    ActionSettings *MouseWheelActionSettings::clone() const
    {
        return new MouseWheelActionSettings(*this);
    }

    KeyActionSettingsWidget::KeyActionSettingsWidget():
        QWidget(),
        keyAction(new QComboBox),
        keyLabel(new QLabel),
        grabKeyButton(new QPushButton("grab key")),
        grabbing_(false)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(keyAction);
        layout->addWidget(keyLabel);
        layout->addWidget(grabKeyButton);

        keyAction->addItem("press", QVariant::fromValue(1));
        keyAction->addItem("release", QVariant::fromValue(2));
        keyAction->addItem("press & release", QVariant::fromValue(3));

        keyLabel->setText(key.name().c_str());
        keyLabel->setAlignment(Qt::AlignCenter);

        connect(grabKeyButton, SIGNAL(clicked()), this, SLOT(grab()));
    }

    void KeyActionSettingsWidget::grab()
    {
        grabKeyboard();
        grabbing_ = true;
        keyLabel->setText("press key ...");
    }

    void KeyActionSettingsWidget::ungrab()
    {
        if(grabbing_)
        {
            grabbing_ = false;
            releaseKeyboard();
            keyLabel->setText(key.name().c_str());
        }
    }

    void KeyActionSettingsWidget::hideEvent(QHideEvent *)
    {
        ungrab();
    }

    bool KeyActionSettingsWidget::x11Event(XEvent *event)
    {
        if(grabbing_)
        {
            if(event->type == KeyPress)
            {
                key = FakeInput::Key(event);
            }
            else if(event->type == KeyRelease && grabbing_)
            {
                ungrab();
            }
        }

        return false;
    }

    KeyActionSettings::KeyActionSettings():
        ActionSettings("key", new KeyActionSettingsWidget),
        widget_(dynamic_cast<KeyActionSettingsWidget*>(widget()))
    {
    }

    void KeyActionSettings::load()
    {
        widget_->keyAction->setCurrentIndex(keyActionIndex_);

        widget_->key = key_;
        widget_->keyLabel->setText(key_.name().c_str());
    }

    void KeyActionSettings::save()
    {
        keyActionIndex_ = widget_->keyAction->currentIndex();
        key_ = widget_->key;
    }

    void KeyActionSettings::reset()
    {
        widget_->keyAction->setCurrentIndex(0);
        widget_->key = FakeInput::Key();
        widget_->keyLabel->setText(widget_->key.name().c_str());

    }

    ActionTriggerWrapper::RawActionTrigger *KeyActionSettings::toTrigger(const ActionSettings::Events &onEvents, const ActionSettings::Events &offEvents) const
    {
        int keyAction = widget_->keyAction->itemData(keyActionIndex_).value<int>();

        FakeInput::ActionSequence action;
        if(keyAction & 1)
        {
            action.press(key_);
        }

        if(keyAction & 2)
        {
            action.release(key_);
        }

        ActionTriggerWrapper::RawActionTrigger* trigger = new ActionTriggerWrapper::RawActionTrigger([=](){
                qDebug("key");
                action.send();
        });

        addSwitches_(trigger, onEvents, offEvents);

        return trigger;
    }

    ActionSettings *KeyActionSettings::clone() const
    {
        return new KeyActionSettings(*this);
    }

    CommandActionSettings::Widget::Widget():
        command(new QLineEdit)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(command);
    }

    CommandActionSettings::CommandActionSettings():
        ActionSettings("command", new Widget),
        widget_(dynamic_cast<Widget*>(widget()))
    {
    }

    void CommandActionSettings::load()
    {
        widget_->command->setText(command_);
    }

    void CommandActionSettings::save()
    {
        command_ = widget_->command->text();
    }

    void CommandActionSettings::reset()
    {
        widget_->command->setText("");
    }

    ActionTriggerWrapper::RawActionTrigger *CommandActionSettings::toTrigger(const ActionSettings::Events &onEvents, const ActionSettings::Events &offEvents) const
    {
        ActionTriggerWrapper::RawActionTrigger* trigger = new ActionTriggerWrapper::RawActionTrigger([=](){
                qDebug("command");
                qDebug(command_.toAscii().data());
                FakeInput::ActionSequence().runCommand(command_.toAscii().data()).send();
        });

        addSwitches_(trigger, onEvents, offEvents);

        return trigger;
    }

    ActionSettings *CommandActionSettings::clone() const
    {
        return new CommandActionSettings(*this);
    }
} // namespace Gecon
