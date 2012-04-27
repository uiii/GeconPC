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

#ifndef GECON_TESTDIALOG_HPP
#define GECON_TESTDIALOG_HPP

#include <QDialog>

#include <QStandardItemModel>

#include "ControlInfo.hpp"

namespace Gecon
{
    namespace Ui
    {
        class TestDialog;
    }

    class GestureModel;
    class ObjectModel;
    class ActionTriggerModel;

    class ObjectWrapper;
    class GestureWrapper;
    class StateGestureWrapper;
    class RelationGestureWrapper;
    class MotionGestureWrapper;
    class ActionTriggerWrapper;

    class TestDialog : public QDialog
    {
        Q_OBJECT
        
    public:
        typedef ControlInfo::DevicePolicy::DeviceAdapter DeviceAdapter;
        typedef Gecon::Image<RGB> Image;
        typedef ControlInfo::ObjectPolicy::ObjectPtr ObjectPtr;
        typedef ControlInfo::ObjectPolicy::ObjectSet ObjectSet;
        typedef ControlInfo::GesturePolicy::Gestures Gestures;
        typedef ControlInfo::ActionPolicy::ActionTrigger ActionTrigger;
        typedef ControlInfo::ActionPolicy::ActionTriggers ActionTriggers;

        explicit TestDialog(ObjectModel* objectModel, GestureModel* gestureModel, ActionTriggerModel* actionTriggerModel, QWidget *parent = 0);
        ~TestDialog();

        template< typename GestureWrapperType >
        void testGesture(GestureWrapperType* gesture);

        void testActionTrigger(ActionTriggerWrapper* trigger);

    public slots:
        void setDevice(DeviceAdapter device);

        void testAll();

        int exec();

    private slots:
        void logGestureEvent_(GestureWrapper* gesture, const QString& eventDescription, bool isTestedGesture);
        void logAction_(const QString& triggerName, bool isTestedTrigger);

        void includeGesture_(StateGestureWrapper* stateGesture, bool isTestedGesture);
        void includeGesture_(RelationGestureWrapper* relationGesture, bool isTestedGesture);
        void includeGesture_(MotionGestureWrapper* motionGesture, bool isTestedGesture);
        void includeAllGestures_();

        void includeActionTrigger_(ActionTriggerWrapper* trigger, bool isTestedTrigger);
        void includeAllActionTriggers_();

        void startCapture_();
        void stopCapture_();

        void displayImage_(Image original, Image segmented, ObjectSet objects);
        void displayObjectsStates_();

        void close_();
        void reset_();

        void reject();
        void accept();

    signals:
        void gestureEventOccured(GestureWrapper* gesture, const QString& eventDescription, bool isTestedGesture);
        void actionTriggered(const QString& triggerName, bool isTestedTrigger);

    protected:
        void closeEvent(QCloseEvent *);

    private:
        ControlInfo::Control control_;

        ControlInfo::GesturePolicy::Gestures gestures_;

        ObjectModel* objectModel_;
        GestureModel* gestureModel_;
        ActionTriggerModel* actionTriggerModel_;

        QStandardItemModel* objectsStatesModel_;

        ActionTriggers triggers_;

        Ui::TestDialog *ui_;
    };

    template< typename GestureWrapperType >
    void TestDialog::testGesture(GestureWrapperType* gesture)
    {
        includeGesture_(gesture, true);
        includeAllGestures_();

        startCapture_();

        QDialog::exec();
    }
} // namespace Gecon
#endif // GECON_TESTDIALOG_HPP
