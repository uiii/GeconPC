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

#ifndef GECON_OBJECTPROPERTYSTATESETTINGS_HPP
#define GECON_OBJECTPROPERTYSTATESETTINGS_HPP

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>

#include <functional>

#include "ControlInfo.hpp"
#include "ObjectWrapper.hpp"

namespace Gecon
{
    class ObjectPropertyStateSettings
    {
    public:
        typedef ControlInfo::GesturePolicy::GesturePtr GesturePtr;
        typedef ControlInfo::ObjectPolicy::Object Object;
        typedef ControlInfo::ObjectPolicy::ObjectPtr ObjectPtr;

        ObjectPropertyStateSettings(const QString& propertyName, QWidget* parent = 0);
        virtual ~ObjectPropertyStateSettings();

        virtual const QString& propertyName();
        virtual QWidget* widget();

        virtual void reset() = 0;
        virtual void save() = 0;
        virtual void load() = 0;

        virtual ObjectPropertyStateSettings* clone() const = 0;

        virtual GesturePtr toGesture(ObjectWrapper object) = 0;

    protected:
        QWidget* widget_;

    private:
        QString propertyName_;
    };

    class VisibilityStateSettings : public ObjectPropertyStateSettings
    {
    public:
        VisibilityStateSettings(QWidget* parent = 0);

        void reset();
        void save();
        void load();

        ObjectPropertyStateSettings* clone() const;

        GesturePtr toGesture(ObjectWrapper object);

    private:
        int visibilityOptionsIndex_;

        QComboBox* visibilityOptions_;
    };

    class PositionStateSettings : public ObjectPropertyStateSettings
    {
    public:
        PositionStateSettings(QWidget* parent = 0);

        void reset();
        void save();
        void load();

        ObjectPropertyStateSettings* clone() const;

        GesturePtr toGesture(ObjectWrapper object);

    private:
        int relationIndex_;
        int positionXValue_;
        int positionYValue_;

        QComboBox* relation_;
        QSpinBox* positionX_;
        QSpinBox* positionY_;
    };

    class AngleStateSettings : public ObjectPropertyStateSettings
    {
    public:
        AngleStateSettings(QWidget* parent = 0);

        void reset();
        void save();
        void load();

        ObjectPropertyStateSettings* clone() const;

        GesturePtr toGesture(ObjectWrapper object);

    private:
        typedef decltype(Object().angle()) PropertyReturnType;
        typedef std::function<bool(const PropertyReturnType&, const PropertyReturnType&)> RelationType;

        QList<RelationType> relations_;

        int relationOptionsIndex_;
        int angleValue_;

        QComboBox* relationOptions_;
        QSpinBox* angle_;
    };

} // namespace Gecon

#endif // GECON_OBJECTPROPERTYSTATESETTINGS_HPP
