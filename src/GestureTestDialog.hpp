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

#ifndef GECON_GESTURETESTDIALOG_HPP
#define GECON_GESTURETESTDIALOG_HPP

#include <QDialog>

#include <QStandardItemModel>

#include "ControlInfo.hpp"

#include <Gecon/Event.hpp>

namespace Gecon
{
    namespace Ui
    {
        class GestureTestDialog;
    }

    class GestureModel;
    class ObjectModel;
    class ObjectWrapper;
    class GestureWrapper;
    class StateGestureWrapper;
    class RelationGestureWrapper;
    class MotionGestureWrapper;
    class Trigger;

    class GestureTestDialog : public QDialog
    {
        Q_OBJECT
        
    public:
        typedef ControlInfo::DevicePolicy::DeviceAdapter DeviceAdapter;
        typedef Gecon::Image<RGB> Image;
        typedef ControlInfo::ObjectPolicy::ObjectPtr ObjectPtr;
        typedef ControlInfo::ObjectPolicy::ObjectSet ObjectSet;

        explicit GestureTestDialog(GestureModel* gestureModel, ObjectModel* objectModel, QWidget *parent = 0);
        ~GestureTestDialog();

        template< typename GestureWrapperType >
        void test(GestureWrapperType* gesture);

    public slots:
        void setDevice(DeviceAdapter device);

        void testAll();

        int exec();

    private slots:
        void logEvent_(GestureWrapper* gesture, const QString& eventDescription);

        void includeGesture_(StateGestureWrapper* stateGesture, bool tested);
        void includeGesture_(RelationGestureWrapper* relationGesture, bool tested);
        void includeGesture_(MotionGestureWrapper* motionGesture, bool tested);

        void includeAllGestures_(bool checked);

        void startCapture_();
        void restartCapture_();
        void stopCapture_();

        void displayImage_(Image original, Image segmented, ObjectSet objects);
        void displayObjectsStates_();

        void close_();
        void reset_();

        void reject();
        void accept();

    signals:
        void eventOccured(GestureWrapper* gesture, const QString& eventDescription);

    protected:
        void closeEvent(QCloseEvent *);

    private:
        ControlInfo::Control control_;

        ControlInfo::GesturePolicy::GestureSet gestures_;

        GestureModel* gestureModel_;
        ObjectModel* objectModel_;

        GestureWrapper* testedGesture_;
        std::list<Event::Trigger*> testedGestureTriggers_;

        QStandardItemModel* objectsStatesModel_;

        std::list<Event::Trigger*> triggers_;

        Ui::GestureTestDialog *ui_;
    };

    template< typename GestureWrapperType >
    void GestureTestDialog::test(GestureWrapperType* gesture)
    {
        includeGesture_(gesture, true);

        startCapture_();

        QDialog::exec();
    }
} // namespace Gecon
#endif // GECON_GESTURETESTDIALOG_HPP
