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

#include "GestureWrapper.hpp"

namespace Gecon
{
    GestureWrapper::GestureWrapper(const QString& name, const GestureWrapper::Objects& objects):
        name_(name),
        objects_(objects)
    {
        addGestureToObjects_();
    }

    GestureWrapper::~GestureWrapper()
    {
        removeGestureFromObjects_();
    }

    const QString &GestureWrapper::name() const
    {
        return name_;
    }

    const GestureWrapper::Objects& GestureWrapper::objects() const
    {
        return objects_;
    }

    void GestureWrapper::setName(const QString& name)
    {
        name_ = name;
    }

    void GestureWrapper::setObjects(const GestureWrapper::Objects &objects)
    {
        removeGestureFromObjects_();
        objects_ = objects;
        addGestureToObjects_();
    }

    void GestureWrapper::addActionTrigger(ActionTriggerWrapper *trigger)
    {
        actionTriggers_.insert(trigger);
    }

    void GestureWrapper::removeActionTrigger(ActionTriggerWrapper *trigger)
    {
        actionTriggers_.erase(trigger);
    }

    const std::set<ActionTriggerWrapper*>& GestureWrapper::actionTriggers()
    {
        return actionTriggers_;
    }

    void GestureWrapper::addGestureToObjects_()
    {
        for(ObjectWrapper* object : objects_)
        {
            object->addGesture(this);
        }
    }

    void GestureWrapper::removeGestureFromObjects_()
    {
        for(ObjectWrapper* object : objects_)
        {
            object->removeGesture(this);
        }
    }
} // namespace Gecon
