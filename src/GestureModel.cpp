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

namespace Gecon
{
    GestureModel::GestureModel(QObject *parent) :
        QAbstractListModel(parent)
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

        StateGestureWrapperList::iterator it = std::find_if(stateGestures_.begin(), stateGestures_.end(),
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

        RelationGestureWrapperList::iterator it = std::find_if(relationGestures_.begin(), relationGestures_.end(),
            [&](const RelationGestureWrapper* item){ return QString::localeAwareCompare(relationGesture->name(), item->name()) < 0; }
        );

        beginInsertRows(QModelIndex(), it - relationGestures_.begin(), it - relationGestures_.begin());

        relationGestures_.insert(it, relationGesture);

        endInsertRows();
    }

    void GestureModel::addMotionGesture(const QString &name, ObjectWrapper *object, const MotionGestureWrapper::Motion& motion)
    {
        QString gestureName = name.isEmpty()
                ? "Motion gesture " + QString::number(motionGestures_.size() + 1)
                : name;

        MotionGestureWrapper* motionGesture = new MotionGestureWrapper(gestureName, object, motion);

        MotionGestureWrapperList::iterator it = std::find_if(motionGestures_.begin(), motionGestures_.end(),
            [&](const MotionGestureWrapper* item){ return QString::localeAwareCompare(motionGesture->name(), item->name()) < 0; }
        );

        beginInsertRows(QModelIndex(), it - motionGestures_.begin(), it - motionGestures_.begin());

        motionGestures_.insert(it, motionGesture);

        endInsertRows();
    }

    const GestureModel::StateGestureWrapperList& GestureModel::stateGestures() const
    {
        return stateGestures_;
    }

    const GestureModel::RelationGestureWrapperList& GestureModel::relationGestures() const
    {
        return relationGestures_;
    }

    const GestureModel::MotionGestureWrapperList& GestureModel::motionGestures() const
    {
        return motionGestures_;
    }

    void GestureModel::removeGesture(const QModelIndex &index)
    {
        if(! index.isValid() || index.row() >= size())
        {
            return;
        }

        beginRemoveRows(QModelIndex(), index.row(), index.row());

        int row = index.row();

        std::cout << "row: " << row << std::endl;
        std::cout << "state: " << stateGestures_.size() << std::endl;
        std::cout << "relation: " << relationGestures_.size() << std::endl;
        std::cout << "motion: " << motionGestures_.size() << std::endl;

        if(row < stateGestures_.size())
        {
            std::cout << "del row: " << row << std::endl;
            stateGestures_.removeAt(row);
        }
        else if((row -= stateGestures_.size()) < relationGestures_.size())
        {
            std::cout << "del row: " << row << std::endl;
            relationGestures_.removeAt(row);
        }
        else if((row -= relationGestures_.size()) < motionGestures_.size())
        {
            std::cout << "del row: " << row << std::endl;
            motionGestures_.removeAt(row);
        }

        endRemoveRows();
    }
} // namespace Gecon
