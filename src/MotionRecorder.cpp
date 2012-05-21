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

#include "MotionRecorder.hpp"

namespace Gecon
{
    MotionRecorder::MotionRecorder(ControlInfo::Object* object):
        ControlInfo::MotionGesture(object, Motion(), new ControlInfo::MotionGesture::MotionStorage),
        signaler_(std::make_shared<MotionRecorderSignaler>())
    {
    }

    MotionRecorder::~MotionRecorder()
    {
        delete motionStorage_;
    }

    MotionRecorder::Events MotionRecorder::check()
    {
        Events events = ControlInfo::MotionGesture::check();

        signaler_->emitMotionUpdated((*motionStorage_)[object_].motion);

        return events;
    }

    const std::shared_ptr<MotionRecorderSignaler>& MotionRecorder::signaler() const
    {
        return signaler_;
    }

    void MotionRecorder::processRecord_(MotionRecord& record, const Size& motionSize)
    {
        std::cout << "emit motion" << std::endl;

        ControlInfo::MotionGesture::processRecord_(record, motionSize);
        signaler_->emitMotionRecorded(record.motion, record.moves);
    }

    void MotionRecorderSignaler::emitMotionUpdated(const MotionRecorder::Motion& motion)
    {
        emit motionUpdated(motion);
    }

    void MotionRecorderSignaler::emitMotionRecorded(const MotionRecorder::Motion& motion, const MotionRecorder::MoveSequence& moves)
    {
        emit motionRecorded(motion, moves);
    }
} // namespace Gecon
