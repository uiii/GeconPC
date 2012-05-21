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

#include "ImageDisplay.hpp"

#include <QPainter>

namespace Gecon {
    
    ImageDisplay::ImageDisplay(QWidget *parent) :
        QLabel(parent)
    {
        reset();
    }

    void ImageDisplay::displayImage(const QImage &image)
    {
        setPixmap(QPixmap::fromImage(image));

        displayedImageSize_ = image.size();

        emit imageDisplayed();
    }

    void ImageDisplay::displayImage(const Image &image, const ControlInfo::Objects &objects, const Motion& motion)
    {
        QImage img = QImage((const uchar*)&(image.rawData()[0]), image.width(), image.height(), image.width() * 3, QImage::Format_RGB888);

        QPainter painter(&img);
        for(ControlInfo::Object* object : objects)
        {
            if(object->isVisible())
            {
                // paint object's border
                const ControlInfo::Object::Border& border = object->border();

                QPolygon borderPolygon;
                for(const Point& point : border)
                {
                    borderPolygon << QPoint(point.x, point.y);
                }

                painter.save();
                painter.setPen(Qt::blue);
                painter.setBrush(QBrush(Qt::blue, Qt::DiagCrossPattern));
                painter.setBrush(QBrush(Qt::blue, Qt::Dense6Pattern));
                painter.drawPolygon(borderPolygon);
                painter.restore();

                // paint object's convex hull
                const ControlInfo::Object::ConvexHull& convexHull = object->convexHull();

                QPolygon convexHullPolygon;
                for(const Point& point : convexHull)
                {
                    convexHullPolygon << QPoint(point.x, point.y);
                }

                painter.save();
                painter.setPen(Qt::green);
                painter.drawPolygon(convexHullPolygon);
                painter.restore();

                // paint object's bounding box
                const ControlInfo::Object::BoundingBox& boundingBox = object->boundingBox();

                painter.save();
                painter.setPen(Qt::red);

                painter.translate(boundingBox.position.x, boundingBox.position.y);
                painter.rotate(-(boundingBox.angle));
                painter.translate(-boundingBox.width / 2.0, -boundingBox.height / 2.0);

                painter.drawRect(0, 0, boundingBox.width, boundingBox.height);

                painter.restore();
            }
        }

        if(! motion.empty())
        {
            // paint object's motion
            QPolygon motionPolyline;
            for(const Point& point : motion)
            {
                motionPolyline << QPoint(point.x, point.y);
            }

            painter.save();
            QLinearGradient gradient;
            gradient.setColorAt(0, Qt::white);
            gradient.setColorAt(1, Qt::red);
            painter.setPen(QPen(QBrush(gradient), 2));

            painter.drawPolyline(motionPolyline);

            painter.restore();
        }

        displayImage(img);
    }

    void ImageDisplay::reset()
    {
        clear();
        setText(tr("Video"));
    }

    void ImageDisplay::mousePressEvent(QMouseEvent *ev)
    {
        QSize topLeft = (size() - displayedImageSize_) / 2.0;

        QPoint absolutePosition = ev->pos();
        QPoint relativePosition = QPoint(absolutePosition.x() - topLeft.width(), absolutePosition.y() - topLeft.height());

        QRect imageRect(QPoint(0, 0), displayedImageSize_);

        if(imageRect.contains(relativePosition))
        {
            emit clicked(relativePosition);
        }
    }
    
} // namespace Gecon
