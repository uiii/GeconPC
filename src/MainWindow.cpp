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

#include "MainWindow.hpp"
#include "ui_MainWindow.hpp"

#include <QMenu>

#include "SettingsDialog.hpp"
#include "ObjectDialog.hpp"
#include "StateGestureDialog.hpp"
#include "RelationGestureDialog.hpp"
#include "MotionGestureDialog.hpp"
#include "GestureTestDialog.hpp"
#include "EventTriggerDialog.hpp"
#include "DebugDialog.hpp"

namespace Gecon
{
    MainWindow::MainWindow(QWidget *parent):
        QMainWindow(parent),
        settingsDialog_(new SettingsDialog(&control_, this)),
        objectDialog_(new ObjectDialog(&objectModel_, this)),
        gestureTestDialog_(new GestureTestDialog(&gestureModel_, &objectModel_, this)),
        stateGestureDialog_(new StateGestureDialog(&gestureModel_, &objectModel_, gestureTestDialog_, this)),
        relationGestureDialog_(new RelationGestureDialog(&gestureModel_, &objectModel_, gestureTestDialog_, this)),
        motionGestureDialog_(new MotionGestureDialog(&gestureModel_, &objectModel_, gestureTestDialog_, this)),
        eventTriggerDialog_(new EventTriggerDialog(&eventTriggerModel_, &gestureModel_, &objectModel_, this)),
        ui_(new Ui::MainWindow)
    {
        DebugDialog* debug = new DebugDialog(this); // TODO
        debug->show();

        StateGestureWrapper::dialog = stateGestureDialog_;
        RelationGestureWrapper::dialog = relationGestureDialog_;
        MotionGestureWrapper::dialog = motionGestureDialog_;

        ui_->setupUi(this);

        ui_->objectView->setModel(&objectModel_);
        ui_->gestureView->setModel(&gestureModel_);
        ui_->eventTriggerView->setModel(&eventTriggerModel_);

        updateDialogs();

        initNewGestureMenu();

        connect(ui_->newObjectButton, SIGNAL(clicked()), objectDialog_, SLOT(newObject()));
        connect(ui_->newEventTriggerButton, SIGNAL(clicked()), eventTriggerDialog_, SLOT(newTrigger()));

        connect(ui_->actionSettings, SIGNAL(triggered()), settingsDialog_, SLOT(exec()));
        connect(ui_->actionTest, SIGNAL(triggered()), gestureTestDialog_, SLOT(testAll()));

        connect(settingsDialog_, SIGNAL(finished(int)), this, SLOT(updateDialogs()));

        connect(ui_->gestureView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editGesture(QModelIndex)));
        connect(ui_->eventTriggerView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editEventTrigger(QModelIndex)));
    }

    MainWindow::~MainWindow()
    {
        delete ui_;
    }

    void MainWindow::editGesture(const QModelIndex& index)
    {
        GestureWrapper* gesture = gestureModel_.data(index, Qt::UserRole).value<GestureWrapper*>();
        gesture->edit();
    }

    void MainWindow::editEventTrigger(const QModelIndex &index)
    {
        EventTriggerWrapper* trigger = eventTriggerModel_.data(index, Qt::UserRole).value<EventTriggerWrapper*>();
        eventTriggerDialog_->editTrigger(trigger);
    }

    void MainWindow::updateDialogs()
    {
        objectDialog_->setDevice(control_.device());
        motionGestureDialog_->setDevice(control_.device());
        gestureTestDialog_->setDevice(control_.device());
    }

    void MainWindow::initNewGestureMenu()
    {
        QMenu* newGestureMenu = new QMenu(ui_->newGestureButton);

        QAction* newStateGestureAction = new QAction("State gesture", newGestureMenu);
        QAction* newRelationGestureAction = new QAction("Relation gesture", newGestureMenu);
        QAction* newMotionGestureAction = new QAction("Motion gesture", newGestureMenu);

        newGestureMenu->addAction(newStateGestureAction);
        newGestureMenu->addAction(newRelationGestureAction);
        newGestureMenu->addAction(newMotionGestureAction);

        ui_->newGestureButton->setMenu(newGestureMenu);

        connect(newStateGestureAction, SIGNAL(triggered()), stateGestureDialog_, SLOT(newGesture()));
        connect(newRelationGestureAction, SIGNAL(triggered()), relationGestureDialog_, SLOT(newGesture()));
        connect(newMotionGestureAction, SIGNAL(triggered()), motionGestureDialog_, SLOT(newGesture()));
    }
} // namespace Gecon
