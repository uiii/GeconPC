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

#include "StateGestureDialog.hpp"
#include "ui_StateGestureDialog.hpp"

#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>

#include "StateGestureWrapper.hpp"
#include "GestureModel.hpp"
#include "ObjectModel.hpp"

#include "TestDialog.hpp"

#include "ObjectStateSettings.hpp"

namespace Gecon {
    
    StateGestureDialog::StateGestureDialog(GestureModel* gestureModel, ObjectModel* objectModel, TestDialog* testDialog, QWidget *parent) :
        QDialog(parent),
        gestureModel_(gestureModel),
        objectModel_(objectModel),
        testDialog_(testDialog),
        testedGesture_(0),
        editedGesture_(0),
        currentState_(0),
        ui_(new Ui::StateGestureDialog)
    {
        ui_->setupUi(this);

        states_.push_back(new ObjectStateSettings<bool>("vibility", &ObjectWrapper::RawObject::isVisible));
        states_.push_back(new ObjectStateSettings<Point>("position", &ObjectWrapper::RawObject::position));
        states_.push_back(new ObjectStateSettings<int>("angle", &ObjectWrapper::RawObject::angle));
        states_.push_back(new ObjectStateSettings<Fraction>("area", &ObjectWrapper::RawObject::areaSize));
        states_.push_back(new ObjectStateSettings<double>("aspect ratio", &ObjectWrapper::RawObject::aspectRatio));

        ui_->object->setModel(objectModel);

        initPropertyComboBox();

        connect(ui_->deleteButton, SIGNAL(clicked()), this, SLOT(deleteGesture()));
        connect(ui_->testButton, SIGNAL(clicked()), this, SLOT(testGesture()));
    }
    
    StateGestureDialog::~StateGestureDialog()
    {
        delete ui_;

        for(ObjectState* stateSettings : states_)
        {
            delete stateSettings;
        }
    }

    int StateGestureDialog::newGesture()
    {
        if(! objectModel_->size())
        {
            QMessageBox::critical(parentWidget(), "Object error", "No objects available", QMessageBox::Ok);

            return QDialog::Rejected;
        }

        reset();

        disconnect(ui_->buttonBox, SIGNAL(accepted()), this, 0);
        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(addGesture()));

        return QDialog::exec();
    }

    int StateGestureDialog::editGesture(StateGestureWrapper* stateGesture)
    {
        reset();

        editedGesture_ = stateGesture;

        ui_->object->setCurrentIndex(objectModel_->index(editedGesture_->object()).row());

        PropertyStateList::iterator it = std::find_if(states_.begin(), states_.end(),
                [&](ObjectState* state){
                    return state->widget() == editedGesture_->state()->widget();
                }
        );

        ui_->objectProperty->setCurrentIndex(it - states_.begin());
        ui_->gestureName->setText(editedGesture_->name());
        ui_->deleteButton->setVisible(true);

        editedGesture_->state()->load();

        disconnect(ui_->buttonBox, SIGNAL(accepted()), this, 0);
        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(updateGesture()));

        return QDialog::exec();
    }

    void StateGestureDialog::addGesture()
    {
        QString name = ui_->gestureName->text();

        currentState_->save();

        try
        {
            gestureModel_->addStateGesture(
                name,
                ui_->object->itemData(ui_->object->currentIndex()).value<ObjectWrapper*>(),
                currentState_
            );

            accept();
        }
        catch(...)
        {
            // TODO
        }
    }

    void StateGestureDialog::updateGesture()
    {
        currentState_->save();

        try
        {
            gestureModel_->editStateGesture(
                gestureModel_->index(editedGesture_),
                ui_->gestureName->text(),
                ui_->object->itemData(ui_->object->currentIndex()).value<ObjectWrapper*>(),
                currentState_
            );

            accept();
        }
        catch(const std::exception& e)
        {
            QMessageBox::critical(this, "State gesture error", e.what(), QMessageBox::Ok);
        }
    }

    void StateGestureDialog::deleteGesture()
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, tr("Delete question"),
            tr("Do you really want to delete gesture '%1'").arg(editedGesture_->name()), QMessageBox::Ok | QMessageBox::Cancel);

        if(button == QMessageBox::Ok)
        {
            QModelIndex index = gestureModel_->index(editedGesture_);
            if(gestureModel_->removeGesture(index))
            {
                accept();
            }
        }
    }

    void StateGestureDialog::testGesture()
    {
        currentState_->save();

        testedGesture_ = new StateGestureWrapper(
            "Tested gesture",
            ui_->object->itemData(ui_->object->currentIndex()).value<ObjectWrapper*>(),
            currentState_
        );

        connect(testDialog_, SIGNAL(finished(int)), this, SLOT(deleteTestedGesture()));

        testDialog_->testGesture(testedGesture_);
    }

    void StateGestureDialog::deleteTestedGesture()
    {
        delete testedGesture_;
        testedGesture_ = 0;

        disconnect(testDialog_, SIGNAL(finished(int)), this, SLOT(deleteTestedGesture()));
    }

    void StateGestureDialog::reset()
    {
        editedGesture_ = 0;

        ui_->object->setCurrentIndex(0);
        ui_->objectProperty->setCurrentIndex(0);

        for(ObjectState* state: states_)
        {
            state->reset();
        }

        ui_->gestureName->setText(QString());

        ui_->deleteButton->setVisible(false);
    }

    void StateGestureDialog::initPropertyComboBox()
    {
        for(ObjectState* state : states_)
        {
            ui_->objectProperty->addItem(state->propertyName());
            ui_->gestureSettingsLayout->addWidget(state->widget());

            state->widget()->setVisible(false);
        }

        //ui_->gestureSettingsLayout->addStretch(0);

        currentState_ = states_.front();
        currentState_->widget()->setVisible(true);

        connect(ui_->objectProperty, SIGNAL(currentIndexChanged(int)), this, SLOT(setObjectStateSettings(int)));
    }

    void StateGestureDialog::setObjectStateSettings(int index)
    {
        currentState_->widget()->setVisible(false);

        currentState_ = states_.at(index);
        currentState_->widget()->setVisible(true);
    }

    int StateGestureDialog::exec()
    {
        return QDialog::Rejected;
    }
} // namespace Gecon
