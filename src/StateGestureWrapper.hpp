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
#include "ObjectPropertyStateSettings.hpp"

#include "GestureWrapper.hpp"

namespace Gecon
{
    class StateGestureDialog; // forward declaration

    class StateGestureWrapper : public GestureWrapper
    {
    public:
        static StateGestureDialog* dialog;

        StateGestureWrapper(const QString& name, const ObjectWrapper& object, const ObjectPropertyStateSettings* stateSettings);
        StateGestureWrapper(const StateGestureWrapper& another);
        virtual ~StateGestureWrapper();

        const QString& name() const;
        const ObjectWrapper& object() const;
        ObjectPropertyStateSettings* stateSettings() const;

        void edit();

    private:
        QString name_;
        ObjectWrapper object_;
        ObjectPropertyStateSettings* stateSettings_;

        ControlInfo::GesturePolicy::GesturePtr rawStateGesture_;
    };
} // namespace Gecon

#endif // GECON_STATEGESTUREWRAPPER_HPP
