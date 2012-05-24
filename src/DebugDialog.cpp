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

#include "ObjectWrapper.hpp"

#include <Gecon/ColorObjectPolicy.hpp>
#include <Gecon/ObjectMotionGesture.hpp>

#include "ActionSettings.hpp"

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
        yRange_ = new QSpinBox;
        cbRange_ = new QSpinBox;
        crRange_ = new QSpinBox;
        mouseMotionBufferSize_ = new QSpinBox;
        sleepTime_ = new QSpinBox;

        motionTimeout_->setMaximum(10000);
        minimalGestureSide_->setMaximum(10000);
        minimalMotionSize_->setMaximum(10000);
        maximalSameGestureDistance_->setMaximum(10000);
        minimalObjectSizeFraction_->setMaximum(10000);
        yRange_->setMaximum(10000);
        cbRange_->setMaximum(10000);
        crRange_->setMaximum(10000);
        mouseMotionBufferSize_->setMaximum(10000);
        sleepTime_->setMaximum(10000);

        QFormLayout* layout = new QFormLayout(ui_->configVariables);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addRow(tr("motionTimeout"),motionTimeout_);
        layout->addRow(tr("minimalGestureSide"),minimalGestureSide_);
        layout->addRow(tr("minimalMotionSize"),minimalMotionSize_);
        layout->addRow(tr("maximalSameGestureDistance"),maximalSameGestureDistance_);
        layout->addRow(tr("minimalObjectSizeFraction"),minimalObjectSizeFraction_);
        layout->addRow(tr("Y range"),yRange_);
        layout->addRow(tr("CB range"),cbRange_);
        layout->addRow(tr("CR range"),crRange_);
        layout->addRow(tr("mouse motion buffer size"),mouseMotionBufferSize_);
        layout->addRow(tr("sleep time"), sleepTime_);

        motionTimeout_->setValue(ControlInfo::MotionGestureChecker::MOTION_TIMEOUT);
        minimalGestureSide_->setValue(ControlInfo::MotionGestureChecker::MINIMAL_GESTURE_SIDE_FRACTION);
        minimalMotionSize_->setValue(ControlInfo::MotionGestureChecker::NOT_MOTION_TOLERANCE_FRACTION);
        maximalSameGestureDistance_->setValue(ControlInfo::MotionGestureChecker::MAXIMAL_SAME_GESTURE_DISTANCE);
        minimalObjectSizeFraction_->setValue(ColorObjectPolicy::MINIMAL_OBJECT_SIZE_FRACTION);
        yRange_->setValue(ControlInfo::Objects::Y_RANGE);
        cbRange_->setValue(ControlInfo::Objects::CB_RANGE);
        crRange_->setValue(ControlInfo::Objects::CR_RANGE);
        mouseMotionBufferSize_->setValue(MouseMotionActionSettings::BUFFER_SIZE);
        sleepTime_->setValue(ControlInfo::Control::SLEEP_TIME);

        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(apply()));
    }
    
    DebugDialog::~DebugDialog()
    {
        delete ui_;
    }

    void DebugDialog::apply()
    {
        qDebug("apply");
        ControlInfo::MotionGestureChecker::MOTION_TIMEOUT = motionTimeout_->value();
        ControlInfo::MotionGestureChecker::MINIMAL_GESTURE_SIDE_FRACTION = minimalGestureSide_->value();
        ControlInfo::MotionGestureChecker::NOT_MOTION_TOLERANCE_FRACTION = minimalMotionSize_->value();
        ControlInfo::MotionGestureChecker::MAXIMAL_SAME_GESTURE_DISTANCE = maximalSameGestureDistance_->value();
        ColorObjectPolicy::MINIMAL_OBJECT_SIZE_FRACTION = minimalObjectSizeFraction_->value();
        ControlInfo::Objects::Y_RANGE = yRange_->value();
        ControlInfo::Objects::CB_RANGE = cbRange_->value();
        ControlInfo::Objects::CR_RANGE = crRange_->value();
        MouseMotionActionSettings::BUFFER_SIZE = mouseMotionBufferSize_->value();
        ControlInfo::Control::SLEEP_TIME = sleepTime_->value();
    }
} // namespace Gecon
