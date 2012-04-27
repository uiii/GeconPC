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

#include "RelationGestureDialog.hpp"
#include "ui_RelationGestureDialog.hpp"

#include <QMessageBox>

#include "ObjectRelationSettings.hpp"
#include "ObjectWrapper.hpp"
#include "RelationGestureWrapper.hpp"

#include "ObjectModel.hpp"
#include "GestureModel.hpp"

#include "TestDialog.hpp"

namespace Gecon {
    
    RelationGestureDialog::RelationGestureDialog(GestureModel* gestureModel, ObjectModel *objectModel, TestDialog *testDialog, QWidget *parent) :
        QDialog(parent),
        objectModel_(objectModel),
        gestureModel_(gestureModel),
        testDialog_(testDialog),
        testedGesture_(0),
        editedGesture_(0),
        currentRelation_(0),
        ui_(new Ui::RelationGestureDialog)
    {
        ui_->setupUi(this);

        for(auto property : ObjectProperties<bool>::list)
        {
            relations_.push_back(new ObjectRelationSettings<bool>(property.name, property.rawProperty));
        }

        for(auto property : ObjectProperties<int>::list)
        {
            relations_.push_back(new ObjectRelationSettings<int>(property.name, property.rawProperty));
        }

        for(auto property : ObjectProperties<Point>::list)
        {
            relations_.push_back(new ObjectRelationSettings<Point>(property.name, property.rawProperty));
        }

        initLeftPropertyComboBox_();

        ui_->leftObject->setModel(objectModel_);
        ui_->rightObject->setModel(objectModel_);

        connect(ui_->deleteButton, SIGNAL(clicked()), this, SLOT(deleteGesture()));
        connect(ui_->testButton, SIGNAL(clicked()), this, SLOT(testGesture()));
    }
    
    RelationGestureDialog::~RelationGestureDialog()
    {
        delete ui_;

        for(ObjectRelation* relation : relations_)
        {
            delete relation;
        }
    }

    int RelationGestureDialog::newGesture()
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

    int RelationGestureDialog::editGesture(RelationGestureWrapper* relationGesture)
    {
        reset();

        editedGesture_ = relationGesture;

        ui_->leftObject->setCurrentIndex(objectModel_->index(editedGesture_->leftObject()).row());
        ui_->rightObject->setCurrentIndex(objectModel_->index(editedGesture_->rightObject()).row());

        ObjectRelationList::iterator it = std::find_if(relations_.begin(), relations_.end(),
                [&](ObjectRelation* relation){
                    return relation->relationWidget() == editedGesture_->relation()->relationWidget()
                        && relation->rightPropertyWidget() == editedGesture_->relation()->rightPropertyWidget();
                }
        );

        ui_->leftProperty->setCurrentIndex(it - relations_.begin());
        ui_->gestureName->setText(editedGesture_->name());
        ui_->deleteButton->setVisible(true);

        editedGesture_->relation()->load();

        disconnect(ui_->buttonBox, SIGNAL(accepted()), this, 0);
        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(updateGesture()));

        return QDialog::exec();
    }

    void RelationGestureDialog::addGesture()
    {
        QString name = ui_->gestureName->text();

        currentRelation_->save();

        try
        {
            gestureModel_->addRelationGesture(
                ui_->gestureName->text(),
                ui_->leftObject->itemData(ui_->leftObject->currentIndex()).value<ObjectWrapper*>(),
                ui_->rightObject->itemData(ui_->rightObject->currentIndex()).value<ObjectWrapper*>(),
                currentRelation_
            );

            accept();
        }
        catch(...)
        {
            // TODO
        }
    }

    void RelationGestureDialog::updateGesture()
    {
        RelationGestureWrapper gestureBackup(*editedGesture_);

        currentRelation_->save();

        try
        {
            QModelIndex index = gestureModel_->index(editedGesture_);
            gestureModel_->removeGesture(index);
            gestureModel_->addRelationGesture(
                ui_->gestureName->text(),
                ui_->leftObject->itemData(ui_->leftObject->currentIndex()).value<ObjectWrapper*>(),
                ui_->rightObject->itemData(ui_->rightObject->currentIndex()).value<ObjectWrapper*>(),
                currentRelation_
            );

            accept();
        }
        catch(...)
        {
            gestureModel_->addRelationGesture(
                gestureBackup.name(),
                gestureBackup.leftObject(),
                gestureBackup.rightObject(),
                gestureBackup.relation());

            // TODO
        }
    }

    void RelationGestureDialog::deleteGesture()
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

    void RelationGestureDialog::testGesture()
    {
        currentRelation_->save();

        testedGesture_ = new RelationGestureWrapper(
            "Tested gesture",
            ui_->leftObject->itemData(ui_->leftObject->currentIndex()).value<ObjectWrapper*>(),
            ui_->rightObject->itemData(ui_->rightObject->currentIndex()).value<ObjectWrapper*>(),
            currentRelation_
        );

        connect(testDialog_, SIGNAL(finished(int)), this, SLOT(deleteTestedGesture()));

        testDialog_->testGesture(testedGesture_);
    }

    void RelationGestureDialog::deleteTestedGesture()
    {
        delete testedGesture_;
        testedGesture_ = 0;

        disconnect(testDialog_, SIGNAL(finished(int)), this, SLOT(deleteTestedGesture()));
    }

    void RelationGestureDialog::reset()
    {
        editedGesture_ = 0;

        ui_->leftObject->setCurrentIndex(0);
        ui_->rightObject->setCurrentIndex(0);

        ui_->leftProperty->setCurrentIndex(0);

        for(ObjectRelation* relation : relations_)
        {
            relation->reset();
        }

        ui_->gestureName->setText(QString());
        ui_->deleteButton->setVisible(false);
    }

    void RelationGestureDialog::setRelationSettings(int index)
    {
        currentRelation_->relationWidget()->setVisible(false);
        currentRelation_->rightPropertyWidget()->setVisible(false);

        currentRelation_ = relations_.at(index);
        currentRelation_->relationWidget()->setVisible(true);
        currentRelation_->rightPropertyWidget()->setVisible(true);
    }

    int RelationGestureDialog::exec()
    {
        return QDialog::Rejected;
    }

    void RelationGestureDialog::initLeftPropertyComboBox_()
    {
        for(ObjectRelation* relation : relations_)
        {
            ui_->leftProperty->addItem(relation->leftPropertyName());
            ui_->gestureSettingsLayout->insertWidget(2, relation->relationWidget());
            ui_->gestureSettingsLayout->addWidget(relation->rightPropertyWidget());

            relation->relationWidget()->setVisible(false);
            relation->rightPropertyWidget()->setVisible(false);
        }

        //ui_->gestureSettingsLayout->addStretch(0);

        currentRelation_ = relations_.front();

        connect(ui_->leftProperty, SIGNAL(currentIndexChanged(int)), this, SLOT(setRelationSettings(int)));

        setRelationSettings(0);
    }
} // namespace Gecon
