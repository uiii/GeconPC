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
    class ObjectModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        typedef ControlInfo::ObjectPolicy::ObjectPtr ObjectPtr;
        typedef ControlInfo::ObjectPolicy::Object RawObject;
        typedef ControlInfo::ObjectPolicy::ObjectSet RawObjectSet;

        typedef QList<ObjectWrapper*> ObjectWrapperList;

        ObjectModel();
        virtual ~ObjectModel();

        int rowCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;

        QModelIndex index(ObjectWrapper* objects) const;
        QModelIndex index(ObjectWrapper::RawObject* objects) const;

        int size() const;

        void addObject(const QString& name, RawObject::Color color);
        void removeObject(const QModelIndex& index);

        const ObjectWrapperList& objects() const;
        const RawObjectSet& rawObjects() const;

    private:
        ObjectWrapperList objects_;

        RawObjectSet rawObjects_;
    };
} // namespace Gecon

#endif // GECON_OBJECTMODEL_HPP
