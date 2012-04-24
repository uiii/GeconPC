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

#ifndef GECON_EVENTTRIGGERWRAPPER_HPP
#define GECON_EVENTTRIGGERWRAPPER_HPP

#include <QList>
#include <QString>
#include <QMetaType>

#include <Gecon/Event.hpp>

namespace Gecon
{
    class EventWrapper;
    class ActionSettings;

    class EventTriggerWrapper
    {
    public:
        typedef QList<EventWrapper*> Events;

        EventTriggerWrapper(const QString& name, const Events& onEvents, const Events& offEvents, ActionSettings* action);
        EventTriggerWrapper(const EventTriggerWrapper& another);
        virtual ~EventTriggerWrapper();

        const QString& name() const;

        const Events& onEvents() const;
        const Events& offEvents() const;

        ActionSettings* action() const;

        Event::Trigger* rawTrigger() const;

    private:
        QString name_;

        Events onEvents_;
        Events offEvents_;

        ActionSettings* action_;

        Event::Trigger* rawTrigger_;
    };
} // namespace Gecon

Q_DECLARE_METATYPE(Gecon::EventTriggerWrapper*)

#endif // GECON_EVENTTRIGGERWRAPPER_HPP
