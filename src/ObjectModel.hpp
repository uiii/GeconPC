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

#ifndef GECON_OBJECTMODEL_HPP
#define GECON_OBJECTMODEL_HPP

#include <QAbstractListModel>
#include <QColor>

#include "ControlInfo.hpp"
#include "ObjectWrapper.hpp"

namespace Gecon
{
    class GestureModel;

    class ObjectModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        typedef QList<ObjectWrapper*> ObjectWrappers;

        ObjectModel(GestureModel* gestureModel);
        virtual ~ObjectModel();

        int rowCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;

        QModelIndex index(ObjectWrapper* object) const;
        QModelIndex index(ControlInfo::Object* object) const;

        int size() const;

        void addObject(const QString& name, ControlInfo::Object::Color color);
        void editObject(const QModelIndex& index, const QString& name, ControlInfo::Object::Color color);
        bool removeObject(const QModelIndex& index);

        const ObjectWrappers& objects() const;
        const ControlInfo::Objects& rawObjects() const;

    private:
        ObjectWrappers objects_;

        ControlInfo::Objects rawObjects_;

        GestureModel* gestureModel_;
    };
} // namespace Gecon

#endif // GECON_OBJECTMODEL_HPP
