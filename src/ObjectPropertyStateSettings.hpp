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

#include <Gecon/ColorObject.hpp>
#include <Gecon/ObjectStateGesture.hpp>

namespace Gecon
{
    class ObjectStateSettings
    {
    public:
        typedef ObjectStateGesture<ControlInfo::ObjectPolicy::Object> RawGesture;

        ObjectStateSettings(const QString& propertyName, QWidget* widget);
        virtual ~ObjectStateSettings();

        virtual const QString& propertyName();
        virtual QWidget* widget();

        virtual void reset() = 0;
        virtual void save() = 0;
        virtual void load() = 0;

        virtual ObjectStateSettings* clone() const = 0;

        virtual RawGesture* toGesture(ObjectWrapper* object) = 0;

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

    class VisibilityStateSettings : public ObjectStateSettings
    {
    public:
        VisibilityStateSettings(QWidget* parent = 0);

        void reset();
        void save();
        void load();

        ObjectStateSettings* clone() const;

        RawGesture* toGesture(ObjectWrapper* object);

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

    class PositionStateSettings : public ObjectStateSettings
    {
    public:
        PositionStateSettings(QWidget* parent = 0);

        void reset();
        void save();
        void load();

        ObjectStateSettings* clone() const;

        RawGesture* toGesture(ObjectWrapper* object);

    private:
        typedef Point PropertyReturnType;
        typedef RawGesture::Relation<PropertyReturnType> RelationType;

        QList<RelationType> relations_;

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

        QComboBox* relation;
        QSpinBox* angle;
    };

    class AngleStateSettings : public ObjectStateSettings
    {
    public:
        AngleStateSettings(QWidget* parent = 0);

        void reset();
        void save();
        void load();

        ObjectStateSettings* clone() const;

        RawGesture* toGesture(ObjectWrapper* object);

    private:
        typedef int PropertyReturnType;
        typedef RawGesture::Relation<PropertyReturnType> RelationType;

        QList<RelationType> relations_;

        int relationIndex_;
        int angleValue_;

        AngleStateSettingsWidget* widget_;
    };

} // namespace Gecon

#endif // GECON_OBJECTPROPERTYSTATESETTINGS_HPP
