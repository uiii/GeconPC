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
        Color<RGB> rgb = color;
        color_ = QColor(rgb.r, rgb.g, rgb.b);
    }

    const QString& ObjectWrapper::name() const
    {
        return name_;
    }

    QColor ObjectWrapper::color() const
    {
        return color_;
    }

    ObjectWrapper::RawObjectPtr ObjectWrapper::rawObject() const
    {
        return rawObject_;
    }

    bool operator==(const ObjectWrapper& left, const ObjectWrapper& right)
    {
        return left.name() == right.name() && left.rawObject() == right.rawObject();
    }

    template<>
    const QList<ObjectPropertyWrapper<bool> > ObjectProperties<bool>::list = {
        { "visibility", &ObjectWrapper::RawObject::isVisible }
    };

    template<>
    const QList<ObjectPropertyWrapper<int> > ObjectProperties<int>::list ={
        { "angle", &ObjectWrapper::RawObject::angle }
    };

    template<>
    const QList<ObjectPropertyWrapper<Point> > ObjectProperties<Point>::list = {
        { "position", &ObjectWrapper::RawObject::position }
    };

    bool operator<(const ObjectWrapper& left, const ObjectWrapper& right)
    {
        return left.name() < right.name();
    }
} // namespace Gecon
