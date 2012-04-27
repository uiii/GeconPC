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
#include <Gecon/GesturePolicy.hpp>
#include <Gecon/ActionPolicy.hpp>

#include "ObjectPolicy.hpp"

namespace Gecon
{
    struct ControlInfo
    {
        typedef Gecon::V4L2VideoDevicePolicy<Gecon::Image<Gecon::RGB> > DevicePolicy;
        typedef Gecon::ObjectPolicy ObjectPolicy;
        typedef Gecon::GesturePolicy<ObjectPolicy::Object> GesturePolicy;
        typedef Gecon::ActionPolicy<GesturePolicy::Event> ActionPolicy;

        typedef Gecon::Control<DevicePolicy, ObjectPolicy, GesturePolicy, ActionPolicy> Control;
    };
}

#endif // CONTROLINFO_HPP
