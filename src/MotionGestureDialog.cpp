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

#include "MotionGestureDialog.hpp"
#include "ui_MotionGestureDialog.hpp"

#include <QPainter>
#include <QMessageBox>
#include <QtConcurrentRun>

#include "ObjectWrapper.hpp"
#include "ObjectModel.hpp"
#include "GestureModel.hpp"
#include "MotionGestureWrapper.hpp"

#include "TestDialog.hpp"

#include <fstream> // TODO remove

namespace Gecon
{
    MotionGestureDialog::MotionGestureDialog(GestureModel* gestureModel, ObjectModel *objectModel, TestDialog *testDialog, QWidget *parent) :
        QDialog(parent),
        objectModel_(objectModel),
        object_(0),
        gestureModel_(gestureModel),
        testDialog_(testDialog),
        testedGesture_(0),
        editedGesture_(0),
        recording_(false),
        motionRecorder_(0),
        ui_(new Ui::MotionGestureDialog)
    {
        ui_->setupUi(this);

        ui_->object->setModel(objectModel_);

        initReadyButton_();

        connect(this, SIGNAL(finished(int)), this, SLOT(stopCapture()));
        connect(ui_->object, SIGNAL(currentIndexChanged(int)), this, SLOT(selectObject(int)));
        connect(ui_->recordMotionButton, SIGNAL(clicked()), this, SLOT(startCapture()));
        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(addGesture()));
        connect(ui_->deleteButton, SIGNAL(clicked()), this, SLOT(deleteGesture()));
        connect(ui_->testButton, SIGNAL(clicked()), this, SLOT(testGesture()));

