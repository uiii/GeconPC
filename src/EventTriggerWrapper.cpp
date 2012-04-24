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

#include "EventTriggerWrapper.hpp"

#include "EventWrapper.hpp"
#include "ActionSettings.hpp"

namespace Gecon
{
    EventTriggerWrapper::EventTriggerWrapper(const QString& name, const Events &onEvents, const Events &offEvents, ActionSettings *action):
        name_(name),
        onEvents_(onEvents),
        offEvents_(offEvents),
        action_(action->clone())
    {
        rawTrigger_ = action_->toTrigger(onEvents_, offEvents_);
    }

    EventTriggerWrapper::EventTriggerWrapper(const EventTriggerWrapper &another):
        EventTriggerWrapper(another.name(), another.onEvents_, another.offEvents_, another.action_)
    {
    }

    EventTriggerWrapper::~EventTriggerWrapper()
    {
        delete rawTrigger_;
    }

    const QString &EventTriggerWrapper::name() const
    {
        return name_;
    }

    const EventTriggerWrapper::Events &EventTriggerWrapper::onEvents() const
    {
        return onEvents_;
    }

    const EventTriggerWrapper::Events &EventTriggerWrapper::offEvents() const
    {
        return offEvents_;
    }

    ActionSettings *EventTriggerWrapper::action() const
    {
        return action_;
    }

    Event::Trigger *EventTriggerWrapper::rawTrigger() const
    {
        return rawTrigger_;
    }
} // namespace Gecon
