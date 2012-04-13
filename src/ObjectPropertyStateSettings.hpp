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
#include <QLabel>

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

        ObjectPropertyStateSettings(const QString& propertyName, QWidget* widget);
        virtual ~ObjectPropertyStateSettings();

        virtual const QString& propertyName();
        virtual QWidget* widget();

        virtual void reset() = 0;
        virtual void save() = 0;
        virtual void load() = 0;

        virtual ObjectPropertyStateSettings* clone() const = 0;

        virtual GesturePtr toGesture(ObjectWrapper object) = 0;

    private:
        QWidget* widget_;
        QString propertyName_;
    };

    class VisibilityStateSettingsWidget : public QWidget
    {
    public:
        VisibilityStateSettingsWidget(QWidget* parent);

        QComboBox* visibilityOptions;
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

        VisibilityStateSettingsWidget* widget_;
    };

    class PositionStateSettingsWidget : public QWidget
    {
        Q_OBJECT

    public:
        PositionStateSettingsWidget(QWidget* parent);

        QComboBox* relation;
        QSpinBox* distance;
        QLabel* from;
        QSpinBox* positionX;
        QSpinBox* positionY;

    public slots:
        void relationChanged();
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
        int distance_;
        int positionXValue_;
        int positionYValue_;

        PositionStateSettingsWidget* widget_;
    };

    class AngleStateSettingsWidget : public QWidget
    {
    public:
        AngleStateSettingsWidget(QWidget* parent);

        QComboBox* relationOptions;
        QSpinBox* angle;
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

        AngleStateSettingsWidget* widget_;
    };

} // namespace Gecon

#endif // GECON_OBJECTPROPERTYSTATESETTINGS_HPP
