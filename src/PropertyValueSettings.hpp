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

#ifndef GECON_PROPERTYVALUESETTINGS_HPP
#define GECON_PROPERTYVALUESETTINGS_HPP

#include <QWidget>
#include <QSpinBox>
#include <QComboBox>

#include <Gecon/Fraction.hpp>
#include <Gecon/Point.hpp>

#include "ObjectWrapper.hpp"

#define DECLARE_RELATION_LIST(PropertyType, ...) \
    template<> \
    struct PropertyRelations<PropertyType> \
    { \
        static QList<PropertyRelation<PropertyType>*> list() \
        { \
            return PropertyRelationList<PropertyType, \
                __VA_ARGS__ \
            >::list(); \
        } \
    };

namespace Gecon
{
    template< typename PropertyType >
    class PropertyValueSettings;

    template<>
    class PropertyValueSettings<int>
    {
    public:
        class Widget : public QWidget
        {
        public:
            Widget();

            QSpinBox* value;
        };

        PropertyValueSettings();

        int value() const;
        QWidget* widget() const;

        void load();
        void save();
        void reset();

    private:
        int value_;

        Widget* widget_;
    };

    template<>
    class PropertyValueSettings<Fraction>
    {
    public:
        class Widget : public QWidget
        {
        public:
            Widget();

            QSpinBox* nominator;
            QSpinBox* denominator;
        };

        PropertyValueSettings();

        Fraction value() const;
        QWidget* widget() const;

        void load();
        void save();
        void reset();

    private:
        Fraction value_;

        Widget* widget_;
    };

    template<>
    class PropertyValueSettings<double>
    {
    public:
        class Widget : public QWidget
        {
        public:
            Widget();

            QDoubleSpinBox* value;
        };

        PropertyValueSettings();

        double value() const;
        QWidget* widget() const;

        void load();
        void save();
        void reset();

    private:
        double value_;

        Widget* widget_;
    };

    template<>
    class PropertyValueSettings<bool>
    {
    public:
        class Widget : public QWidget
        {
        public:
            Widget();

            QComboBox* value;
        };

        PropertyValueSettings();
        PropertyValueSettings(const QString& trueOption, const QString& falseOption);

        bool value() const;
        QWidget* widget() const;

        void load();
        void save();
        void reset();

    private:
        int optionIndex_;

        Widget* widget_;
    };

    template<>
    class PropertyValueSettings<Point>
    {
    public:
        class Widget : public QWidget
        {
        public:
            Widget();

            QSpinBox* xValue;
            QSpinBox* yValue;
        };

        PropertyValueSettings();

        Point value() const;
        QWidget* widget() const;

        void load();
        void save();
        void reset();

    private:
        Point value_;

        Widget* widget_;
    };

    template< typename PropertyType >
    class PropertyValueSettings<RawObjectProperty<PropertyType> >
    {
    public:
        class Widget : public QWidget
        {
        public:
            Widget();

            QComboBox* property;
        };

        PropertyValueSettings();

        RawObjectProperty<PropertyType> value() const;
        QWidget* widget() const;

        void load();
        void save();
        void reset();

    private:
        int propertyIndex_;

        QList<RawObjectProperty<PropertyType> > properties_;

        Widget* widget_;
    };
} // namespace Gecon

#include "PropertyValueSettings.tpp"

#endif // GECON_PROPERTYVALUESETTINGS_HPP
