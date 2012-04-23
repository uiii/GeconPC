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

#include "ObjectRelationSettings.hpp"

#include <QHBoxLayout>

namespace Gecon
{
    template< typename PropertyType >
    ObjectRelationSettings<PropertyType>::ObjectRelationSettings(const QString &propertyName, RawObjectProperty<PropertyType> property):
        ObjectRelation(propertyName),
        leftProperty_(property),
        relationSettings_(new PropertyRelationSettings<PropertyType>()),
        rightPropertySettings_(new PropertyValueSettings<RawObjectProperty<PropertyType> >())
    {
    }

    template< typename PropertyType >
    ObjectRelationSettings<PropertyType>::ObjectRelationSettings(const ObjectRelationSettings &another):
        ObjectRelation(another.leftPropertyName()),
        leftProperty_(another.leftProperty_),
        relationSettings_(new PropertyRelationSettings<PropertyType>(*another.relationSettings_)),
        rightPropertySettings_(new PropertyValueSettings<RawObjectProperty<PropertyType> >(*another.rightPropertySettings_))
    {
    }

    template< typename PropertyType >
    ObjectRelationSettings<PropertyType>::~ObjectRelationSettings()
    {
        delete relationSettings_;
        delete rightPropertySettings_;
    }

    template< typename PropertyType >
    QWidget *ObjectRelationSettings<PropertyType>::relationWidget() const
    {
        return relationSettings_->widget();
    }

    template< typename PropertyType >
    QWidget *ObjectRelationSettings<PropertyType>::rightPropertyWidget() const
    {
        return rightPropertySettings_->widget();
    }

    template< typename PropertyType >
    void ObjectRelationSettings<PropertyType>::load()
    {
        relationSettings_->load();
        rightPropertySettings_->load();
    }

    template< typename PropertyType >
    void ObjectRelationSettings<PropertyType>::save()
    {
        relationSettings_->save();
        rightPropertySettings_->save();
    }

    template< typename PropertyType >
    void ObjectRelationSettings<PropertyType>::reset()
    {
        relationSettings_->reset();
        rightPropertySettings_->reset();
    }

    template< typename PropertyType >
    typename ObjectRelationSettings<PropertyType>::RawGesture *ObjectRelationSettings<PropertyType>::toGesture(ObjectWrapper* leftObject, ObjectWrapper* rightObject) const
    {
        return new RawGesture(
            leftObject->rawObject(),
            leftProperty_,
            relationSettings_->relation(),
            rightObject->rawObject(),
            rightPropertySettings_->value()
        );
    }

    template< typename PropertyType >
    ObjectRelation *ObjectRelationSettings<PropertyType>::clone() const
    {
        return new ObjectRelationSettings<PropertyType>(*this);
    }
}
