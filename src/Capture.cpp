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

#include "Capture.hpp"

#include <QPainter>

namespace Gecon
{
    Capture::Capture():
        object_(0),
        stop_(true)
    {
        connect(this, SIGNAL(finished()), this, SLOT(afterRun()));
    }

    void Capture::setDevice(ControlInfo::DevicePolicy::DeviceAdapter device)
    {
        device_ = device;
    }

    void Capture::setObjectColor(Object::Color color)
    {
        object_ = new Object(color);
    }

    void Capture::reset()
    {
        object_ = 0;
    }

    const QImage& Capture::image() const
    {
        return img_;
    }

    const Capture::Snapshot& Capture::rawImage() const
    {
        return raw_;
    }

    void Capture::run()
    {
        ObjectSet objects;
        if(object_)
        {
            objects.insert(object_);
        }

        objectPolicy_.prepareObjects(objects);

        raw_ = device_.getSnapshot();
        objectPolicy_.recognizeObjects(raw_);

        img_ = QImage((const uchar*)&(raw_.rawData()[0]), raw_.width(), raw_.height(), raw_.width() * 3, QImage::Format_RGB888);

        if(object_ && object_->isVisible())
        {
            QPainter painter(&img_);

            // paint object's border
            const Object::Border& border = object_->border();

            QPolygon borderPolygon;
            for(const Object::Point& point : border)
            {
                borderPolygon << QPoint(point.x, point.y);
            }

            painter.save();
            painter.setPen(Qt::blue);
            painter.setBrush(QBrush(Qt::blue, Qt::DiagCrossPattern));
            painter.setBrush(QBrush(Qt::blue, Qt::Dense6Pattern));
            painter.drawPolygon(borderPolygon);
            painter.restore();

            /*
            // paint object's convex hull
            const Object::ConvexHull& convexHull = object_->convexHull();

            QPolygon convexHullPolygon;
            for(const Object::Point& point : convexHull)
            {
                convexHullPolygon << QPoint(point.x, point.y);
            }

            painter.save();
            painter.setPen(Qt::green);
            painter.drawPolygon(convexHullPolygon);
            painter.restore();

            // paint object's bounding box
            const Object::BoundingBox& boundingBox = object_->boundingBox();

            painter.save();
            painter.setPen(Qt::red);

            painter.translate(boundingBox.position.x, boundingBox.position.y);
            painter.rotate(-(boundingBox.angle));
            painter.translate(-boundingBox.width / 2.0, -boundingBox.height / 2.0);

            painter.drawRect(0, 0, boundingBox.width, boundingBox.height);

            painter.restore();
            */
        }
    }

    void Capture::start()
    {
        device_.open();
        stop_ = false;
    }

    void Capture::stop()
    {
        stop_ = true;

        if(isFinished())
        {
            afterRun();
        }
    }

    void Capture::captureImage()
    {
        QThread::start();
    }

    void Capture::afterRun()
    {
        if(stop_)
        {
            device_.close();
        }
    }
} // namespace Gecon
