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
    class ActionTriggerModel;

    class GestureModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        typedef QList<StateGestureWrapper*> StateGestureWrappers;
        typedef QList<RelationGestureWrapper*> RelationGestureWrappers;
        typedef QList<MotionGestureWrapper*> MotionGestureWrappers;

        typedef QList<ControlInfo::StateGesture*> RawStateGestures;
        typedef QList<ControlInfo::RelationGesture*> RawRelationGestures;
        typedef QList<ControlInfo::MotionGesture*> RawMotionGestures;

        explicit GestureModel(ActionTriggerModel* actionTriggerModel);
        virtual ~GestureModel();

        int rowCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;

        QModelIndex index(GestureWrapper* gesture) const;
        //QModelIndex index(GestureWrapper::RawGesture* rawGesture) const;

        int size() const;

        void addStateGesture(const QString& name, ObjectWrapper* object, ObjectState* stateSettings);
        void addRelationGesture(const QString& name, ObjectWrapper* leftObject, ObjectWrapper* rightObject, ObjectRelation* relationSettings);
        void addMotionGesture(const QString& name, ObjectWrapper* object, const ControlInfo::MotionGesture::Motion& motion);

        void editStateGesture(const QModelIndex& index, const QString& name, ObjectWrapper* object, ObjectState* stateSettings);
        void editRelationGesture(const QModelIndex& index, const QString& name, ObjectWrapper* leftObject, ObjectWrapper* rightObject, ObjectRelation* relationSettings);
        void editMotionGesture(const QModelIndex& index, const QString& name, ObjectWrapper* object, const ControlInfo::MotionGesture::Motion& motion);

        bool removeGesture(const QModelIndex& index);

        const StateGestureWrappers& stateGestures() const;
        const RelationGestureWrappers& relationGestures() const;
        const MotionGestureWrappers& motionGestures() const;

        ControlInfo::GestureCheckers checkers();

    signals:
        
    public slots:

    private:
        StateGestureWrappers stateGestures_;
        RelationGestureWrappers relationGestures_;
        MotionGestureWrappers motionGestures_;

        RawStateGestures rawStateGestures_;
        RawRelationGestures rawRelationGestures_;
        RawMotionGestures rawMotionGestures_;

        ControlInfo::StateGestureChecker* stateGestureChecker_;
        ControlInfo::RelationGestureChecker* relationGestureChecker_;
        ControlInfo::MotionGestureChecker* motionGestureChecker_;

        ActionTriggerModel* actionTriggerModel_;
    };
} // namespace Gecon

#endif // GECON_GESTUREMODEL_HPP
