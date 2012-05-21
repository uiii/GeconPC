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

#include "PropertyStateSettings.hpp"

#include <QHBoxLayout>
#include <QLabel>

#include <Gecon/ObjectStateGesture.hpp>

namespace Gecon
{
    ObjectStateSettings::ObjectStateSettings(const QString& propertyName, QWidget* widget):
        widget_(widget),
        propertyName_(propertyName)
    {
    }

    ObjectStateSettings::~ObjectStateSettings()
    {
    }

    const QString& ObjectStateSettings::propertyName() const
    {
        return propertyName_;
    }

    QWidget* PropertyStateSetting::widget() const
    {
        return widget_;
    }

    VisibilityStateSettingsWidget::VisibilityStateSettingsWidget(QWidget *parent):
        QWidget(parent)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0,0,0,0);

        visibilityOptions = new QComboBox(this);
        visibilityOptions->addItem(tr("visible"), true);
        visibilityOptions->addItem(tr("hidden"), false);

        QLabel* equality = new QLabel(tr("="), this);
        equality->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

        layout->addWidget(equality);
        layout->addWidget(visibilityOptions);
    }

    VisibilityStateSettings::VisibilityStateSettings(QWidget* parent):
        PropertyStateSettings("visibility", new VisibilityStateSettingsWidget(parent)),
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

    ObjectStateSettings *VisibilityStateSettings::clone() const
    {
        return new VisibilityStateSettings(*this);
    }

    ControlInfo::StateGesture* VisibilityStateSettings::toGesture(ObjectWrapper* object)
    {
        bool value = widget_->visibilityOptions->itemData(visibilityOptionsIndex_).value<bool>();

        return new RawGesture(
            object->rawObject(),
            &ObjectWrapper::RawObject::isVisible,
            RawGesture::Relation<bool>(std::equal_to<bool>()),
            value
        );
    }

    PositionStateSettingsWidget::PositionStateSettingsWidget(QWidget *parent):
        QWidget(parent)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0,0,0,0);

        relation = new QComboBox(this);
        relation->addItem(tr("over"));
        relation->addItem(tr("under"));
        relation->addItem(tr("on the left of"));
        relation->addItem(tr("on the right of"));
        relation->addItem(tr("closer than"));
        relation->addItem(tr("farther than"));

        distance = new QSpinBox(this);
        distance->setMaximum(100);
        distance->setSuffix(tr(" %"));
        distance->setVisible(false);

        from = new QLabel(tr("from"), this);
        from->setVisible(false);

        positionX = new QSpinBox(this);
        positionX->setMaximum(100);
        positionX->setSuffix(tr(" %"));

        positionY = new QSpinBox(this);
        positionY->setMaximum(100);
        positionY->setSuffix(tr(" %"));

        layout->addWidget(new QLabel(tr("is"), this));
        layout->addWidget(relation);
        layout->addWidget(distance);
        layout->addWidget(from);
        layout->addWidget(new QLabel(tr("x:"), this));
        layout->addWidget(positionX);
        layout->addWidget(new QLabel(tr("y:"), this));
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
        PropertyStateSettings(QObject::tr("position"), new PositionStateSettingsWidget(parent)),
        relationIndex_(0),
        positionXValue_(0),
        positionYValue_(0),
        widget_(dynamic_cast<PositionStateSettingsWidget*>(widget()))
    {
        relations_.push_back(&isOver);
        relations_.push_back(&isUnder);
        relations_.push_back(&isOnTheLeftOf);
        relations_.push_back(&isOnTheRightOf);
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

    ObjectStateSettings *PositionStateSettings::clone() const
    {
        return new PositionStateSettings(*this);
    }

    ControlInfo::StateGesture* PositionStateSettings::toGesture(ObjectWrapper* object)
    {
        RelationType relation = relations_.at(relationIndex_);
        if(relationIndex_ == 4)
        {
            relation = std::bind(
                std::less<int>(),
                std::bind(&distance, std::placeholders::_1, std::placeholders::_2),
                distance_
            );
        }
        else if(relationIndex_ == 5)
        {
            relation = std::bind(
                std::greater<int>(),
                std::bind(&distance, std::placeholders::_1, std::placeholders::_2),
                distance_
            );
        }

        return new RawGesture(
            object->rawObject(),
            &ObjectWrapper::RawObject::position,
            relation,
            PropertyReturnType{(double)positionXValue_, (double)positionYValue_}
        );
    }

    AngleStateSettingsWidget::AngleStateSettingsWidget(QWidget *parent)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0,0,0,0);

        relation = new QComboBox(this);
        relation->addItem(tr("="));
        relation->addItem(tr("!="));
        relation->addItem(tr("<"));
        relation->addItem(tr(">"));

        angle = new QSpinBox(this);
        angle->setMaximum(179);

        layout->addWidget(relation);
        layout->addWidget(angle);
    }

    AngleStateSettings::AngleStateSettings(QWidget *parent):
        PropertyStateSettings(QObject::tr("angle"), new AngleStateSettingsWidget(parent)),
        relationIndex_(0),
        angleValue_(0),
        widget_(dynamic_cast<AngleStateSettingsWidget*>(widget()))
    {
        relations_.push_back(std::equal_to<PropertyReturnType>());
        relations_.push_back(std::not_equal_to<PropertyReturnType>());
        relations_.push_back(std::less<PropertyReturnType>());
        relations_.push_back(std::greater<PropertyReturnType>());
    }

    void AngleStateSettings::reset()
    {
        widget_->relation->setCurrentIndex(0);
        widget_->angle->setValue(0);
    }

    void AngleStateSettings::save()
    {
        relationIndex_ = widget_->relation->currentIndex();
        angleValue_ = widget_->angle->value();
    }

    void AngleStateSettings::load()
    {
        widget_->relation->setCurrentIndex(relationIndex_);
        widget_->angle->setValue(angleValue_);
    }

    ObjectStateSettings *AngleStateSettings::clone() const
    {
        return new AngleStateSettings(*this);
    }

    ObjectStateSettings::RawGesture* AngleStateSettings::toGesture(ObjectWrapper* object)
    {
        return new RawGesture(
            object->rawObject(),
            &ObjectWrapper::RawObject::angle,
            relations_.at(relationIndex_),
            angleValue_
        );
    }

    AreaSizeStateSettingsWidget::AreaSizeStateSettingsWidget(QWidget *parent)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0,0,0,0);

        relation = new QComboBox(this);
        relation->addItem(tr("="));
        relation->addItem(tr("!="));
        relation->addItem(tr("<"));
        relation->addItem(tr(">"));

        area = new QSpinBox(this);
        area->setMaximum(179);

        layout->addWidget(relation);
        layout->addWidget(area);
    }

    AreaSizeStateSettings::AreaSizeStateSettings(QWidget *parent):
        PropertyStateSettings(QObject::tr("area"), new AreaSizeStateSettingsWidget(parent)),
        relationIndex_(0),
        areaValue_(0),
        widget_(dynamic_cast<AreaSizeStateSettingsWidget*>(widget()))
    {
        relations_.push_back(std::equal_to<PropertyReturnType>());
        relations_.push_back(std::not_equal_to<PropertyReturnType>());
        relations_.push_back(std::less<PropertyReturnType>());
        relations_.push_back(std::greater<PropertyReturnType>());
    }

    void AreaSizeStateSettings::reset()
    {
        widget_->relation->setCurrentIndex(0);
        widget_->area->setValue(0);
    }

    void AreaSizeStateSettings::save()
    {
        relationIndex_ = widget_->relation->currentIndex();
        areaValue_ = widget_->area->value();
    }

    void AreaSizeStateSettings::load()
    {
        widget_->relation->setCurrentIndex(relationIndex_);
        widget_->area->setValue(areaValue_);
    }

    ObjectStateSettings *AreaSizeStateSettings::clone() const
    {
        return new AreaSizeStateSettings(*this);
    }

    ObjectStateSettings::RawGesture* AreaSizeStateSettings::toGesture(ObjectWrapper* object)
    {
        return new RawGesture(
            object->rawObject(),
            &ObjectWrapper::RawObject::area,
            relations_.at(relationIndex_),
            areaValue_
        );
    }

    AspectRatioStateSettingsWidget::AspectRatioStateSettingsWidget(QWidget *parent)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0,0,0,0);

        relation = new QComboBox(this);
        relation->addItem(tr("="));
        relation->addItem(tr("!="));
        relation->addItem(tr("<"));
        relation->addItem(tr(">"));

        aspectRatio = new QDoubleSpinBox(this);
        aspectRatio->setMaximum(1000.0);
        aspectRatio->setDecimals(2);
        aspectRatio->setSingleStep(0.1);

        layout->addWidget(relation);
        layout->addWidget(aspectRatio);
    }

    AspectRatioStateSettings::AspectRatioStateSettings(QWidget *parent):
        PropertyStateSettings(QObject::tr("aspect ratio"), new AspectRatioStateSettingsWidget(parent)),
        relationIndex_(0),
        aspectRatioValue_(0.0),
        widget_(dynamic_cast<AspectRatioStateSettingsWidget*>(widget()))
    {
        relations_.push_back(std::equal_to<PropertyReturnType>());
        relations_.push_back(std::not_equal_to<PropertyReturnType>());
        relations_.push_back(std::less<PropertyReturnType>());
        relations_.push_back(std::greater<PropertyReturnType>());
    }

    void AspectRatioStateSettings::reset()
    {
        widget_->relation->setCurrentIndex(0);
        widget_->aspectRatio->setValue(0.0);
    }

    void AspectRatioStateSettings::save()
    {
        relationIndex_ = widget_->relation->currentIndex();
        aspectRatioValue_ = widget_->aspectRatio->value();
    }

    void AspectRatioStateSettings::load()
    {
        widget_->relation->setCurrentIndex(relationIndex_);
        widget_->aspectRatio->setValue(aspectRatioValue_);
    }

    ObjectStateSettings *AspectRatioStateSettings::clone() const
    {
        return new AspectRatioStateSettings(*this);
    }

    ObjectStateSettings::RawGesture* AspectRatioStateSettings::toGesture(ObjectWrapper* object)
    {
        return new RawGesture(
            object->rawObject(),
            &ObjectWrapper::RawObject::aspectRatio,
            relations_.at(relationIndex_),
            aspectRatioValue_
        );
    }
} // namespace Gecon
