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

#include "PropertyValueSettings.hpp"

#include <QHBoxLayout>

namespace Gecon
{
    template< typename PropertyType >
    PropertyValueSettings<RawObjectProperty<PropertyType> >::Widget::Widget():
        property(new QComboBox)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(property);
    }

    template< typename PropertyType >
    PropertyValueSettings<RawObjectProperty<PropertyType> >::PropertyValueSettings():
        widget_(new Widget)
    {
        for(auto property : ObjectProperties<PropertyType>::list)
        {
            widget_->property->addItem(property.name);
            properties_.push_back(property.rawProperty);
        }
    }

    template< typename PropertyType >
    RawObjectProperty<PropertyType> PropertyValueSettings<RawObjectProperty<PropertyType> >::value() const
    {
        return properties_.at(propertyIndex_);
    }

    template< typename PropertyType >
    QWidget *Gecon::PropertyValueSettings<RawObjectProperty<PropertyType> >::widget() const
    {
        return widget_;
    }

    template< typename PropertyType >
    void PropertyValueSettings<RawObjectProperty<PropertyType> >::load()
    {
        widget_->property->setCurrentIndex(propertyIndex_);
    }

    template< typename PropertyType >
    void Gecon::PropertyValueSettings<RawObjectProperty<PropertyType> >::save()
    {
        propertyIndex_ = widget_->property->currentIndex();
    }

    template< typename PropertyType >
    void Gecon::PropertyValueSettings<RawObjectProperty<PropertyType> >::reset()
    {
        widget_->property->setCurrentIndex(0);
    }
}
