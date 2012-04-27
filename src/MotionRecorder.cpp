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
    MotionRecorder::MotionRecorder(Object* object):
        ObjectMotionGesture<Object>(object, Motion()),
        signaler_(std::make_shared<MotionRecorderSignaler>())
    {
    }

    MotionRecorder::Events MotionRecorder::check()
    {
        Events events = ObjectMotionGesture<Object>::check();

        signaler_->emitMotionUpdated(recordedMotion_);

        return events;
    }

    const std::shared_ptr<MotionRecorderSignaler>& MotionRecorder::signaler() const
    {
        return signaler_;
    }

    bool MotionRecorder::checkMotion_(const Motion& motion)
    {
        std::cout << "emit motion" << std::endl;

        MoveSequence moves;
        motionToMoves_(motion, moves);

        signaler_->emitMotionRecorded(motion, moves);

        return true;
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
