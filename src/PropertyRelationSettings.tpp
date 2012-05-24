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

#include "PropertyRelationSettings.hpp"

#include <QHBoxLayout>
#include <QLabel>

#include <iostream>

#include "Gecon/Point.hpp"

namespace Gecon
{
    template< typename PropertyType >
    PropertyRelation<PropertyType>::PropertyRelation(const QString& name, PropertyRelation<PropertyType>::RawRelation relation, QWidget *widget):
        name_(name),
        relation_(relation),
        widget_(widget)
    {
    }

    template< typename PropertyType >
    PropertyRelation<PropertyType>::~PropertyRelation()
    {
        delete widget_;
    }

    template< typename PropertyType >
    const QString &PropertyRelation<PropertyType>::name() const
    {
        return name_;
    }

    template< typename PropertyType >
    typename PropertyRelation<PropertyType>::RawRelation PropertyRelation<PropertyType>::rawRelation() const
    {
        return relation_;
    }

    template< typename PropertyType >
    QWidget *PropertyRelation<PropertyType>::widget() const
    {
        return widget_;
    }

    template< typename PropertyType >
    void PropertyRelation<PropertyType>::load()
    {
        // By default do nothing
    }

    template< typename PropertyType >
    void PropertyRelation<PropertyType>::save()
    {
        // By default do nothing
    }

    template< typename PropertyType >
    void PropertyRelation<PropertyType>::reset()
    {
        // By default do nothing
    }

    template< typename PropertyType >
    closer_than<PropertyType>::closer_than():
        PropertyRelation<PropertyType>("closer than", RawRelation(), new DistanceWidget),
        distance_(0),
        widget_(dynamic_cast<DistanceWidget*>(this->widget()))
    {
        widget_->layout()->addWidget(new QLabel("to"));
    }

    template< typename PropertyType >
    typename closer_than<PropertyType>::RawRelation closer_than<PropertyType>::rawRelation() const
    {
        std::cout << "jo" << std::endl;
        return std::bind(
            std::less<double>(),
            std::bind(&distance, std::placeholders::_1, std::placeholders::_2),
            distance_ / 100.0
        );
    }

    template< typename PropertyType >
    void closer_than<PropertyType>::load()
    {
        widget_->distance->setValue(distance_);
    }

    template< typename PropertyType >
    void closer_than<PropertyType>::save()
    {
        distance_ = widget_->distance->value();
    }

    template< typename PropertyType >
    void closer_than<PropertyType>::reset()
    {
        widget_->distance->setValue(0);
    }

    template< typename PropertyType >
    farther_than<PropertyType>::farther_than():
        PropertyRelation<PropertyType>("farther than", RawRelation(), new DistanceWidget),
        distance_(0),
        widget_(dynamic_cast<DistanceWidget*>(this->widget()))
    {
        widget_->layout()->addWidget(new QLabel("from"));
    }

    template< typename PropertyType >
    typename farther_than<PropertyType>::RawRelation farther_than<PropertyType>::rawRelation() const
    {
        return std::bind(
            std::greater<double>(),
            std::bind(&distance, std::placeholders::_1, std::placeholders::_2),
            distance_ / 100.0
        );
    }

    template< typename PropertyType >
    void farther_than<PropertyType>::load()
    {
        widget_->distance->setValue(distance_);
    }

    template< typename PropertyType >
    void farther_than<PropertyType>::save()
    {
        distance_ = widget_->distance->value();
    }

    template< typename PropertyType >
    void farther_than<PropertyType>::reset()
    {
        widget_->distance->setValue(0);
    }

    template< typename PropertyType >
    PropertyRelationSettings<PropertyType>::PropertyRelationSettings():
        relationIndex_(0),
        widget_(new PropertyRelationSettingsWidget),
        relations_(PropertyRelations<PropertyType>::list())
    {
        for(auto relation : relations_)
        {
            widget_->relation->addItem(relation->name(), QVariant::fromValue(relation->widget()));

            if(relation->widget())
            {
                widget_->layout()->addWidget(relation->widget());
                relation->widget()->hide();
            }
        }
    }

    template< typename PropertyType >
    typename PropertyRelationSettings<PropertyType>::RawRelation PropertyRelationSettings<PropertyType>::relation() const
    {
        std::cout << "rel: " << relations_.at(relationIndex_)->name().toAscii().data() << std::endl;
        return relations_.at(relationIndex_)->rawRelation();
    }

    template< typename PropertyType >
    QWidget *PropertyRelationSettings<PropertyType>::widget() const
    {
        return widget_;
    }

    template< typename PropertyType >
    void PropertyRelationSettings<PropertyType>::load()
    {
        widget_->relation->setCurrentIndex(relationIndex_);
        relations_.at(relationIndex_)->load();
    }

    template< typename PropertyType >
    void PropertyRelationSettings<PropertyType>::save()
    {
        relationIndex_ = widget_->relation->currentIndex();
        relations_.at(relationIndex_)->save();
    }

    template< typename PropertyType >
    void PropertyRelationSettings<PropertyType>::reset()
    {
        widget_->relation->setCurrentIndex(0);

        for(auto relation : relations_)
        {
            relation->reset();
        }
    }
} // namespace Gecon
