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

#ifndef CONTROLINFO_HPP
#define CONTROLINFO_HPP

#include <Gecon/Control.hpp>
#include <Gecon/Image.hpp>
#include <Gecon/V4L2VideoDevicePolicy.hpp>
#include <Gecon/ObjectGesturePolicy.hpp>
#include <Gecon/ActionPolicy.hpp>

#include <Gecon/ObjectStateGesture.hpp>
#include <Gecon/ObjectRelationGesture.hpp>
#include <Gecon/ObjectMotionGesture.hpp>

#include "ObjectPolicy.hpp"

namespace Gecon
{
    struct ControlInfo
    {
        typedef Gecon::V4L2VideoDevicePolicy<Gecon::Image<Gecon::RGB> > DevicePolicy;
        typedef Gecon::ObjectPolicy ObjectPolicy;
        typedef Gecon::ObjectGesturePolicy<ObjectPolicy::Object, ObjectPolicy::Objects> GesturePolicy;
        typedef Gecon::ActionPolicy<GesturePolicy::Event> ActionPolicy;

        typedef Gecon::Control<DevicePolicy, ObjectPolicy, GesturePolicy, ActionPolicy> Control;

        typedef DevicePolicy::DeviceAdapter DeviceAdapter;

        typedef ObjectPolicy::Object Object;
        typedef ObjectPolicy::Objects Objects;

        typedef GesturePolicy::Gesture Gesture;
        typedef GesturePolicy::Gestures Gestures;

        typedef ObjectStateGesture<Object> StateGesture;
        typedef ObjectRelationGesture<Object> RelationGesture;
        typedef ObjectMotionGesture<Object> MotionGesture;

        typedef ActionPolicy::ActionTrigger ActionTrigger;
        typedef ActionPolicy::ActionTriggers ActionTriggers;
    };
}

#endif // CONTROLINFO_HPP
