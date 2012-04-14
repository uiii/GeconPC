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

#ifndef GECON_OBJECTDIALOG_HPP
#define GECON_OBJECTDIALOG_HPP

#include <QDialog>
#include <QTimer>
#include <QThread>
#include <QPainter>

#include "ControlInfo.hpp"

#include <Gecon/ObjectStateGesture.hpp>

namespace Gecon
{
    namespace Ui
    {
        class ObjectDialog;
    }

    class ObjectModel; // forward declaration

    class ObjectDialog : public QDialog
    {
        Q_OBJECT

    public:
        typedef ControlInfo::ObjectPolicy::Object Object;
        typedef ControlInfo::GesturePolicy::Gesture Gesture;
        typedef ControlInfo::DevicePolicy::DeviceAdapter DeviceAdapter;
        typedef ControlInfo::DevicePolicy::Snapshot Snapshot;
        typedef Gecon::Image<RGB> Image;
        typedef ControlInfo::ObjectPolicy::ObjectSet ObjectSet;

        explicit ObjectDialog(ObjectModel* objectModel, QWidget *parent = 0);
        ~ObjectDialog();

    public slots:
        int newObject();

        void addObject();

        void startCapture();
        void stopCapture();

        void displayImage(Image original, Image segmented, ObjectSet objects);
        void firstImageDisplayed();

        void grabColor(QMouseEvent* event);

        void setDevice(DeviceAdapter device);

        void reset();

        int exec();

    private:
        ControlInfo::Control control_;

        Snapshot rawImage_;

        Object::Color objectColor_;
        bool colorGrabbed_;

        Object object_;
        ObjectStateGesture<Object, bool> gesture_;

        ObjectModel* objectModel_;

        Ui::ObjectDialog *ui_;
    };
} // namespace Gecon
#endif // GECON_OBJECTDIALOG_HPP
