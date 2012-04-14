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

#include "MotionGestureWrapper.hpp"

#include "MotionGestureDialog.hpp"

namespace Gecon
{
    MotionGestureDialog* MotionGestureWrapper::dialog = 0;

    MotionGestureWrapper::MotionGestureWrapper(const QString &name, ObjectWrapper *object, const Motion& motion):
        GestureWrapper(name),
        object_(object),
        motion_(motion)
    {
    }

    MotionGestureWrapper::~MotionGestureWrapper()
    {
    }

    ObjectWrapper *MotionGestureWrapper::object() const
    {
        return object_;
    }

    const MotionGestureWrapper::Motion &MotionGestureWrapper::motion() const
    {
        return motion_;
    }

    void MotionGestureWrapper::edit()
    {
        if(dialog)
        {
            dialog->editGesture(this);
        }
    }
} // namespace Gecon
