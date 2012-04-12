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

#include "ObjectWrapper.hpp"

namespace Gecon
{
    ObjectWrapper::ObjectWrapper():
        rawObject_(0)
    {
    }

    ObjectWrapper::ObjectWrapper(const QString &name, RawObject::Color color):
        name_(name),
        rawObject_(new RawObject(color))
    {
    }

    const QString& ObjectWrapper::name() const
    {
        return name_;
    }

    QColor ObjectWrapper::color() const
    {
        Color<RGB> rgb = color_;
        return QColor(rgb.r, rgb.g, rgb.b);
    }

    ObjectWrapper::RawObjectPtr ObjectWrapper::rawObject() const
    {
        return rawObject_;
    }

    bool operator==(const ObjectWrapper& left, const ObjectWrapper& right)
    {
        return left.name() == right.name() && left.rawObject() == right.rawObject();
    }

    bool operator<(const ObjectWrapper& left, const ObjectWrapper& right)
    {
        return left.name() < right.name();
    }
} // namespace Gecon
