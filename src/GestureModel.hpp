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
#include "RelationGestureWrapper.hpp"
#include "MotionGestureWrapper.hpp"

#include <Gecon/ObjectGesture.hpp>

namespace Gecon
{
    class GestureModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        typedef QList<StateGestureWrapper*> StateGestureWrapperList;
        typedef QList<RelationGestureWrapper*> RelationGestureWrapperList;
        typedef QList<MotionGestureWrapper*> MotionGestureWrapperList;

        typedef QList<StateGestureWrapper::RawGesture*> RawStateGestureList;
        typedef QList<RelationGestureWrapper::RawGesture*> RawRelationGestureList;
        typedef QList<MotionGestureWrapper::RawGesture*> RawMotionGestureList;

        explicit GestureModel(QObject *parent = 0);
        virtual ~GestureModel();

        int rowCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;

        QModelIndex index(GestureWrapper* gesture) const;
        //QModelIndex index(GestureWrapper::RawGesture* rawGesture) const;

        int size() const;

        void addStateGesture(const QString& name, ObjectWrapper* object, ObjectState* stateSettings);
        void addRelationGesture(const QString& name, ObjectWrapper* leftObject, ObjectWrapper* rightObject, ObjectRelation* relationSettings);
        void addMotionGesture(const QString& name, ObjectWrapper* object, const MotionGestureWrapper::Motion& motion);

        const StateGestureWrapperList& stateGestures() const;
        const RelationGestureWrapperList& relationGestures() const;
        const MotionGestureWrapperList& motionGestures() const;

        void removeGesture(const QModelIndex& index);

    signals:
        
    public slots:

    private:
        StateGestureWrapperList stateGestures_;
        RelationGestureWrapperList relationGestures_;
        MotionGestureWrapperList motionGestures_;

        RawStateGestureList rawStateGestures_;
        RawRelationGestureList rawRelationGestures_;
        RawMotionGestureList rawMotionGestures_;
    };
} // namespace Gecon

#endif // GECON_GESTUREMODEL_HPP
