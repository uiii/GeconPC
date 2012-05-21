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

#ifndef GECON_MOTIONRECORDER_HPP
#define GECON_MOTIONRECORDER_HPP

#include "ControlInfo.hpp"

#include <memory>

#include <QObject>

#include <Gecon/ObjectMotionGesture.hpp>

namespace Gecon
{
    class MotionRecorderSignaler; // forward declaration

    class MotionRecorder : public ControlInfo::MotionGesture
    {
    public:
        MotionRecorder(ControlInfo::Object* object = 0); // TODO!!!
        virtual ~MotionRecorder();

        Events check();

        const std::shared_ptr<MotionRecorderSignaler>& signaler() const;

    protected:
        void processRecord_(MotionRecord& record, const Size& motionSize);

    private:
        std::shared_ptr<MotionRecorderSignaler> signaler_;
    };

    class MotionRecorderSignaler : public QObject
    {
        Q_OBJECT

    public:
        void emitMotionUpdated(const MotionRecorder::Motion& motion);
        void emitMotionRecorded(const MotionRecorder::Motion& motion, const MotionRecorder::MoveSequence& moves);

    signals:
        void motionUpdated(const MotionRecorder::Motion& motion);
        void motionRecorded(const MotionRecorder::Motion& motion, const MotionRecorder::MoveSequence& moves);
    };

} // namespace Gecon

#endif // GECON_MOTIONRECORDER_HPP
