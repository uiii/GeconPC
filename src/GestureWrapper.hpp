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

#ifndef GECON_GESTUREWRAPPER_HPP
#define GECON_GESTUREWRAPPER_HPP

#include <QString>
#include <QMetaType>

#include <Gecon/ObjectGesture.hpp>

#include "ControlInfo.hpp"
#include "ObjectWrapper.hpp"
#include "EventWrapper.hpp"
#include <ActionTriggerWrapper.hpp>

namespace Gecon
{
    class ActionTriggerWrapper;

    class GestureWrapper
    {
    public:
        typedef QList<ObjectWrapper*> Objects;
        typedef QList<EventWrapper*> Events;

        GestureWrapper(const QString& name, const Objects& objects);
        virtual ~GestureWrapper();

        virtual const QString& name() const;
        virtual const Objects& objects() const;
        virtual const Events& events() const = 0;

        virtual void setName(const QString& name);
        virtual void setObjects(const Objects& objects);

        virtual void edit() = 0;

        virtual ControlInfo::Gesture* rawGesture() = 0;

        void addActionTrigger(ActionTriggerWrapper* trigger);
        void removeActionTrigger(ActionTriggerWrapper* trigger);
        const std::set<ActionTriggerWrapper*>& actionTriggers();

    private:
        QString name_;
        Objects objects_;

        void addGestureToObjects_();
        void removeGestureFromObjects_();

        std::set<ActionTriggerWrapper*> actionTriggers_;
    };
} // namespace Gecon

Q_DECLARE_METATYPE(Gecon::GestureWrapper*)

#endif // GECON_GESTUREWRAPPER_HPP
