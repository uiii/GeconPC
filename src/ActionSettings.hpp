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

#ifndef GECON_ACTIONSETTINGS_HPP
#define GECON_ACTIONSETTINGS_HPP

#include <QString>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>

#include "ObjectModel.hpp"
#include "EventWrapper.hpp"
#include "ActionTriggerWrapper.hpp"

#include <FakeInput/key.hpp>
#include <FakeInput/mouse.hpp>
#include <FakeInput/actions/actionsequence.hpp>

namespace Gecon
{
    class ActionSettings
    {
    public:
        typedef QList<EventWrapper*> Events;

        ActionSettings(const QString& name, QWidget* widget);
        virtual ~ActionSettings();

        virtual const QString& name() const;
        virtual QWidget* widget() const;

        virtual void load() = 0;
        virtual void save() = 0;
        virtual void reset() = 0;

        virtual ActionTriggerWrapper::RawActionTrigger* toTrigger(const Events& onEvents, const Events& offEvent) const = 0;

        virtual ActionSettings* clone() const = 0;

    protected:
        virtual void addSwitches_(ActionTriggerWrapper::RawActionTrigger* trigger, const Events& onEvents, const Events& offEvents) const;

    private:
        QString name_;
        QWidget* widget_;
    };

    class MouseMotionActionSettings : public ActionSettings
    {
    public:
        static config_variable<int> BUFFER_SIZE;

        struct Widget : public QWidget
        {
            Widget();

            QComboBox* object;
        };

        MouseMotionActionSettings(ObjectModel* objectModel);

        void load();
        void save();
        void reset();

        ActionTriggerWrapper::RawActionTrigger* toTrigger(const Events& onEvents, const Events& offEvents) const;

        ActionSettings* clone() const;

    private:
        ObjectWrapper* object_;
        ObjectModel* objectModel_;

        Widget* widget_;
    };

    class MouseButtonActionSettings : public ActionSettings
    {
    public:
        struct Widget : public QWidget
        {
            Widget();

            QComboBox* button;
        };

        MouseButtonActionSettings();

        void load();
        void save();
        void reset();

        ActionTriggerWrapper::RawActionTrigger* toTrigger(const Events& onEvents, const Events& offEvents) const;

        ActionSettings* clone() const;

    private:
        int buttonIndex_;

        Widget* widget_;
    };

    class MouseWheelActionSettings : public ActionSettings
    {
    public:
        struct Widget : public QWidget
        {
            Widget();

            QComboBox* wheelMove;
        };

        MouseWheelActionSettings();

        void load();
        void save();
        void reset();

        ActionTriggerWrapper::RawActionTrigger* toTrigger(const Events& onEvents, const Events& offEvents) const;

        ActionSettings* clone() const;

    private:
        int wheelMoveIndex_;

        Widget* widget_;
    };

    class KeyActionSettingsWidget : public QWidget
    {
        Q_OBJECT

    public:
        KeyActionSettingsWidget();

        QComboBox* keyAction;
        QLabel* keyLabel;
        FakeInput::Key key;
        QPushButton* grabKeyButton;

    protected slots:
        void grab();
        void ungrab();

    protected:
        void hideEvent(QHideEvent *);
        bool x11Event(XEvent* event);

    private:
        bool grabbing_;
    };

    class KeyActionSettings : public ActionSettings
    {
    public:
        KeyActionSettings();

        void load();
        void save();
        void reset();

        ActionTriggerWrapper::RawActionTrigger* toTrigger(const Events& onEvents, const Events& offEvents) const;

        ActionSettings* clone() const;

    private:
        int keyActionIndex_;
        FakeInput::Key key_;

        KeyActionSettingsWidget* widget_;
    };

    class CommandActionSettings : public ActionSettings
    {
    public:
        struct Widget : public QWidget
        {
            Widget();

            QLineEdit* command;
        };

        CommandActionSettings();

        void load();
        void save();
        void reset();

        ActionTriggerWrapper::RawActionTrigger* toTrigger(const Events& onEvents, const Events& offEvents) const;

        ActionSettings* clone() const;

    private:
        QString command_;

        Widget* widget_;
    };
} // namespace Gecon

#endif // GECON_ACTIONSETTINGS_HPP
