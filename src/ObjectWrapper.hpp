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

#ifndef GECON_OBJECTWRAPPER_HPP
#define GECON_OBJECTWRAPPER_HPP

#include <QString>
#include <QColor>
#include <QMetaType>

#include "Gecon/ColorObjectPolicy.hpp"

namespace Gecon
{
    class ObjectWrapper
    {
    public:
        typedef ColorObjectPolicy::Object RawObject;
        typedef ColorObjectPolicy::ObjectPtr RawObjectPtr;

        ObjectWrapper();
        ObjectWrapper(const QString& name, RawObject::Color color);

        const QString& name() const;
        QColor color() const;
        RawObjectPtr rawObject() const;

    private:
        QString name_;
        QColor color_;

        RawObjectPtr rawObject_;
    };

    bool operator==(const ObjectWrapper& left, const ObjectWrapper& right);
    //bool operator<(const ObjectWrapper& left, const ObjectWrapper& right);

} // namespace Gecon

Q_DECLARE_METATYPE(Gecon::ObjectWrapper)

#endif // GECON_OBJECTWRAPPER_HPP
