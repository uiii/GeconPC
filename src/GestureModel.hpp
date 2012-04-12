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

#ifndef GECON_GESTUREMODEL_HPP
#define GECON_GESTUREMODEL_HPP

#include <QAbstractListModel>

#include "StateGestureWrapper.hpp"

namespace Gecon
{
    class GestureModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        typedef QList<StateGestureWrapper*> StateGestureWrapperList;

        explicit GestureModel(QObject *parent = 0);
        virtual ~GestureModel();

        int rowCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;

        QModelIndex index(GestureWrapper* gesture) const;

        int size() const;

        void addStateGesture(const QString& name, const ObjectWrapper& object, const ObjectPropertyStateSettings* stateSettings);
        void removeStateGesture(const QModelIndex& index);

    signals:
        
    public slots:

    private:
        StateGestureWrapperList stateGestures_;
    };
} // namespace Gecon

#endif // GECON_GESTUREMODEL_HPP
