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

#ifndef GECON_MAINWINDOW_HPP
#define GECON_MAINWINDOW_HPP

#include <QMainWindow>

#include "ControlInfo.hpp"

#include "ObjectModel.hpp"
#include "GestureModel.hpp"
#include "ActionTriggerModel.hpp"

namespace Gecon
{
    namespace Ui
    {
        class MainWindow;
    }

    class SettingsDialog;
    class ObjectDialog;
    class StateGestureDialog;
    class RelationGestureDialog;
    class MotionGestureDialog;
    class TestDialog;
    class ActionTriggerDialog;

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public slots:
        void editObject(const QModelIndex& index);
        void editGesture(const QModelIndex& index);
        void editActionTrigger(const QModelIndex& index);

        void updateDialogs();

        void toggleControl(bool start);

    private:
        void initNewGestureMenu();

        ControlInfo::Control control_;

        ObjectModel objectModel_;
        GestureModel gestureModel_;
        ActionTriggerModel actionTriggerModel_;

        SettingsDialog* settingsDialog_;
        ObjectDialog* objectDialog_;

        TestDialog* gestureTestDialog_;

        StateGestureDialog* stateGestureDialog_;
        RelationGestureDialog* relationGestureDialog_;
        MotionGestureDialog* motionGestureDialog_;

        ActionTriggerDialog* eventTriggerDialog_;

        Ui::MainWindow *ui_;
    };
} // namespace Gecon
#endif // GECON_MAINWINDOW_HPP
