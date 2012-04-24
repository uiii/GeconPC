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

#include "EventTriggerModel.hpp"

namespace Gecon
{
    EventTriggerModel::EventTriggerModel()
    {
    }

    EventTriggerModel::~EventTriggerModel()
    {
        for(EventTriggerWrapper* trigger : triggers_)
        {
            delete trigger;
        }
    }

    int EventTriggerModel::rowCount(const QModelIndex &parent) const
    {
        return size();
    }

    QVariant EventTriggerModel::data(const QModelIndex &index, int role) const
    {
        if(! index.isValid() || index.row() >= size())
        {
            return QVariant();
        }

        EventTriggerWrapper* trigger = triggers_.at(index.row());

        if(role == Qt::DisplayRole)
        {
            return trigger->name();
        }
        if(role == Qt::UserRole)
        {
            return QVariant::fromValue(trigger);
        }
        else
        {
            return QVariant();
        }
    }

    QModelIndex EventTriggerModel::index(EventTriggerWrapper* trigger) const
    {
        return createIndex(triggers_.indexOf(trigger), 0);
    }

    int EventTriggerModel::size() const
    {
        return triggers_.size();
    }

    void EventTriggerModel::addTrigger(const QString &name, const EventTriggerWrapper::Events &onEvents, const EventTriggerWrapper::Events &offEvents, ActionSettings *action)
    {
        QString triggerName = name.isEmpty()
                ? "EventTrigger " + QString::number(triggers_.size() + 1)
                : name;

        EventTriggerWrapper* trigger = new EventTriggerWrapper(triggerName, onEvents, offEvents, action);

        rawTriggers_.push_back(trigger->rawTrigger());

        EventTriggerWrapperList::iterator it = std::find_if(triggers_.begin(), triggers_.end(),
            [&](EventTriggerWrapper* item){ return QString::localeAwareCompare(trigger->name(), item->name()) < 0; }
        );

        beginInsertRows(QModelIndex(), it - triggers_.begin(), it - triggers_.begin());

        triggers_.insert(it, trigger);

        endInsertRows();
    }

    void EventTriggerModel::removeTrigger(const QModelIndex &index)
    {
        if(! index.isValid() || index.row() >= size())
        {
            return;
        }

        beginRemoveRows(QModelIndex(), index.row(), index.row());

        delete triggers_.at(index.row());

        triggers_.removeAt(index.row());
        rawTriggers_.removeAt(index.row());

        endRemoveRows();
    }

    const EventTriggerModel::EventTriggerWrapperList& EventTriggerModel::triggers() const
    {
        return triggers_;
    }

    const EventTriggerModel::RawEventTriggerList& EventTriggerModel::rawTriggers() const
    {
        return rawTriggers_;
    }
} // namespace Gecon
