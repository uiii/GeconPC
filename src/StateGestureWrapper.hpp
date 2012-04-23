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

#ifndef GECON_STATEGESTUREWRAPPER_HPP
#define GECON_STATEGESTUREWRAPPER_HPP

#include "ObjectWrapper.hpp"
#include "ObjectStateSettings.hpp"

#include "GestureWrapper.hpp"

#include <Gecon/ObjectStateGesture.hpp>

namespace Gecon
{
    class StateGestureDialog; // forward declaration

    class StateGestureWrapper : public GestureWrapper
    {
    public:
        typedef ObjectStateGesture<ControlInfo::ObjectPolicy::Object> RawGesture;

        static StateGestureDialog* dialog;

        StateGestureWrapper(const QString& name, ObjectWrapper* object, ObjectState* state);
        StateGestureWrapper(const StateGestureWrapper& another);
        virtual ~StateGestureWrapper();

        ObjectWrapper* object();
        ObjectState* state();

        void edit();

        RawGesture* rawGesture();

    private:
        ObjectWrapper* object_;
        ObjectState* state_;

        RawGesture* rawGesture_;
    };
} // namespace Gecon

#endif // GECON_STATEGESTUREWRAPPER_HPP
