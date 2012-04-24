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

#ifndef GECON_EVENTWRAPPER_HPP
#define GECON_EVENTWRAPPER_HPP

#include <QString>
#include <QMetaType>

namespace Gecon
{
    class Event;
    class GestureWrapper;

    class EventWrapper
    {
    public:
        EventWrapper(const QString& name, Event* event, GestureWrapper* gesture);

        const QString& name() const;
        Event* rawEvent() const;

        GestureWrapper* gesture() const;

    private:
        QString name_;
        Event* event_;

        GestureWrapper* gesture_;
    };
} // namespace Gecon

#endif // GECON_EVENTWRAPPER_HPP
