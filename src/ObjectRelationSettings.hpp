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

#ifndef GECON_OBJECTRELATIONSETTINGS_HPP
#define GECON_OBJECTRELATIONSETTINGS_HPP

#include <QString>
#include <QWidget>

#include <Gecon/ObjectRelationGesture.hpp>

#include "ObjectWrapper.hpp"
#include "PropertyRelationSettings.hpp"
#include "PropertyValueSettings.hpp"

namespace Gecon
{
    class ObjectRelation
    {
    public:
        typedef ObjectRelationGesture<ObjectWrapper::RawObject> RawGesture;

        ObjectRelation(const QString& leftPropertyName);
        virtual ~ObjectRelation();

        virtual const QString& leftPropertyName() const;
        virtual QWidget* relationWidget() const = 0;
        virtual QWidget* rightPropertyWidget() const = 0;

        virtual void load() = 0;
        virtual void save() = 0;
        virtual void reset() = 0;

        virtual RawGesture* toGesture(ObjectWrapper* leftObject, ObjectWrapper* rightObject) const = 0;

        virtual ObjectRelation* clone() const = 0;

    private:
        QString propertyName_;
    };

    template< typename PropertyType >
    class ObjectRelationSettings : public ObjectRelation
    {
    public:
        ObjectRelationSettings(
            const QString& leftPropertyName,
            RawObjectProperty<PropertyType> property
        );
        ObjectRelationSettings(const ObjectRelationSettings& another);
        virtual ~ObjectRelationSettings();

        QWidget* relationWidget() const;
        QWidget* rightPropertyWidget() const;

        virtual void load();
        virtual void save();
        virtual void reset();

        RawGesture* toGesture(ObjectWrapper* leftObject, ObjectWrapper* rightObject) const;

        ObjectRelation* clone() const;

    private:
        RawObjectProperty<PropertyType> leftProperty_;
        PropertyRelationSettings<PropertyType>* relationSettings_;
        PropertyValueSettings<RawObjectProperty<PropertyType> >* rightPropertySettings_;
    };
} // namespace Gecon

#include "ObjectRelationSettings.tpp"

#endif // GECON_OBJECTRELATIONSETTINGS_HPP
