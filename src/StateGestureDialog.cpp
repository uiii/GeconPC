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

namespace Gecon {
    
    StateGestureDialog::StateGestureDialog(GestureModel* gestureModel, ObjectModel* objectModel, QWidget *parent) :
        QDialog(parent),
        gestureModel_(gestureModel),
        objectModel_(objectModel),
        editedGesture_(0),
        ui_(new Ui::StateGestureDialog)
    {
        ui_->setupUi(this);

        stateSettings_.push_back(new VisibilityStateSettings);
        stateSettings_.push_back(new PositionStateSettings);
        stateSettings_.push_back(new AngleStateSettings);

        initObjectComboBox(objectModel);
        initPropertyComboBox();

        connect(ui_->deleteButton, SIGNAL(clicked()), this, SLOT(deleteGesture()));
    }
    
    StateGestureDialog::~StateGestureDialog()
    {
        delete ui_;
    }

    int StateGestureDialog::newGesture()
    {
        if(! objectModel_->size())
        {
            QMessageBox::critical(parentWidget(), "Object error", "No objects available", QMessageBox::Ok);

            return QDialog::Rejected;
        }

        reset();

        ui_->deleteButton->setVisible(false);

        disconnect(ui_->buttonBox, SIGNAL(accepted()), this, 0);
        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(addGesture()));

        return QDialog::exec();
    }

    int StateGestureDialog::editGesture(StateGestureWrapper* stateGesture)
    {
        reset();

        editedGesture_ = stateGesture;

        ui_->object->setCurrentIndex(objectModel_->index(editedGesture_->object()).row());

        ObjectPropertyStateSettingsList::iterator it = std::find_if(stateSettings_.begin(), stateSettings_.end(),
                [&](ObjectPropertyStateSettings* stateSettings){
                    return stateSettings->widget() == editedGesture_->stateSettings()->widget();
                }
        );

        ui_->objectProperty->setCurrentIndex(it - stateSettings_.begin());
        ui_->gestureName->setText(editedGesture_->name());
        ui_->deleteButton->setVisible(true);

        editedGesture_->stateSettings()->load();

        disconnect(ui_->buttonBox, SIGNAL(accepted()), this, 0);
        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(updateGesture()));

        return QDialog::exec();
    }

    void StateGestureDialog::addGesture()
    {
        QString name = ui_->gestureName->text();
        if(name.isEmpty())
        {
            name = "State gesture";
        }

        currentStateSettings_->save();

        try
        {
            gestureModel_->addStateGesture(
                name,
                ui_->object->itemData(ui_->object->currentIndex()).value<ObjectWrapper*>(),
                currentStateSettings_
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
        StateGestureWrapper gestureBackup(*editedGesture_);

        QString name = ui_->gestureName->text();
        if(name.isEmpty())
        {
            name = "State gesture";
        }

        currentStateSettings_->save();

        try
        {
            QModelIndex index = gestureModel_->index(editedGesture_);
            gestureModel_->removeGesture(index);
            gestureModel_->addStateGesture(
                name,
                ui_->object->itemData(ui_->object->currentIndex()).value<ObjectWrapper*>(),
                currentStateSettings_
            );

            accept();
        }
        catch(...)
        {
            gestureModel_->addStateGesture(gestureBackup.name(), gestureBackup.object(), gestureBackup.stateSettings());

            // TODO
        }
    }

    void StateGestureDialog::deleteGesture()
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, tr("Delete question"),
            tr("Do you really want to delete gesture '%1'").arg(editedGesture_->name()), QMessageBox::Ok | QMessageBox::Cancel);

        if(button == QMessageBox::Ok)
        {
            QModelIndex index = gestureModel_->index(editedGesture_);
            gestureModel_->removeGesture(index);

            accept();
        }
    }

    void StateGestureDialog::reset()
    {
        editedGesture_ = 0;

        ui_->object->setCurrentIndex(0);
        ui_->objectProperty->setCurrentIndex(0);

        for(ObjectPropertyStateSettings* stateSettings : stateSettings_)
        {
            stateSettings->reset();
        }

        ui_->gestureName->setText(QString());
    }

    void StateGestureDialog::pack()
    {
        //ui_->objectPropertyState->resize(0, 0);
        resize(0, 0);
    }

    void StateGestureDialog::initObjectComboBox(ObjectModel* objectModel)
    {
        ui_->object->setModel(objectModel);

        /*QPixmap red(16,16);
        red.fill(Qt::red);
        QLabel* icon = new QLabel(ui_->object);
        icon->setPixmap(red);*/
    }

    void StateGestureDialog::initPropertyComboBox()
    {
        for(ObjectPropertyStateSettings* stateSettings : stateSettings_)
        {
            ui_->objectProperty->addItem(stateSettings->propertyName());
            ui_->gestureSettingsLayout->addWidget(stateSettings->widget());

            stateSettings->widget()->setVisible(false);
        }

        //ui_->gestureSettingsLayout->addStretch(0);

        currentStateSettings_ = stateSettings_.front();
        currentStateSettings_->widget()->setVisible(true);

        connect(ui_->objectProperty, SIGNAL(currentIndexChanged(int)), this, SLOT(setPropertyStateSettings(int)));
    }

    void StateGestureDialog::setPropertyStateSettings(int index)
    {
        currentStateSettings_->widget()->setVisible(false);

        currentStateSettings_ = stateSettings_.at(index);
        currentStateSettings_->widget()->setVisible(true);

        pack();
    }

    int StateGestureDialog::exec()
    {
        return QDialog::Rejected;
    }
    
} // namespace Gecon
