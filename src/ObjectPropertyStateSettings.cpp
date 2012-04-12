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

#include "ObjectPropertyStateSettings.hpp"

#include <QHBoxLayout>
#include <QLabel>

#include <Gecon/ObjectStateGesture.hpp>

namespace Gecon
{
    ObjectPropertyStateSettings::ObjectPropertyStateSettings(const QString& propertyName, QWidget* parent):
        widget_(new QWidget(parent)),
        propertyName_(propertyName)
    {
    }

    ObjectPropertyStateSettings::~ObjectPropertyStateSettings()
    {
    }

    const QString& ObjectPropertyStateSettings::propertyName()
    {
        return propertyName_;
    }

    QWidget* ObjectPropertyStateSettings::widget()
    {
        return widget_;
    }

    VisibilityStateSettings::VisibilityStateSettings(QWidget* parent):
        ObjectPropertyStateSettings("visibility", parent),
        visibilityOptionsIndex_(0)
    {
        QHBoxLayout* layout = new QHBoxLayout(widget_);
        layout->setContentsMargins(0,0,0,0);

        visibilityOptions_ = new QComboBox(widget_);
        visibilityOptions_->addItem(QObject::tr("visible"));
        visibilityOptions_->addItem(QObject::tr("hidden"));

        layout->addWidget(new QLabel(QObject::tr("="), widget_));
        layout->addWidget(visibilityOptions_);
    }

    void VisibilityStateSettings::reset()
    {
        visibilityOptions_->setCurrentIndex(0);
    }

    void VisibilityStateSettings::save()
    {
        visibilityOptionsIndex_ = visibilityOptions_->currentIndex();
    }

    void VisibilityStateSettings::load()
    {
        visibilityOptions_->setCurrentIndex(visibilityOptionsIndex_);
    }

    ObjectPropertyStateSettings *VisibilityStateSettings::clone() const
    {
        return new VisibilityStateSettings(*this);
    }

    ObjectPropertyStateSettings::GesturePtr VisibilityStateSettings::toGesture(ObjectWrapper object)
    {
        return 0;
    }

    PositionStateSettings::PositionStateSettings(QWidget *parent):
        ObjectPropertyStateSettings(QObject::tr("position"), parent),
        relationIndex_(0),
        positionXValue_(0),
        positionYValue_(0)
    {
        QHBoxLayout* layout = new QHBoxLayout(widget_);
        layout->setContentsMargins(0,0,0,0);

        relation_ = new QComboBox(widget_);
        relation_->addItem(QObject::tr("over"));
        relation_->addItem(QObject::tr("under"));
        relation_->addItem(QObject::tr("to the left of"));
        relation_->addItem(QObject::tr("to the right of"));

        positionX_ = new QSpinBox(widget_);
        positionX_->setMaximum(100);
        positionX_->setSuffix(QObject::tr(" %"));

        positionY_ = new QSpinBox(widget_);
        positionY_->setMaximum(100);
        positionY_->setSuffix(QObject::tr(" %"));

        layout->addWidget(new QLabel(QObject::tr("is"), widget_));
        layout->addWidget(relation_);
        layout->addWidget(new QLabel(QObject::tr("x:"), widget_));
        layout->addWidget(positionX_);
        layout->addWidget(new QLabel(QObject::tr("y:"), widget_));
        layout->addWidget(positionY_);
    }

    void PositionStateSettings::reset()
    {
        relation_->setCurrentIndex(0);
        positionX_->setValue(0);
        positionY_->setValue(0);
    }

    void PositionStateSettings::save()
    {
        relationIndex_ = relation_->currentIndex();
        positionXValue_ = positionX_->value();
        positionYValue_ = positionY_->value();
    }

    void PositionStateSettings::load()
    {
        relation_->setCurrentIndex(relationIndex_);
        positionX_->setValue(positionXValue_);
        positionY_->setValue(positionYValue_);
    }

    ObjectPropertyStateSettings *PositionStateSettings::clone() const
    {
        return new PositionStateSettings(*this);
    }

    ObjectPropertyStateSettings::GesturePtr PositionStateSettings::toGesture(ObjectWrapper object)
    {
        return 0;
    }

    AngleStateSettings::AngleStateSettings(QWidget *parent):
        ObjectPropertyStateSettings(QObject::tr("angle"), parent),
        relationOptionsIndex_(0),
        angleValue_(0)
    {
        QHBoxLayout* layout = new QHBoxLayout(widget_);
        layout->setContentsMargins(0,0,0,0);

        relationOptions_ = new QComboBox(widget_);

        relations_.push_back(std::equal_to<PropertyReturnType>());
        relationOptions_->addItem(QObject::tr("="));

        relations_.push_back(std::less<PropertyReturnType>());
        relationOptions_->addItem(QObject::tr("<"));

        relations_.push_back(std::less_equal<PropertyReturnType>());
        relationOptions_->addItem(QObject::tr("<="));

        relations_.push_back(std::greater<PropertyReturnType>());
        relationOptions_->addItem(QObject::tr(">"));

        relations_.push_back(std::greater_equal<PropertyReturnType>());
        relationOptions_->addItem(QObject::tr(">="));

        relations_.push_back(std::not_equal_to<PropertyReturnType>());
        relationOptions_->addItem(QObject::tr("!="));

        angle_ = new QSpinBox(widget_);
        angle_->setMaximum(179);

        layout->addWidget(relationOptions_);
        layout->addWidget(angle_);
    }

    void AngleStateSettings::reset()
    {
        relationOptions_->setCurrentIndex(0);
        angle_->setValue(0);
    }

    void AngleStateSettings::save()
    {
        relationOptionsIndex_ = relationOptions_->currentIndex();
        angleValue_ = angle_->value();
    }

    void AngleStateSettings::load()
    {
        relationOptions_->setCurrentIndex(relationOptionsIndex_);
        angle_->setValue(angleValue_);
    }

    ObjectPropertyStateSettings *AngleStateSettings::clone() const
    {
        return new AngleStateSettings(*this);
    }

    ObjectPropertyStateSettings::GesturePtr AngleStateSettings::toGesture(ObjectWrapper object)
    {
        //return makeGestureStateGesture(object, &Object::angle, );
        return 0;
    }
} // namespace Gecon
