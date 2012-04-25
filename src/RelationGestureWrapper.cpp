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

#include "RelationGestureWrapper.hpp"

#include "RelationGestureDialog.hpp"

namespace Gecon
{
    RelationGestureDialog* RelationGestureWrapper::dialog = 0;

    RelationGestureWrapper::RelationGestureWrapper(
            const QString& name,
            ObjectWrapper* leftObject,
            ObjectWrapper* rightObject,
            ObjectRelation *relation):
        GestureWrapper(name, {leftObject, rightObject}),
        left_(leftObject),
        right_(rightObject),
        relation_(relation->clone())
    {
        rawGesture_ = relation_->toGesture(leftObject, rightObject);

        events_.push_back(new EventWrapper("relation enter", rawGesture_->relationEnterEvent(), this));
        events_.push_back(new EventWrapper("relation leave", rawGesture_->relationLeaveEvent(), this));
        events_.push_back(new EventWrapper("in relation", rawGesture_->inRelationEvent(), this));
        events_.push_back(new EventWrapper("not in relation", rawGesture_->notInRelationEvent(), this));
    }

    RelationGestureWrapper::RelationGestureWrapper(const RelationGestureWrapper& another):
        RelationGestureWrapper(another.name(), another.left_, another.right_, another.relation_)
    {
    }

    RelationGestureWrapper::~RelationGestureWrapper()
    {
        delete relation_;

        for(EventWrapper* event : events_)
        {
            delete event;
        }
    }

    const RelationGestureWrapper::Events& RelationGestureWrapper::events() const
    {
        return events_;
    }

    ObjectWrapper* RelationGestureWrapper::leftObject()
    {
        return left_;
    }

    ObjectWrapper* RelationGestureWrapper::rightObject()
    {
        return right_;
    }

    ObjectRelation* RelationGestureWrapper::relation()
    {
        return relation_;
    }

    void RelationGestureWrapper::edit()
    {
        if(dialog)
        {
            dialog->editGesture(this);
        }
    }

    RelationGestureWrapper::RawGesture *RelationGestureWrapper::rawGesture()
    {
        return rawGesture_;
    }
} // namespace Gecon
