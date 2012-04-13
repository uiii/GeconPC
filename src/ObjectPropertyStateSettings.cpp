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
    ObjectPropertyStateSettings::ObjectPropertyStateSettings(const QString& propertyName, QWidget* widget):
        widget_(widget),
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

    VisibilityStateSettingsWidget::VisibilityStateSettingsWidget(QWidget *parent):
        QWidget(parent)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0,0,0,0);

        visibilityOptions = new QComboBox(this);
        visibilityOptions->addItem(QObject::tr("visible"));
        visibilityOptions->addItem(QObject::tr("hidden"));

        QLabel* equality = new QLabel(QObject::tr("="), this);
        equality->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

        layout->addWidget(equality);
        layout->addWidget(visibilityOptions);
    }

    VisibilityStateSettings::VisibilityStateSettings(QWidget* parent):
        ObjectPropertyStateSettings("visibility", new VisibilityStateSettingsWidget(parent)),
        visibilityOptionsIndex_(0),
        widget_(dynamic_cast<VisibilityStateSettingsWidget*>(widget()))
    {
    }

    void VisibilityStateSettings::reset()
    {
        widget_->visibilityOptions->setCurrentIndex(0);
    }

    void VisibilityStateSettings::save()
    {
        visibilityOptionsIndex_ = widget_->visibilityOptions->currentIndex();
    }

    void VisibilityStateSettings::load()
    {
        widget_->visibilityOptions->setCurrentIndex(visibilityOptionsIndex_);
    }

    ObjectPropertyStateSettings *VisibilityStateSettings::clone() const
    {
        return new VisibilityStateSettings(*this);
    }

    ObjectPropertyStateSettings::GesturePtr VisibilityStateSettings::toGesture(ObjectWrapper object)
    {
        return 0;
    }

    PositionStateSettingsWidget::PositionStateSettingsWidget(QWidget *parent):
        QWidget(parent)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0,0,0,0);

        relation = new QComboBox(this);
        relation->addItem(QObject::tr("over"));
        relation->addItem(QObject::tr("under"));
        relation->addItem(QObject::tr("to the left of"));
        relation->addItem(QObject::tr("to the right of"));
        relation->addItem(QObject::tr("closer than"));
        relation->addItem(QObject::tr("farther than"));

        distance = new QSpinBox(this);
        distance->setMaximum(100);
        distance->setSuffix(QObject::tr(" %"));
        distance->setVisible(false);

        from = new QLabel(QObject::tr("from"), this);
        from->setVisible(false);

        positionX = new QSpinBox(this);
        positionX->setMaximum(100);
        positionX->setSuffix(QObject::tr(" %"));

        positionY = new QSpinBox(this);
        positionY->setMaximum(100);
        positionY->setSuffix(QObject::tr(" %"));

        layout->addWidget(new QLabel(QObject::tr("is"), this));
        layout->addWidget(relation);
        layout->addWidget(distance);
        layout->addWidget(from);
        layout->addWidget(new QLabel(QObject::tr("x:"), this));
        layout->addWidget(positionX);
        layout->addWidget(new QLabel(QObject::tr("y:"), this));
        layout->addWidget(positionY);

        connect(relation, SIGNAL(currentIndexChanged(int)), this, SLOT(relationChanged()));
    }

    void PositionStateSettingsWidget::relationChanged()
    {
        if(relation->currentText() == "closer than" || relation->currentText() == "farther than")
        {
            distance->setVisible(true);
            from->setVisible(true);
        }
        else
        {
            distance->setVisible(false);
            from->setVisible(false);
        }
    }

    PositionStateSettings::PositionStateSettings(QWidget *parent):
        ObjectPropertyStateSettings(QObject::tr("position"), new PositionStateSettingsWidget(parent)),
        relationIndex_(0),
        positionXValue_(0),
        positionYValue_(0),
        widget_(dynamic_cast<PositionStateSettingsWidget*>(widget()))
    {
    }

    void PositionStateSettings::reset()
    {
        widget_->relation->setCurrentIndex(0);
        widget_->distance->setValue(0);
        widget_->positionX->setValue(0);
        widget_->positionY->setValue(0);
    }

    void PositionStateSettings::save()
    {
        relationIndex_ = widget_->relation->currentIndex();
        distance_ = widget_->distance->value();
        positionXValue_ = widget_->positionX->value();
        positionYValue_ = widget_->positionY->value();
    }

    void PositionStateSettings::load()
    {
        widget_->relation->setCurrentIndex(relationIndex_);
        widget_->distance->setValue(distance_);
        widget_->positionX->setValue(positionXValue_);
        widget_->positionY->setValue(positionYValue_);
    }

    ObjectPropertyStateSettings *PositionStateSettings::clone() const
    {
        return new PositionStateSettings(*this);
    }

    ObjectPropertyStateSettings::GesturePtr PositionStateSettings::toGesture(ObjectWrapper object)
    {
        return 0;
    }

    AngleStateSettingsWidget::AngleStateSettingsWidget(QWidget *parent)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0,0,0,0);

        relationOptions = new QComboBox(this);
        relationOptions->addItem(QObject::tr("="));
        relationOptions->addItem(QObject::tr("<"));
        relationOptions->addItem(QObject::tr("<="));
        relationOptions->addItem(QObject::tr(">"));
        relationOptions->addItem(QObject::tr(">="));
        relationOptions->addItem(QObject::tr("!="));

        angle = new QSpinBox(this);
        angle->setMaximum(179);

        layout->addWidget(relationOptions);
        layout->addWidget(angle);
    }

    AngleStateSettings::AngleStateSettings(QWidget *parent):
        ObjectPropertyStateSettings(QObject::tr("angle"), new AngleStateSettingsWidget(parent)),
        relationOptionsIndex_(0),
        angleValue_(0),
        widget_(dynamic_cast<AngleStateSettingsWidget*>(widget()))
    {
        relations_.push_back(std::equal_to<PropertyReturnType>());
        relations_.push_back(std::less<PropertyReturnType>());
        relations_.push_back(std::less_equal<PropertyReturnType>());
        relations_.push_back(std::greater<PropertyReturnType>());
        relations_.push_back(std::greater_equal<PropertyReturnType>());
        relations_.push_back(std::not_equal_to<PropertyReturnType>());
    }

    void AngleStateSettings::reset()
    {
        widget_->relationOptions->setCurrentIndex(0);
        widget_->angle->setValue(0);
    }

    void AngleStateSettings::save()
    {
        relationOptionsIndex_ = widget_->relationOptions->currentIndex();
        angleValue_ = widget_->angle->value();
    }

    void AngleStateSettings::load()
    {
        widget_->relationOptions->setCurrentIndex(relationOptionsIndex_);
        widget_->angle->setValue(angleValue_);
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
