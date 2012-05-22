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

#include "GestureModel.hpp"

#include <QMessageBox>

#include "ActionTriggerModel.hpp"

namespace Gecon
{
    GestureModel::GestureModel(ActionTriggerModel* actionTriggerModel) :
        motionStorage_(new ControlInfo::MotionGesture::MotionStorage),
        actionTriggerModel_(actionTriggerModel)
    {
    }

    GestureModel::~GestureModel()
    {
        for(StateGestureWrapper* stateGesture : stateGestures_)
        {
            delete stateGesture;
        }

        for(RelationGestureWrapper* relationGesture : relationGestures_)
        {
            delete relationGesture;
        }

        for(MotionGestureWrapper* motionGesture : motionGestures_)
        {
            delete motionGesture;
        }

        delete motionStorage_;
    }

    int GestureModel::rowCount(const QModelIndex &parent) const
    {
        return size();
    }

    QVariant GestureModel::data(const QModelIndex &index, int role) const
    {
        if(! index.isValid() || index.row() >= size())
        {
            return QVariant();
        }

        int row = index.row();

        GestureWrapper* gesture = 0;
        if(row < stateGestures_.size())
        {
            gesture = stateGestures_.at(row);
        }
        else if((row -= stateGestures_.size()) < relationGestures_.size())
        {
            gesture = relationGestures_.at(row);
        }
        else if((row -= relationGestures_.size()) < motionGestures_.size())
        {
            gesture = motionGestures_.at(row);
        }

        if(role == Qt::DisplayRole)
        {
            return gesture->name();
        }
        else if(role == Qt::UserRole)
        {
            return QVariant::fromValue(gesture);
        }
        else
        {
            return QVariant();
        }
    }

    QModelIndex GestureModel::index(GestureWrapper* gesture) const
    {
        int index = 0;
        int offset = 0;

        index = stateGestures_.indexOf(static_cast<StateGestureWrapper*>(gesture));
        if(index == -1)
        {
            offset += stateGestures_.size();
            index = relationGestures_.indexOf(static_cast<RelationGestureWrapper*>(gesture));

            if(index == -1)
            {
                offset += relationGestures_.size();
                index = motionGestures_.indexOf(static_cast<MotionGestureWrapper*>(gesture));
            }
        }

        if(index != -1)
        {
            index += offset;
        }

        return createIndex(index, 0);
    }

    /*QModelIndex GestureModel::index(GestureWrapper::RawGesture* rawGesture) const
    {
        int index = -1;
        int offset = 0;

        auto hasSameRawGestures = [&](const GestureWrapper* item) -> bool
        {
                return rawGesture == item->rawGesture();
        };

        StateGestureWrapperList::iterator it = std::find_if(
            stateGestures_.begin(), stateGestures_.end(), hasSameRawGestures);

        if(it == stateGestures_.end())
        {
            offset += stateGestures_.size();

            RelationGestureWrapperList::iterator it = std::find_if(
                relationGestures_.begin(), relationGestures_.end(), hasSameRawGestures);

            if(it == relationGestures_.end())
            {
                offset += relationGestures_.size();

                MotionGestureWrapperList::iterator it = std::find_if(
                    motionGestures_.begin(), motionGestures_.end(), hasSameRawGestures);

                index = motionGestures_.indexOf(static_cast<MotionGestureWrapper*>(gesture));
            }
        }
        else
        {
            index = it - stateGestures_.begin();
        }

        if(index != -1)
        {
            index += offset;
        }

        return createIndex(index, 0);
    }*/

    int GestureModel::size() const
    {
        return stateGestures_.size() + relationGestures_.size() + motionGestures_.size();
    }

    void GestureModel::addStateGesture(const QString& name, ObjectWrapper* object, ObjectState *stateSettings)
    {
        QString gestureName = name.isEmpty()
                ? "State gesture " + QString::number(stateGestures_.size() + 1)
                : name;

        StateGestureWrapper* stateGesture = new StateGestureWrapper(gestureName, object, stateSettings);

        StateGestureWrappers::iterator it = std::find_if(stateGestures_.begin(), stateGestures_.end(),
            [&](const StateGestureWrapper* item){ return QString::localeAwareCompare(stateGesture->name(), item->name()) < 0; }
        );

        beginInsertRows(QModelIndex(), it - stateGestures_.begin(), it - stateGestures_.begin());

        stateGestures_.insert(it, stateGesture);

        endInsertRows();
    }

