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

    ObjectModel::~ObjectModel()
    {
        for(ObjectWrapper* object : objects_)
        {
            delete object;
        }
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

        ObjectWrapper* object = objects_.at(index.row());

        if(role == Qt::DisplayRole)
        {
            return object->name();
        }
        else if(role == Qt::DecorationRole)
        {
            return object->color();
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

    QModelIndex ObjectModel::index(ObjectWrapper* object) const
    {
        return createIndex(objects_.indexOf(object), 0);
    }

    int ObjectModel::size() const
    {
        return objects_.size();
    }

    void ObjectModel::addObject(const QString& name, ControlInfo::Object::Color color)
    {
        QString objectName = name.isEmpty()
                ? "Object " + QString::number(objects_.size() + 1)
                : name;

        ObjectWrapper* object = new ObjectWrapper(objectName, color);

        rawObjects_.insert(object->rawObject());

        ObjectWrappers::iterator it = std::find_if(objects_.begin(), objects_.end(),
            [&](ObjectWrapper* item){ return QString::localeAwareCompare(object->name(), item->name()) < 0; }
        );

        beginInsertRows(QModelIndex(), it - objects_.begin(), it - objects_.begin());

        objects_.insert(it, object);

        endInsertRows();
    }

    void ObjectModel::editObject(const QModelIndex& index, const QString& name, ControlInfo::Object::Color color)
    {
        if(! index.isValid() || index.row() >= objects_.size())
        {
            return;
        }

        QString objectName = name.isEmpty()
                ? "Object " + QString::number(objects_.size() + 1)
                : name;

        ObjectWrapper* object = objects_.at(index.row());
        object->setName(objectName);
        object->setColor(color);

        rawObjects_.update(object->rawObject());

        beginRemoveRows(QModelIndex(), index.row(), index.row());

        objects_.removeAt(index.row());

        endRemoveRows();

        ObjectWrappers::iterator it = std::find_if(objects_.begin(), objects_.end(),
            [&](ObjectWrapper* item){ return QString::localeAwareCompare(object->name(), item->name()) < 0; }
        );

        beginInsertRows(QModelIndex(), it - objects_.begin(), it - objects_.begin());

        objects_.insert(it, object);

        endInsertRows();
    }

    void ObjectModel::removeObject(const QModelIndex& index)
    {
        if(! index.isValid() || index.row() >= objects_.size())
        {
            return;
        }

        ObjectWrapper* object = objects_.at(index.row());
        rawObjects_.remove(object->rawObject());

        delete object;

        beginRemoveRows(QModelIndex(), index.row(), index.row());

        objects_.removeAt(index.row());

        endRemoveRows();
    }

    const ObjectModel::ObjectWrappers &ObjectModel::objects() const
    {
        return objects_;
    }

    const ControlInfo::Objects &ObjectModel::rawObjects() const
    {
        return rawObjects_;
    }
} // namespace Gecon
