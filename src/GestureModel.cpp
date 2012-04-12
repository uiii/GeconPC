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

#include "GestureModel.hpp"

namespace Gecon
{
    GestureModel::GestureModel(QObject *parent) :
        QAbstractListModel(parent)
    {
    }

    GestureModel::~GestureModel()
    {
        for(StateGestureWrapper* stateGesture : stateGestures_)
        {
            delete stateGesture;
        }
    }

    int GestureModel::rowCount(const QModelIndex &parent) const
    {
        return size();
    }

    QVariant GestureModel::data(const QModelIndex &index, int role) const
    {
        if(! index.isValid() || index.row() >= size())
        {
            return QVariant();
        }

        GestureWrapper* gesture = stateGestures_.at(index.row());

        if(role == Qt::DisplayRole)
        {
            return gesture->name();
        }
        else if(role == Qt::UserRole)
        {
            return QVariant::fromValue(gesture);
        }
        else
        {
            return QVariant();
        }
    }

    QModelIndex GestureModel::index(GestureWrapper* gesture) const
    {
        return createIndex(stateGestures_.indexOf(static_cast<StateGestureWrapper*>(gesture)), 0);
    }

    int GestureModel::size() const
    {
        return stateGestures_.size(); // + other gestures
    }

    void GestureModel::addStateGesture(const QString& name, const ObjectWrapper& object, const ObjectPropertyStateSettings* stateSettings)
    {
        StateGestureWrapper* stateGesture = new StateGestureWrapper(name, object, stateSettings);

        StateGestureWrapperList::iterator it = std::find_if(stateGestures_.begin(), stateGestures_.end(),
            [&](const StateGestureWrapper* item){ return QString::localeAwareCompare(stateGesture->name(), item->name()) < 0; }
        );

        beginInsertRows(QModelIndex(), it - stateGestures_.begin(), it - stateGestures_.begin());

        stateGestures_.insert(it, stateGesture);

        endInsertRows();
    }

    void GestureModel::removeStateGesture(const QModelIndex &index)
    {
        if(! index.isValid() || index.row() >= size())
        {
            return;
        }

        beginRemoveRows(QModelIndex(), index.row(), index.row());

        stateGestures_.removeAt(index.row());

        endRemoveRows();
    }
} // namespace Gecon
