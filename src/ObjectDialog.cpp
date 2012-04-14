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

#include "ObjectDialog.hpp"
#include "ui_ObjectDialog.hpp"

#include <QPainter>
#include <QMessageBox>
#include <QtConcurrentRun>
#include <QtTest/QTest>

#include "ObjectModel.hpp"

namespace Gecon
{
    ObjectDialog::ObjectDialog(ObjectModel* objectModel, QWidget *parent):
        QDialog(parent),
        colorGrabbed_(false),
        objectModel_(objectModel),
        ui_(new Ui::ObjectDialog)
    {
        ui_->setupUi(this);

        connect(this, SIGNAL(finished(int)), this, SLOT(stopCapture()));
        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(addObject()));

        connect(control_.signaler().get(), SIGNAL(objectsRecognized(Image,Image,ObjectSet)), this, SLOT(displayImage(Image,Image,ObjectSet)), Qt::BlockingQueuedConnection);
        connect(ui_->display, SIGNAL(imageDisplayed()), this, SLOT(firstImageDisplayed()));
    }

    ObjectDialog::~ObjectDialog()
    {
        delete ui_;
    }

    int ObjectDialog::exec()
    {
        return QDialog::Rejected;
    }

    int ObjectDialog::newObject()
    {
        reset();

        startCapture();

        return QDialog::exec();
    }

    void ObjectDialog::addObject()
    {
        if(! colorGrabbed_)
        {
            QMessageBox::critical(this, "Color object error", "No object selected!", QMessageBox::Ok);
            return;
        }

        QString name = ui_->objectName->text();
        if(name.isEmpty())
        {
            name = "Object";
        }

        try
        {
            objectModel_->addObject(name, objectColor_);

            accept();
        }
        catch(...)
        {
            // TODO
        }
    }

    void ObjectDialog::setDevice(ObjectDialog::DeviceAdapter device)
    {
        control_.setDevice(device);
    }

    void ObjectDialog::reset()
    {
        colorGrabbed_ = false;
        ui_->objectName->setText(QString());
    }

    void ObjectDialog::startCapture()
    {
        /*capture_.reset();
        capture_.setDevice(device);

        capture_.start();
        capture_.captureImage();
        capture_.wait();*/
        ui_->display->reset();

        control_.start();
    }

    void ObjectDialog::stopCapture()
    {
        /*captureTimer_.stop();
        capture_.stop();*/

        qDebug("before stop");
        QtConcurrent::run(&control_, &ControlInfo::Control::stop);
        qDebug("after stop");
    }

    void ObjectDialog::displayImage(Image original, Image segmented, ObjectSet objects)
    {
        rawImage_ = original;
        QImage img = QImage((const uchar*)&(original.rawData()[0]), original.width(), original.height(), original.width() * 3, QImage::Format_RGB888);

        if(colorGrabbed_ && object_.isVisible())
        {
            QPainter painter(&img);

            // paint object's border
            const Object::Border& border = object_.border();

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

            // paint object's convex hull
            const Object::ConvexHull& convexHull = object_.convexHull();

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
            const Object::BoundingBox& boundingBox = object_.boundingBox();

            painter.save();
            painter.setPen(Qt::red);

            painter.translate(boundingBox.position.x, boundingBox.position.y);
            painter.rotate(-(boundingBox.angle));
            painter.translate(-boundingBox.width / 2.0, -boundingBox.height / 2.0);

            painter.drawRect(0, 0, boundingBox.width, boundingBox.height);

            painter.restore();
        }

        ui_->display->displayImage(img);
    }

    void ObjectDialog::firstImageDisplayed()
    {
        disconnect(ui_->display, SIGNAL(imageDisplayed()), this, SLOT(firstImageDisplayed()));
        connect(ui_->display, SIGNAL(clicked(QMouseEvent*)), this, SLOT(grabColor(QMouseEvent*)));
    }

    void ObjectDialog::grabColor(QMouseEvent *event)
    {
        objectColor_ = rawImage_.at(event->pos().x(), event->pos().y());

        ControlInfo::ObjectPolicy::ObjectSet objects;
        object_ = Object(objectColor_);
        objects.insert(&object_);
        control_.prepareObjects(objects);

        /*ControlInfo::GesturePolicy::GestureSet gestures;
        std::function<bool(const bool&)> f =
                std::bind(std::equal_to<bool>(), std::placeholders::_1, true);
        gesture_ = ObjectStateGesture<Object, bool>(&object_, &Object::isVisible, f);
        gestures.insert(&gesture_);
        control_.prepareGestures(gestures);

        gesture_.stateEnterEvent().connect([&](const decltype(gesture_)::Event*){reject();});*/

        QtConcurrent::run(&control_, &ControlInfo::Control::restart);
        //control_.restart();

        colorGrabbed_ = true;
    }
} // namespace Gecon
