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

#ifndef GECON_DEBUGDIALOG_HPP
#define GECON_DEBUGDIALOG_HPP

#include <QDialog>
#include <QSpinBox>

namespace Gecon {
    
    namespace Ui {
        class DebugDialog;
    }
    
    class DebugDialog : public QDialog
    {
        Q_OBJECT
        
    public:
        explicit DebugDialog(QWidget *parent = 0);
        ~DebugDialog();

    public slots:
        void apply();
        
    private:
        QSpinBox* motionTimeout_;
        QSpinBox* minimalGestureSide_;
        QSpinBox* minimalMotionSize_;
        QSpinBox* maximalSameGestureDistance_;
        QSpinBox* minimalObjectSizeFraction_;
        QSpinBox* yRange_;
        QSpinBox* cbRange_;
        QSpinBox* crRange_;
        QSpinBox* mouseMotionBufferSize_;
        QSpinBox* sleepTime_;

        Ui::DebugDialog *ui_;
    };
    
    
} // namespace Gecon
#endif // GECON_DEBUGDIALOG_HPP
