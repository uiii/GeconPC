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

#include "StateGestureWrapper.hpp"

#include "StateGestureDialog.hpp"

namespace Gecon
{
    StateGestureDialog* StateGestureWrapper::dialog = 0;

    StateGestureWrapper::StateGestureWrapper(const QString& name, ObjectWrapper* object, ObjectState *state):
        GestureWrapper(name, {object}),
        object_(object),
        state_(state->clone())
    {
        rawGesture_ = state_->toGesture(object_);

        events_.push_back(new EventWrapper("state enter", rawGesture_->stateEnterEvent(), this));
        events_.push_back(new EventWrapper("state leave", rawGesture_->stateLeaveEvent(), this));
    }

    StateGestureWrapper::StateGestureWrapper(const StateGestureWrapper& another):
        StateGestureWrapper(another.name(), another.object_, another.state_)
    {
    }

    StateGestureWrapper::~StateGestureWrapper()
    {
        delete state_;

        for(EventWrapper* event : events_)
        {
            delete event;
        }
    }

    const StateGestureWrapper::Events& StateGestureWrapper::events() const
    {
        return events_;
    }

    ObjectWrapper* StateGestureWrapper::object()
    {
        return object_;
    }

    ObjectState* StateGestureWrapper::state()
    {
        return state_;
    }

    void StateGestureWrapper::setObject(ObjectWrapper* object)
    {
        object_ = object;

        setObjects({object});

        *rawGesture_ = *(state_->toGesture(object_));
    }

    void StateGestureWrapper::setState(ObjectState* state)
    {
        delete state_;
        state_ = state->clone();

        *rawGesture_ = *(state_->toGesture(object_));
    }

    void StateGestureWrapper::edit()
    {
        if(dialog)
        {
            dialog->editGesture(this);
        }
    }

    ControlInfo::StateGesture* StateGestureWrapper::rawGesture()
    {
        return rawGesture_;
    }
} // namespace Gecon
