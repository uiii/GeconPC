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
#include <QLabel>
#include <QPushButton>

#include "MotionRecorder.hpp"

namespace Gecon
{
    namespace Ui
    {
        class MotionGestureDialog;
    }

    class ObjectModel; // forward declaration
    class ObjectWrapper;
    class GestureModel;
    class MotionGestureWrapper;
    class TestDialog;

    class MotionGestureDialog : public QDialog
    {
        Q_OBJECT
        
    public:
        typedef ControlInfo::DevicePolicy::DeviceAdapter DeviceAdapter;
        typedef Gecon::Image<RGB> Image;
        typedef ControlInfo::ObjectPolicy::Object Object;
        typedef ControlInfo::ObjectPolicy::ObjectSet ObjectSet;

        explicit MotionGestureDialog(GestureModel* gestureModel, ObjectModel* objectModel, TestDialog* testDialog, QWidget *parent = 0);
        ~MotionGestureDialog();

    public slots:
        int newGesture();
        int editGesture(MotionGestureWrapper* gesture);

        void addGesture();
        void updateGesture();
        void deleteGesture();

        void startCapture();
        void stopCapture();

        void startRecording();
        void stopRecording();

        void motionUpdated(const MotionRecorder::Motion& motion);
        void motionRecorded(const MotionRecorder::Motion& motion, const MotionRecorder::MoveSequence& moves);

        void displayImage(Image original, Image segmented, ObjectSet obejcts);
        void displayRecordedMotion();

        void firstImageDisplayed();

        void selectObject(int index);

        void setDevice(DeviceAdapter device);

        void testGesture();
        void deleteTestedGesture();

        void reset();

        int exec();

    private:
        void initReadyButton_();

        ControlInfo::Control control_;

        ObjectModel* objectModel_;
        ObjectWrapper* object_;

        GestureModel* gestureModel_;

        TestDialog* testDialog_;
        MotionGestureWrapper* testedGesture_;

        MotionGestureWrapper* editedGesture_;

        bool recording_;
        MotionRecorder* motionRecorder_;
        MotionRecorder::Motion currentlyRecordedMotion_;
        MotionRecorder::Motion recordedMotion_;
        MotionRecorder::MoveSequence recordedMoves_;

        QLabel* readyLabel_;
        QPushButton* readyButton_;

        Ui::MotionGestureDialog *ui_;
    };
} // namespace Gecon
#endif // GECON_MOTIONGESTUREDIALOG_HPP
