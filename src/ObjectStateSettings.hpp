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

#ifndef GECON_PROPERTYSTATESETTINGS_HPP
#define GECON_PROPERTYSTATESETTINGS_HPP

#include <functional>

#include <QString>

#include <Gecon/ObjectStateGesture.hpp>

#include "ControlInfo.hpp"
#include "ObjectWrapper.hpp"

#include "PropertyRelationSettings.hpp"
#include "PropertyValueSettings.hpp"

namespace Gecon
{
    class ObjectState
    {
    public:
        typedef ObjectStateGesture<ObjectWrapper::RawObject> RawGesture;

        ObjectState(const QString& propertyName);
        virtual ~ObjectState();

        virtual const QString& propertyName() const;
        virtual QWidget* widget() const = 0;

        virtual void load() = 0;
        virtual void save() = 0;
        virtual void reset() = 0;

        virtual RawGesture* toGesture(ObjectWrapper* object) const = 0;

        virtual ObjectState* clone() const = 0;

    private:
        QString propertyName_;
    };

    template< typename PropertyType >
    class ObjectStateSettings : public ObjectState
    {
    public:
        typedef PropertyType(ObjectWrapper::RawObject::*Property)() const;

        ObjectStateSettings(
            const QString& propertyName,
            Property property
        );
        ObjectStateSettings(const ObjectStateSettings& another);

        virtual ~ObjectStateSettings();

        QWidget* widget() const;

        virtual void load();
        virtual void save();
        virtual void reset();

        RawGesture* toGesture(ObjectWrapper* object) const;

        ObjectState* clone() const;

    private:
        Property property_;
        PropertyRelationSettings<PropertyType>* relationSettings_;
        PropertyValueSettings<PropertyType>* valueSettings_;

        QWidget* widget_;
    };
} // namespace Gecon

#include "ObjectStateSettings.tpp"

#endif // GECON_PROPERTYSTATESETTINGS_HPP
