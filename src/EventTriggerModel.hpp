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

#ifndef GECON_EVENTTRIGGERMODEL_HPP
#define GECON_EVENTTRIGGERMODEL_HPP

#include <QAbstractListModel>

#include <Gecon/Event.hpp>

#include "EventTriggerWrapper.hpp"

namespace Gecon
{
    class EventTriggerModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        typedef QList<EventTriggerWrapper*> EventTriggerWrapperList;
        typedef QList<Event::Trigger*> RawEventTriggerList;

        explicit EventTriggerModel();
        virtual ~EventTriggerModel();
        
        int rowCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;

        QModelIndex index(EventTriggerWrapper* trigger) const;

        int size() const;

        void addTrigger(const QString& name, const EventTriggerWrapper::Events& onEvents, const EventTriggerWrapper::Events& offEvents, ActionSettings* action);
        void removeTrigger(const QModelIndex& index);

        const EventTriggerWrapperList& triggers() const;
        const RawEventTriggerList& rawTriggers() const;

    private:
        EventTriggerWrapperList triggers_;
        RawEventTriggerList rawTriggers_;
    };
} // namespace Gecon

#endif // GECON_EVENTTRIGGERMODEL_HPP
