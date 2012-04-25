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

#include <Gecon/Event.hpp>

#include <QString>
#include <QComboBox>

#include "ObjectModel.hpp"
#include "EventWrapper.hpp"

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

        virtual Event::Trigger* toTrigger(const Events& onEvents, const Events& offEvent) const = 0;

        virtual ActionSettings* clone() const = 0;

    protected:
        virtual void addSwitches_(Event::Trigger* trigger, const Events& onEvents, const Events& offEvents) const;

    private:
        QString name_;
        QWidget* widget_;
    };

    class MouseMotionActionSettings : public ActionSettings
    {
    public:
        struct Widget : public QWidget
        {
            Widget();

            QComboBox* object;
        };

        MouseMotionActionSettings(ObjectModel* objectModel);

        void load();
        void save();
        void reset();

        Event::Trigger* toTrigger(const Events& onEvents, const Events& offEvents) const;

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

        Event::Trigger* toTrigger(const Events& onEvents, const Events& offEvents) const;

        ActionSettings* clone() const;

    private:
        int buttonIndex_;

        Widget* widget_;
    };
} // namespace Gecon

#endif // GECON_ACTIONSETTINGS_HPP
