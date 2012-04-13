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

#ifndef GECON_MOTIONGESTUREDIALOG_HPP
#define GECON_MOTIONGESTUREDIALOG_HPP

#include <QDialog>
#include <QTimer>

#include "Capture.hpp"

namespace Gecon
{
    namespace Ui
    {
        class MotionGestureDialog;
    }
    
    class MotionGestureDialog : public QDialog
    {
        Q_OBJECT
        
    public:
        typedef ControlInfo::DevicePolicy::DeviceAdapter DeviceAdapter;

        explicit MotionGestureDialog(QWidget *parent = 0);
        ~MotionGestureDialog();

    public slots:
        void startCapture();
        void stopCapture();

        void displayImage();

        void countdown();

        int newGesture(DeviceAdapter device);

        int exec();
        
    private:
        Capture capture_;

        QTimer captureTimer_;

        QTimer countdownTimer_;
        int countdownCount_;

        Ui::MotionGestureDialog *ui_;
    };
} // namespace Gecon
#endif // GECON_MOTIONGESTUREDIALOG_HPP
