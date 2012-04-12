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

#include "ObjectModel.hpp"

namespace Gecon
{
    ObjectModel::ObjectModel()
    {
    }

    int ObjectModel::rowCount(const QModelIndex&) const
    {
        return size();
    }

    QVariant ObjectModel::data(const QModelIndex& index, int role) const
    {
        if(! index.isValid() || index.row() >= objects_.size())
        {
            return QVariant();
        }

        ObjectWrapper object = objects_.at(index.row());

        if(role == Qt::DisplayRole)
        {
            return object.name();
        }
        else if(role == Qt::DecorationRole)
        {
            return object.color();
        }
        else if(role == Qt::UserRole)
        {
            return QVariant::fromValue(object);
        }
        else
        {
            return QVariant();
        }
    }

    QModelIndex ObjectModel::index(const ObjectWrapper &object) const
    {
        return createIndex(objects_.indexOf(object), 0);
    }

    int ObjectModel::size() const
    {
        return objects_.size();
    }

    void ObjectModel::addObject(const ObjectWrapper& object)
    {
        // TODO
        /*if(objects_.find(object->color()) != objects_.end())
        {
            // throw
        }*/

        rawObjects_.insert(object.rawObject());

        ObjectWrapperList::iterator it = std::find_if(objects_.begin(), objects_.end(),
            [&](const ObjectWrapper& item){ return QString::localeAwareCompare(object.name(), item.name()) < 0; }
        );

        beginInsertRows(QModelIndex(), it - objects_.begin(), it - objects_.begin());

        objects_.insert(it, object);

        endInsertRows();
    }

    void ObjectModel::removeObject(const QModelIndex& index)
    {
        if(! index.isValid() || index.row() >= objects_.size())
        {
            // throw
        }

        // TODO
        /*auto it = objects_.find(visualData.second);
        if(it == objects_.end())
        {
            // throw
        }

        objects_.erase(it);*/
    }
} // namespace Gecon
