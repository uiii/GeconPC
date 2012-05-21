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
#include "DialogBase.hpp"

#include <Gecon/ObjectStateGesture.hpp>

namespace Gecon
{
    namespace Ui
    {
        class ObjectDialog;
    }

    class ObjectModel; // forward declaration
    class ObjectWrapper;

    class ObjectDialog : public DialogBase
    {
        Q_OBJECT

    public:
        typedef ControlInfo::Objects Objects;
        typedef ControlInfo::DevicePolicy::DeviceAdapter DeviceAdapter;
        typedef ControlInfo::DevicePolicy::Snapshot Snapshot;
        typedef Gecon::Image<RGB> Image;

        explicit ObjectDialog(ObjectModel* objectModel, QWidget *parent = 0);
        ~ObjectDialog();

        void setDevice(DeviceAdapter device);

    public slots:
        int newObject();
        int editObject(ObjectWrapper* object);

        int exec();

    protected slots:
        void beforeClose_();

    private slots:
        void addObject_();
        void updateObject_();
        void deleteObject_();

        void startCapture_();
        void stopCapture_();

        void displayImage_(const Image& original, const Image& segmented, const Objects& objects);
        void firstImageDisplayed_();

        void grabColor_(QPoint position);

        void reset_();

    private:
        ControlInfo::Control control_;

        Snapshot rawImage_;

        ControlInfo::Object* rawObject_;

        ObjectWrapper* editedObject_;

        ObjectModel* objectModel_;

        Ui::ObjectDialog *ui_;
    };
} // namespace Gecon
#endif // GECON_OBJECTDIALOG_HPP
