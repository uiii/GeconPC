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

#ifndef GECON_NEWOBJECTDIALOG_HPP
#define GECON_NEWOBJECTDIALOG_HPP

#include <QDialog>
#include <QTimer>
#include <QThread>
#include <QPainter>

#include "ControlInfo.hpp"
#include "Capture.hpp"

namespace Gecon
{
    namespace Ui
    {
        class NewObjectDialog;
    }

    class NewObjectDialog : public QDialog
    {
        Q_OBJECT

    public:
        typedef ControlInfo::ObjectPolicy::Object Object;
        typedef ControlInfo::DevicePolicy::Snapshot Snapshot;

        explicit NewObjectDialog(ControlInfo::Control* control, QWidget *parent = 0);
        ~NewObjectDialog();

        Object* object() const;

    public slots:
        void startCapture();
        void stopCapture();

        void displayImage();

        void grabObject(QMouseEvent* event);

        int exec();

        void newObject();

    private:
        Capture capture_;
        QTimer captureTimer_;

        Snapshot rawImage_;

        ControlInfo::ObjectPolicy::ObjectPtr object_;

        ControlInfo::Control* control_;

        Ui::NewObjectDialog *ui_;
    };
} // namespace Gecon
#endif // GECON_NEWOBJECTDIALOG_HPP
