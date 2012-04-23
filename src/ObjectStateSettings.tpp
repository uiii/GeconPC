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

#include "ObjectStateSettings.hpp"

namespace Gecon
{
    template< typename PropertyType >
    ObjectStateSettings<PropertyType>::ObjectStateSettings(
            const QString& propertyName,
            Property property
        ):
        ObjectState(propertyName),
        property_(property),
        relationSettings_(new PropertyRelationSettings<PropertyType>()),
        valueSettings_(new PropertyValueSettings<PropertyType>()),
        widget_(new QWidget)
    {
        QHBoxLayout* layout = new QHBoxLayout(widget_);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(relationSettings_->widget());
        layout->addWidget(valueSettings_->widget());
    }

    template< typename PropertyType >
    ObjectStateSettings<PropertyType>::ObjectStateSettings(const ObjectStateSettings &another):
        ObjectState(another.propertyName()),
        property_(another.property_),
        relationSettings_(new PropertyRelationSettings<PropertyType>(*another.relationSettings_)),
        valueSettings_(new PropertyValueSettings<PropertyType>(*another.valueSettings_)),
        widget_(another.widget_)
    {
    }

    template< typename PropertyType >
    ObjectStateSettings<PropertyType>::~ObjectStateSettings()
    {
        delete relationSettings_;
        delete valueSettings_;
    }


    template< typename PropertyType >
    QWidget* ObjectStateSettings<PropertyType>::widget() const
    {
        return widget_;
    }

    template< typename PropertyType >
    void ObjectStateSettings<PropertyType>::load()
    {
        relationSettings_->load();
        valueSettings_->load();
    }

    template< typename PropertyType >
    void ObjectStateSettings<PropertyType>::save()
    {
        relationSettings_->save();
        valueSettings_->save();
    }

    template< typename PropertyType >
    void ObjectStateSettings<PropertyType>::reset()
    {
        relationSettings_->reset();
        valueSettings_->reset();
    }

    template< typename PropertyType >
    typename ObjectStateSettings<PropertyType>::RawGesture* ObjectStateSettings<PropertyType>::toGesture(ObjectWrapper *object) const
    {
        return new RawGesture(
            object->rawObject(),
            property_,
            relationSettings_->relation(),
            valueSettings_->value()
        );
    }

    template< typename PropertyType >
    ObjectState *ObjectStateSettings<PropertyType>::clone() const
    {
        return new ObjectStateSettings<PropertyType>(*this);
    }
}
