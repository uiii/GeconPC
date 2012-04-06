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

#ifndef GECON_CAPTURE_HPP
#define GECON_CAPTURE_HPP

#include <QThread>
#include <QImage>

#include "ControlInfo.hpp"

namespace Gecon
{
    class Capture : public QThread
    {
        Q_OBJECT

    public:
        typedef ControlInfo::DevicePolicy::Snapshot Snapshot;

        typedef ControlInfo::ObjectPolicy::Object Object;
        typedef ControlInfo::ObjectPolicy::ObjectPtr ObjectPtr;
        typedef ControlInfo::ObjectPolicy::ObjectSet ObjectSet;

        Capture();

        void setDevice(ControlInfo::DevicePolicy::DeviceAdapter device);
        void setObject(ObjectPtr object);

        const QImage& image() const;
        const Snapshot& rawImage() const;

        void run();

    public slots:
        void start();
        void stop();

        void captureImage();

    private slots:
        void afterRun();

    private:
        Snapshot raw_;
        QImage img_;

        ObjectPtr object_;

        bool stop_;

        ControlInfo::DevicePolicy::DeviceAdapter device_;
        ControlInfo::ObjectPolicy objectPolicy_;
    };
} // namespace Gecon

#endif // GECON_CAPTURE_HPP
