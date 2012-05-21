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

#include <Gecon/Fraction.hpp>
#include <Gecon/ColorObjectPolicy.hpp>

#include "ControlInfo.hpp"

namespace Gecon
{
    class ObjectWrapper
    {
    public:
        typedef ControlInfo::Object RawObject;

        ObjectWrapper();
        ObjectWrapper(const QString& name, const RawObject::Color& color);
        virtual ~ObjectWrapper();

        const QString& name() const;
        QColor color() const;

        void setName(const QString& name);
        void setColor(const RawObject::Color& color);

        RawObject* rawObject() const;

    private:
        QString name_;
        QColor color_;

        RawObject* rawObject_;
    };

    template< typename PropertyType >
    using RawObjectProperty = PropertyType(ObjectWrapper::RawObject::*)() const;

    template< typename PropertyType >
    struct ObjectPropertyWrapper
    {
        QString name;
        RawObjectProperty<PropertyType> rawProperty;
    };

    template< typename PropertyType >
    struct ObjectProperties
    {
        static const QList<ObjectPropertyWrapper<PropertyType> > list;
    };

    template< typename PropertyType >
    const QList<ObjectPropertyWrapper<PropertyType> > ObjectProperties<PropertyType>::list = {};

    template<> const QList<ObjectPropertyWrapper<bool> > ObjectProperties<bool>::list;
    template<> const QList<ObjectPropertyWrapper<int> > ObjectProperties<int>::list;
    template<> const QList<ObjectPropertyWrapper<double> > ObjectProperties<double>::list;
    template<> const QList<ObjectPropertyWrapper<Fraction> > ObjectProperties<Fraction>::list;
    template<> const QList<ObjectPropertyWrapper<Point> > ObjectProperties<Point>::list;

    bool operator==(const ObjectWrapper& left, const ObjectWrapper& right);
    //bool operator<(const ObjectWrapper& left, const ObjectWrapper& right);

} // namespace Gecon

Q_DECLARE_METATYPE(Gecon::ObjectWrapper*)

#endif // GECON_OBJECTWRAPPER_HPP
