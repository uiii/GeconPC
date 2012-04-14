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

#ifndef GECON_MOTIONGESTUREWRAPPER_HPP
#define GECON_MOTIONGESTUREWRAPPER_HPP

#include "ControlInfo.hpp"

#include "GestureWrapper.hpp"
#include "ObjectWrapper.hpp"

#include <Gecon/ObjectMotionGesture.hpp>

namespace Gecon
{
    class MotionGestureDialog; // forward declaration

    class MotionGestureWrapper : public GestureWrapper
    {
    public:
        typedef ObjectMotionGesture<ControlInfo::ObjectPolicy::Object> RawGesture;
        typedef RawGesture::Motion Motion;

        static MotionGestureDialog* dialog;

        MotionGestureWrapper(const QString& name, ObjectWrapper* object, const Motion& motion);

        virtual ~MotionGestureWrapper();

        ObjectWrapper* object() const;
        const Motion& motion() const;

        void edit();

    private:
        ObjectWrapper* object_;
        Motion motion_;

        ControlInfo::GesturePolicy::GesturePtr rawMotionGesture_;
    };
} // namespace Gecon

#endif // GECON_MOTIONGESTUREWRAPPER_HPP
