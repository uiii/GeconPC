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

#include "ActionTriggerModel.hpp"

namespace Gecon
{
    ActionTriggerModel::ActionTriggerModel()
    {
    }

    ActionTriggerModel::~ActionTriggerModel()
    {
        for(ActionTriggerWrapper* trigger : triggers_)
        {
            delete trigger;
        }
    }

    int ActionTriggerModel::rowCount(const QModelIndex &parent) const
    {
        return size();
    }

    QVariant ActionTriggerModel::data(const QModelIndex &index, int role) const
    {
        if(! index.isValid() || index.row() >= size())
        {
            return QVariant();
        }

        ActionTriggerWrapper* trigger = triggers_.at(index.row());

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

    QModelIndex ActionTriggerModel::index(ActionTriggerWrapper* trigger) const
    {
        return createIndex(triggers_.indexOf(trigger), 0);
    }

    int ActionTriggerModel::size() const
    {
        return triggers_.size();
    }

    void ActionTriggerModel::addTrigger(const QString &name, const ActionTriggerWrapper::Events &onEvents, const ActionTriggerWrapper::Events &offEvents, ActionSettings *action)
    {
        QString triggerName = name.isEmpty()
                ? "ActionTrigger " + QString::number(triggers_.size() + 1)
                : name;

        ActionTriggerWrapper* trigger = new ActionTriggerWrapper(triggerName, onEvents, offEvents, action);

        rawTriggers_.insert(trigger->rawTrigger());

        ActionTriggerWrappers::iterator it = std::find_if(triggers_.begin(), triggers_.end(),
            [&](ActionTriggerWrapper* item){ return QString::localeAwareCompare(trigger->name(), item->name()) < 0; }
        );

        beginInsertRows(QModelIndex(), it - triggers_.begin(), it - triggers_.begin());

        triggers_.insert(it, trigger);

        endInsertRows();
    }

    bool ActionTriggerModel::removeTrigger(const QModelIndex &index)
    {
        if(! index.isValid() || index.row() >= size())
        {
            return false;
        }

        ActionTriggerWrapper* trigger = triggers_.at(index.row());

        rawTriggers_.erase(trigger->rawTrigger());

        beginRemoveRows(QModelIndex(), index.row(), index.row());

        delete trigger;
        triggers_.removeAt(index.row());

        endRemoveRows();

        return true;
    }

    const ActionTriggerModel::ActionTriggerWrappers& ActionTriggerModel::triggers() const
    {
        return triggers_;
    }

    const ControlInfo::ActionTriggers& ActionTriggerModel::rawTriggers() const
    {
        return rawTriggers_;
    }
} // namespace Gecon
