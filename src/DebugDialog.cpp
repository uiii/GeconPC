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

#include "DebugDialog.hpp"
#include "ui_DebugDialog.hpp"

#include <QFormLayout>

#include <Gecon/ColorObjectPolicy.hpp>
#include <Gecon/ObjectMotionGesture.hpp>
#include "ObjectWrapper.hpp"

namespace Gecon
{
    DebugDialog::DebugDialog(QWidget *parent) :
        QDialog(parent),
        ui_(new Ui::DebugDialog)
    {
        ui_->setupUi(this);

        motionTimeout_ = new QSpinBox;
        minimalGestureSide_ = new QSpinBox;
        minimalMotionSize_ = new QSpinBox;
        maximalSameGestureDistance_ = new QSpinBox;
        minimalObjectSizeFraction_ = new QSpinBox;

        motionTimeout_->setMaximum(10000);
        minimalGestureSide_->setMaximum(10000);
        minimalMotionSize_->setMaximum(10000);
        maximalSameGestureDistance_->setMaximum(10000);
        minimalObjectSizeFraction_->setMaximum(10000);

        QFormLayout* layout = new QFormLayout(ui_->configVariables);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addRow(tr("motionTimeout"),motionTimeout_);
        layout->addRow(tr("minimalGestureSide"),minimalGestureSide_);
        layout->addRow(tr("minimalMotionSize"),minimalMotionSize_);
        layout->addRow(tr("maximalSameGestureDistance"),maximalSameGestureDistance_);
        layout->addRow(tr("minimalObjectSizeFraction"),minimalObjectSizeFraction_);

        motionTimeout_->setValue(ObjectMotionGesture<ObjectWrapper::RawObject>::MOTION_TIMEOUT);
        minimalGestureSide_->setValue(ObjectMotionGesture<ObjectWrapper::RawObject>::MINIMAL_GESTURE_SIDE);
        minimalMotionSize_->setValue(ObjectMotionGesture<ObjectWrapper::RawObject>::NOT_MOTION_TOLERANCE);
        maximalSameGestureDistance_->setValue(ObjectMotionGesture<ObjectWrapper::RawObject>::MAXIMAL_SAME_GESTURE_DISTANCE);
        minimalObjectSizeFraction_->setValue(ColorObjectPolicy::MINIMAL_OBJECT_SIZE_FRACTION);

        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(apply()));
    }
    
    DebugDialog::~DebugDialog()
    {
        delete ui_;
    }

    void DebugDialog::apply()
    {
        qDebug("apply");
        ObjectMotionGesture<ObjectWrapper::RawObject>::MOTION_TIMEOUT = motionTimeout_->value();
        ObjectMotionGesture<ObjectWrapper::RawObject>::MINIMAL_GESTURE_SIDE = minimalGestureSide_->value();
        ObjectMotionGesture<ObjectWrapper::RawObject>::NOT_MOTION_TOLERANCE = minimalMotionSize_->value();
        ObjectMotionGesture<ObjectWrapper::RawObject>::MAXIMAL_SAME_GESTURE_DISTANCE = maximalSameGestureDistance_->value();
        ColorObjectPolicy::MINIMAL_OBJECT_SIZE_FRACTION = minimalObjectSizeFraction_->value();
        std::cout << ColorObjectPolicy::MINIMAL_OBJECT_SIZE_FRACTION << std::endl;
    }
} // namespace Gecon