    void GestureModel::addRelationGesture(const QString &name, ObjectWrapper* leftObject, ObjectWrapper* rightObject, ObjectRelation *relationSettings)
    {
        QString gestureName = name.isEmpty()
                ? "Relation gesture " + QString::number(relationGestures_.size() + 1)
                : name;

        RelationGestureWrapper* relationGesture = new RelationGestureWrapper(gestureName, leftObject, rightObject, relationSettings);

        RelationGestureWrappers::iterator it = std::find_if(relationGestures_.begin(), relationGestures_.end(),
            [&](const RelationGestureWrapper* item){ return QString::localeAwareCompare(relationGesture->name(), item->name()) < 0; }
        );

        beginInsertRows(QModelIndex(), it - relationGestures_.begin(), it - relationGestures_.begin());

        relationGestures_.insert(it, relationGesture);

        endInsertRows();
    }

    void GestureModel::addMotionGesture(const QString &name, ObjectWrapper *object, const ControlInfo::MotionGesture::Motion& motion)
    {
        QString gestureName = name.isEmpty()
                ? "Motion gesture " + QString::number(motionGestures_.size() + 1)
                : name;

        MotionGestureWrapper* motionGesture = new MotionGestureWrapper(gestureName, object, motion, motionStorage_);

        MotionGestureWrapper* tooSimilarGesture = 0;
        for(MotionGestureWrapper* gesture : motionGestures_)
        {
            std::size_t maxDistance = ControlInfo::MotionGesture::MAXIMAL_SAME_GESTURE_DISTANCE * 2;
            std::size_t distance = gestureDistance(gesture->rawGesture(), motionGesture->rawGesture(), maxDistance);
            if(distance < maxDistance)
            {
                std::cout << "too similar distance: " << distance << std::endl;
                tooSimilarGesture = gesture;
                break;
            }
        }

        if(tooSimilarGesture)
        {
            delete motionGesture;
            throw std::logic_error(QString("Too similar gesture already exists: %1").arg(tooSimilarGesture->name()).toAscii().data());
        }

        MotionGestureWrappers::iterator it = std::find_if(motionGestures_.begin(), motionGestures_.end(),
            [&](const MotionGestureWrapper* item){ return QString::localeAwareCompare(motionGesture->name(), item->name()) < 0; }
        );

        beginInsertRows(QModelIndex(), it - motionGestures_.begin(), it - motionGestures_.begin());

        motionGestures_.insert(it, motionGesture);

        endInsertRows();
    }

    void GestureModel::editStateGesture(const QModelIndex &index, const QString &name, ObjectWrapper *object, ObjectState *stateSettings)
    {
        int realIndex = index.row();

        if(! index.isValid() || realIndex >= stateGestures_.size())
        {
            return;
        }

        QString gestureName = name.isEmpty()
                ? "State gesture " + QString::number(stateGestures_.size() + 1)
                : name;

        StateGestureWrapper* stateGesture = stateGestures_.at(realIndex);
        stateGesture->setName(gestureName);
        stateGesture->setObject(object);
        stateGesture->setState(stateSettings);

        beginRemoveRows(QModelIndex(), realIndex, realIndex);
        stateGestures_.removeAt(realIndex);
        endRemoveRows();

        StateGestureWrappers::iterator it = std::find_if(stateGestures_.begin(), stateGestures_.end(),
            [&](const StateGestureWrapper* item){ return QString::localeAwareCompare(stateGesture->name(), item->name()) < 0; }
        );

        beginInsertRows(QModelIndex(), it - stateGestures_.begin(), it - stateGestures_.begin());
        stateGestures_.insert(it, stateGesture);
        endInsertRows();
    }

    void GestureModel::editRelationGesture(const QModelIndex &index, const QString &name, ObjectWrapper *leftObject, ObjectWrapper *rightObject, ObjectRelation *relationSettings)
    {
        int realIndex = index.row() - stateGestures_.size();

        if(! index.isValid() || realIndex >= relationGestures_.size())
        {
            return;
        }

        QString gestureName = name.isEmpty()
                ? "Relation gesture " + QString::number(relationGestures_.size() + 1)
                : name;

        RelationGestureWrapper* relationGesture = relationGestures_.at(realIndex);
        relationGesture->setName(gestureName);
        relationGesture->setLeftObject(leftObject);
        relationGesture->setRightObject(rightObject);
        relationGesture->setRelation(relationSettings);

        beginRemoveRows(QModelIndex(), realIndex, realIndex);
        relationGestures_.removeAt(realIndex);
        endRemoveRows();

        RelationGestureWrappers::iterator it = std::find_if(relationGestures_.begin(), relationGestures_.end(),
            [&](const RelationGestureWrapper* item){ return QString::localeAwareCompare(relationGesture->name(), item->name()) < 0; }
        );

        beginInsertRows(QModelIndex(), it - relationGestures_.begin(), it - relationGestures_.begin());
        relationGestures_.insert(it, relationGesture);
        endInsertRows();
    }

