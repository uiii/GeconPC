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

    StateGestureWrapper::StateGestureWrapper(const QString& name, const ObjectWrapper &object, const ObjectPropertyStateSettings *stateSettings):
        name_(name),
        object_(object),
        stateSettings_(stateSettings->clone())
    {
        rawStateGesture_ = stateSettings_->toGesture(object_);
    }

    StateGestureWrapper::StateGestureWrapper(const StateGestureWrapper& another):
        StateGestureWrapper(another.name_, another.object_, another.stateSettings_)
    {
    }

    StateGestureWrapper::~StateGestureWrapper()
    {
        delete stateSettings_;
    }

    const QString &StateGestureWrapper::name() const
    {
        return name_;
    }

    const ObjectWrapper &StateGestureWrapper::object() const
    {
        return object_;
    }

    ObjectPropertyStateSettings *StateGestureWrapper::stateSettings() const
    {
        return stateSettings_;
    }

    void StateGestureWrapper::edit()
    {
        if(dialog)
        {
            dialog->editGesture(this);
        }
    }
} // namespace Gecon