        //TODO remove
        connect(ui_->saveButton, SIGNAL(clicked()), this, SLOT(save()));
    }
    
    MotionGestureDialog::~MotionGestureDialog()
    {
        delete ui_;
    }

    int MotionGestureDialog::newGesture()
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

    int MotionGestureDialog::editGesture(MotionGestureWrapper* gesture)
    {
        reset();

        editedGesture_ = gesture;

        ui_->object->setCurrentIndex(objectModel_->index(editedGesture_->object()).row());
        ui_->gestureName->setText(editedGesture_->name());

        recordedMotion_ = gesture->motion();
        recordedMoves_ = gesture->rawGesture()->moves();
        displayRecordedMotion();

        ui_->deleteButton->show();

        disconnect(ui_->buttonBox, SIGNAL(accepted()), this, 0);
        connect(ui_->buttonBox, SIGNAL(accepted()), this, SLOT(updateGesture()));

        return QDialog::exec();
    }

    void MotionGestureDialog::addGesture()
    {
        if(recordedMotion_.empty())
        {
            QMessageBox::critical(this, "Motion error", "No motion recorded!", QMessageBox::Ok);
            return;
        }

        QString name = ui_->gestureName->text();

        try
        {
            gestureModel_->addMotionGesture(
                name,
                ui_->object->itemData(ui_->object->currentIndex()).value<ObjectWrapper*>(),
                recordedMotion_
            );

            accept();
        }
        catch(const std::exception& e)
        {
            QMessageBox::critical(this, "Motion gesture error", e.what(), QMessageBox::Ok);
        }
    }

    void MotionGestureDialog::updateGesture()
    {
        QString name = ui_->gestureName->text();

        try
        {
            gestureModel_->editMotionGesture(
                gestureModel_->index(editedGesture_),
                name,
                ui_->object->itemData(ui_->object->currentIndex()).value<ObjectWrapper*>(),
                recordedMotion_
            );

            accept();
        }
        catch(const std::exception& e)
        {
            QMessageBox::critical(this, "Motion gesture error", e.what(), QMessageBox::Ok);
        }
    }

    void MotionGestureDialog::deleteGesture()
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

    void MotionGestureDialog::startCapture()
    {
        ui_->object->setEnabled(false);

        connect(ui_->display, SIGNAL(imageDisplayed()), this, SLOT(firstImageDisplayed()));
        connect(control_.objectPolicySignaler(), SIGNAL(objectsRecognized(Image,Image,Objects)), this, SLOT(displayImage(Image,Image,Objects)), Qt::BlockingQueuedConnection);

        qDebug("dialog: start capture");

        control_.start();
    }

    void MotionGestureDialog::stopCapture()
    {
        disconnect(ui_->recordMotionButton, SIGNAL(clicked()), this, 0);
        connect(ui_->recordMotionButton, SIGNAL(clicked()), this, SLOT(startCapture()));

        if(recording_)
        {
            stopRecording();
        }

        disconnect(control_.objectPolicySignaler(), SIGNAL(objectsRecognized(Image,Image,Objects)), this, SLOT(displayImage(Image,Image,Objects)));

        QCoreApplication::processEvents();

        QtConcurrent::run(&control_, &ControlInfo::Control::stop);
        control_.prepareGestureCheckers(ControlInfo::GesturePolicy::GestureCheckers());

        readyLabel_->hide();
        readyButton_->hide();
        ui_->recordMotionButton->setChecked(false);
        ui_->object->setEnabled(true);

        ui_->display->reset();
        displayRecordedMotion();
    }

    void MotionGestureDialog::startRecording()
    {
        recording_ = true;

        motionRecorder_ = new MotionRecorder(object_->rawObject());

        connect(motionRecorder_->signaler().get(), SIGNAL(motionUpdated(const MotionRecorder::Motion&)),
                this, SLOT(motionUpdated(const MotionRecorder::Motion&)), Qt::BlockingQueuedConnection);
        connect(motionRecorder_->signaler().get(), SIGNAL(motionRecorded(const MotionRecorder::Motion&, const MotionRecorder::MoveSequence&)),
                this, SLOT(motionRecorded(const MotionRecorder::Motion&, const MotionRecorder::MoveSequence&)), Qt::BlockingQueuedConnection);

        ControlInfo::GesturePolicy::GestureCheckers checkers;
        checkers.insert(motionRecorder_);
        control_.prepareGestureCheckers(checkers);

        QtConcurrent::run(&control_, &ControlInfo::Control::restart);

        readyLabel_->hide();
        readyButton_->hide();
    }

    void MotionGestureDialog::stopRecording()
    {
        recording_ = false;

        currentlyRecordedMotion_.clear();

        disconnect(motionRecorder_->signaler().get(), SIGNAL(motionUpdated(const MotionRecorder::Motion&)),
                this, SLOT(motionUpdated(const MotionRecorder::Motion&)));
        disconnect(motionRecorder_->signaler().get(), SIGNAL(motionRecorded(const MotionRecorder::Motion&, const MotionRecorder::MoveSequence&)),
                this, SLOT(motionRecorded(const MotionRecorder::Motion&, const MotionRecorder::MoveSequence&)));

        QCoreApplication::processEvents();
    }

    void MotionGestureDialog::motionUpdated(const MotionRecorder::Motion &motion)
    {
        if(recording_)
        {
            currentlyRecordedMotion_ = motion;
        }
    }

    void MotionGestureDialog::motionRecorded(const MotionRecorder::Motion& motion, const MotionRecorder::MoveSequence& moves)
    {
        if(recording_)
        {
            std::cout << "motion recorded" << std::endl;

            recordedMotion_ = motion;
            recordedMoves_ = moves;

            std::cout << "moves count: " << moves.size() << std::endl;

            stopRecording();
            stopCapture();
        }
    }

    void MotionGestureDialog::displayImage(const Image &original, const Image &segmented, const ControlInfo::Objects &obejcts)
    {
        ui_->display->displayImage(original, obejcts, currentlyRecordedMotion_);
    }

    void MotionGestureDialog::displayRecordedMotion()
    {
        if(! recordedMotion_.empty())
        {
            QPolygon motion;
            for(const Point& point : recordedMotion_)
            {
                motion << QPoint(point.x, point.y);
            }

            double ratio = 1.0;
            if(motion.boundingRect().height() > motion.boundingRect().width())
            {
                ratio = 0.9 * ui_->display->height() / motion.boundingRect().height();
            }
            else
            {
                ratio = 0.9 * ui_->display->height() / motion.boundingRect().width();
            }

            QImage img(ui_->display->size(), QImage::Format_RGB888);
            img.fill(Qt::white);

            QPainter painter(&img);
            painter.setRenderHint(QPainter::Antialiasing);

            motion.translate(- motion.boundingRect().topLeft());

            double moveX = (ui_->display->width() - ratio * motion.boundingRect().width()) / 2;
            double moveY = (ui_->display->height() - ratio * motion.boundingRect().height()) / 2;

            painter.save();
            painter.setPen(QPen(Qt::red, 2.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.translate(moveX, moveY);
            painter.scale(ratio, ratio);
            painter.drawPolyline(motion);
            painter.restore();

            QPolygonF moves;
            QPointF point(0,0);
            std::cout << "moves count: " << recordedMoves_.size() << std::endl;
            for(auto move : recordedMoves_)
            {
                point = point + QPointF(std::cos(move * PI/4.0) * 20, -std::sin(move * PI/4.0) * 20);
                //std::cout << "point: " << point.x() << " " << point.y() << std::endl;
                //moves << point;
                //point = point + QPoint(1,1);
                moves << point;
            }

            ratio = 1.0;
            if(moves.boundingRect().height() > moves.boundingRect().width())
            {
                ratio = 0.9 * ui_->display->height() / moves.boundingRect().height();
            }
            else
            {
                ratio = 0.9 * ui_->display->height() / moves.boundingRect().width();
            }

            moves.translate(- moves.boundingRect().topLeft());

            moveX = (ui_->display->width() - ratio * moves.boundingRect().width()) / 2;
            moveY = (ui_->display->height() - ratio * moves.boundingRect().height()) / 2;

            painter.save();
            painter.translate(moveX, moveY);
            painter.scale(ratio, ratio);
            painter.setPen(QPen(Qt::green, 2.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawPolyline(moves);
            painter.restore();

            ui_->display->displayImage(img);
        }
    }

    void MotionGestureDialog::firstImageDisplayed()
    {
        disconnect(ui_->display, SIGNAL(imageDisplayed()), this, SLOT(firstImageDisplayed()));

        disconnect(ui_->recordMotionButton, SIGNAL(clicked()), this, 0);
        connect(ui_->recordMotionButton, SIGNAL(clicked()), this, SLOT(stopCapture()));

        readyLabel_->show();
        readyButton_->show();
    }

    void MotionGestureDialog::selectObject(int index)
    {
        if(index == -1)
        {
            return;
        }

        object_ = ui_->object->itemData(ui_->object->currentIndex()).value<ObjectWrapper*>();

        ControlInfo::Objects objects;
        objects.insert(object_->rawObject());
        control_.prepareObjects(objects);
    }

    void MotionGestureDialog::setDevice(MotionGestureDialog::DeviceAdapter device)
    {
        control_.setDevice(device);
    }

    void MotionGestureDialog::testGesture()
    {
        testedGesture_ = new MotionGestureWrapper(
            "Tested gesture",
            ui_->object->itemData(ui_->object->currentIndex()).value<ObjectWrapper*>(),
            recordedMotion_
        );

        connect(testDialog_, SIGNAL(finished(int)), this, SLOT(deleteTestedGesture()));

        testDialog_->testGesture(testedGesture_);
    }

    void MotionGestureDialog::deleteTestedGesture()
    {
        delete testedGesture_;
        testedGesture_ = 0;

        disconnect(testDialog_, SIGNAL(finished(int)), this, SLOT(deleteTestedGesture()));
    }

    void MotionGestureDialog::reset()
    {
        recordedMotion_.clear();
        recordedMoves_.clear();

        ui_->display->reset();
        ui_->object->setCurrentIndex(0);
        ui_->gestureName->setText(QString());
        ui_->deleteButton->hide();
    }

    int MotionGestureDialog::exec()
    {
        return QDialog::Rejected;
    }

    void MotionGestureDialog::save()
    {
        // TODO remove
        QString motionsFile = QString("/mnt/uloziste/Pracoviste/Projekty/GeconFramework/tmp/gesta-pismena/") + ui_->gestureName->text() + ".motions";
        QString movesFile = QString("/mnt/uloziste/Pracoviste/Projekty/GeconFramework/tmp/gesta-pismena/") + ui_->gestureName->text() + ".moves";
        std::ofstream motionsOut(motionsFile.toAscii().data(), std::ios_base::app);
        std::ofstream movesOut(movesFile.toAscii().data(), std::ios_base::app);
        for(auto point : recordedMotion_)
        {
            motionsOut << point.x << " " << point.y << " ";
        }
        motionsOut << "\n";
        std::cout << "moves count: " << recordedMoves_.size() << std::endl;
        for(auto move : recordedMoves_)
        {
            std::cout << "move: " << move << std::endl;
            movesOut << move << " ";
        }
        movesOut << "\n";
    }

    void MotionGestureDialog::initReadyButton_()
    {
        readyLabel_ = new QLabel("Prepare object and click <strong>Ready!</strong> to record motion.");
        readyLabel_->setStyleSheet(
                "color: red;"
                "background-color: rgb(255,255,255,200);"
                "border: 1px solid black;"
                "border-radius: 5px;"
                "padding: 3px;"
                "font-size: 12pt;"
        );
        readyLabel_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        readyLabel_->hide();

        readyButton_ = new QPushButton(tr("Ready!"));
        readyButton_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        readyButton_->hide();

        QVBoxLayout* layout = new QVBoxLayout(ui_->display);
        layout->addStretch();
        layout->addWidget(readyLabel_);
        layout->addWidget(readyButton_);
        layout->addStretch();

        layout->setAlignment(readyLabel_, Qt::AlignHCenter);
        layout->setAlignment(readyButton_, Qt::AlignHCenter);

        connect(readyButton_, SIGNAL(clicked()), this, SLOT(startRecording()));
    }
} // namespace Gecon