    void GestureModel::editMotionGesture(const QModelIndex &index, const QString &name, ObjectWrapper *object, const ControlInfo::MotionGesture::Motion &motion)
    {
        int realIndex = index.row() - relationGestures_.size() - stateGestures_.size();

        if(! index.isValid() || realIndex >= motionGestures_.size())
        {
            return;
        }

        QString gestureName = name.isEmpty()
                ? "Motion gesture " + QString::number(motionGestures_.size() + 1)
                : name;

        MotionGestureWrapper* motionGesture = motionGestures_.at(realIndex);
        motionGesture->setName(gestureName);
        motionGesture->setObject(object);
        motionGesture->setMotion(motion);

        beginRemoveRows(QModelIndex(), realIndex, realIndex);
        motionGestures_.removeAt(realIndex);
        endRemoveRows();

        MotionGestureWrappers::iterator it = std::find_if(motionGestures_.begin(), motionGestures_.end(),
            [&](const MotionGestureWrapper* item){ return QString::localeAwareCompare(motionGesture->name(), item->name()) < 0; }
        );

        beginInsertRows(QModelIndex(), it - motionGestures_.begin(), it - motionGestures_.begin());
        motionGestures_.insert(it, motionGesture);
        endInsertRows();
    }

    const GestureModel::StateGestureWrappers& GestureModel::stateGestures() const
    {
        return stateGestures_;
    }

    const GestureModel::RelationGestureWrappers& GestureModel::relationGestures() const
    {
        return relationGestures_;
    }

    const GestureModel::MotionGestureWrappers& GestureModel::motionGestures() const
    {
        return motionGestures_;
    }

    ControlInfo::Gestures GestureModel::rawGestures()
    {
        ControlInfo::Gestures rawGestures;

        for(StateGestureWrapper* stateGesture : stateGestures_)
        {
            rawGestures.insert(stateGesture->rawGesture());
        }

        for(RelationGestureWrapper* relationGesture : relationGestures_)
        {
            rawGestures.insert(relationGesture->rawGesture());
        }

        for(MotionGestureWrapper* motionGesture : motionGestures_)
        {
            rawGestures.insert(motionGesture->rawGesture());
        }

        return rawGestures;
    }

    bool GestureModel::removeGesture(const QModelIndex &index)
    {
        if(! index.isValid() || index.row() >= size())
        {
            return false;
        }

        beginRemoveRows(QModelIndex(), index.row(), index.row());

        int row = index.row();

        std::cout << "row: " << row << std::endl;
        std::cout << "state: " << stateGestures_.size() << std::endl;
        std::cout << "relation: " << relationGestures_.size() << std::endl;
        std::cout << "motion: " << motionGestures_.size() << std::endl;

        GestureWrapper* gesture = data(index, Qt::UserRole).value<GestureWrapper*>();

        if(! gesture->actionTriggers().empty())
        {
            QMessageBox message(QMessageBox::Warning, tr("Delete action trigger question"),
                tr("There are %1 action triggers defined for this gesture's events. Action triggers will be deleted too.").arg(gesture->actionTriggers().size()), QMessageBox::Ok | QMessageBox::Cancel);

            QString detailedText;
            for(ActionTriggerWrapper* trigger : gesture->actionTriggers())
            {
                detailedText.append(trigger->name() + "\n");
            }
            message.setDetailedText(detailedText);

            int button = message.exec();

            if(button != QMessageBox::Ok)
            {
                return false;
            }

            for(ActionTriggerWrapper* trigger : gesture->actionTriggers())
            {
                if(! actionTriggerModel_->removeTrigger(actionTriggerModel_->index(trigger)))
                {
                    return false;
                }
            }
        }

        if(row < stateGestures_.size())
        {
            std::cout << "del row: " << row << std::endl;
            delete stateGestures_.at(row);
            stateGestures_.removeAt(row);
        }
        else if((row -= stateGestures_.size()) < relationGestures_.size())
        {
            std::cout << "del row: " << row << std::endl;
            delete relationGestures_.at(row);
            relationGestures_.removeAt(row);
        }
        else if((row -= relationGestures_.size()) < motionGestures_.size())
        {
            std::cout << "del row: " << row << std::endl;
            delete motionGestures_.at(row);
            motionGestures_.removeAt(row);
        }

        endRemoveRows();

        return true;
    }

    ControlInfo::MotionGesture::MotionStorage *GestureModel::motionStorage()
    {
        return motionStorage_;
    }
} // namespace Gecon
