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

#ifndef GECON_PROPERTYRELATIONSETTINGS_HPP
#define GECON_PROPERTYRELATIONSETTINGS_HPP

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QMetaType>

#include <Gecon/Point.hpp>

#define DECLARE_SIMPLE_RELATION(ClassName, name, relation) \
    template< typename PropertyType > \
    struct ClassName : public PropertyRelation<PropertyType> \
    { \
        ClassName(): PropertyRelation<PropertyType>(name, relation) \
        {} \
    };

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

Q_DECLARE_METATYPE(QWidget*)

namespace Gecon
{
    template< typename PropertyType >
    class PropertyRelation
    {
    public:
        typedef std::function<bool(const PropertyType&, const PropertyType&)> RawRelation;

        PropertyRelation(const QString& name, RawRelation relation = RawRelation(), QWidget* widget = 0);
        virtual ~PropertyRelation();

        const QString& name() const;
        virtual RawRelation rawRelation() const;
        virtual QWidget* widget() const;

        virtual void load();
        virtual void save();
        virtual void reset();

    private:
        QString name_;
        RawRelation relation_;
        QWidget* widget_;
    };

    template< typename PropertyType, typename... Relation >
    struct PropertyRelationList;

    template< typename PropertyType, typename Relation, typename... OtherRelations >
    class PropertyRelationList<PropertyType, Relation, OtherRelations...>
    {
    public:
        typedef PropertyRelationList<PropertyType, OtherRelations...> Other;
        typedef QList<PropertyRelation<PropertyType>*> List;

        static List list()
        {
            if(!relation_)
            {
                relation_ = new Relation();
            }

            List list = Other::list();
            list.push_front(new Relation);
            return list;
        }

    private:
        static Relation* relation_;
    };

    template< typename PropertyType, typename Relation, typename... OtherRelations >
    Relation* PropertyRelationList<PropertyType, Relation, OtherRelations...>::relation_ = 0;

    template< typename PropertyType >
    struct PropertyRelationList<PropertyType>
    {
        static QList<PropertyRelation<PropertyType>*> list()
        {
            return QList<PropertyRelation<PropertyType>*>();
        }
    };

    template< typename PropertyType >
    struct PropertyRelations;

    DECLARE_SIMPLE_RELATION(equal_to, "=", std::equal_to<PropertyType>())
    DECLARE_SIMPLE_RELATION(not_equal_to, "!=", std::not_equal_to<PropertyType>())
    DECLARE_SIMPLE_RELATION(less, "<", std::less<PropertyType>())
    DECLARE_SIMPLE_RELATION(greater, ">", std::greater<PropertyType>())

    DECLARE_SIMPLE_RELATION(over, "over", &isOver)
    DECLARE_SIMPLE_RELATION(under, "under", &isUnder)
    DECLARE_SIMPLE_RELATION(on_the_left_of, "on the left of", &isOnTheLeftOf)
    DECLARE_SIMPLE_RELATION(on_the_right_of, "on the right of", &isOnTheRightOf)

    struct DistanceWidget : public QWidget
    {
        DistanceWidget();

        QSpinBox* distance;
    };

    template< typename PropertyType >
    class closer_than : public PropertyRelation<PropertyType>
    {
    public:
        typedef typename PropertyRelation<PropertyType>::RawRelation RawRelation;

        closer_than();

        RawRelation rawRelation() const;

        void load();
        void save();
        void reset();

    private:
        int distance_;

        DistanceWidget* widget_;
    };

    template< typename PropertyType >
    class farther_than : public PropertyRelation<PropertyType>
    {
    public:
        typedef typename PropertyRelation<PropertyType>::RawRelation RawRelation;

        farther_than();

        RawRelation rawRelation() const;

        void load();
        void save();
        void reset();

    private:
        int distance_;

        DistanceWidget* widget_;
    };

    DECLARE_RELATION_LIST(int,
        equal_to<int>,
        not_equal_to<int>,
        less<int>,
        greater<int>
    )

    DECLARE_RELATION_LIST(bool,
        equal_to<bool>,
        not_equal_to<bool>
    )

    DECLARE_RELATION_LIST(Point,
        over<Point>,
        under<Point>,
        on_the_left_of<Point>,
        on_the_right_of<Point>,
        closer_than<Point>,
        farther_than<Point>
    )

    class PropertyRelationSettingsWidget : public QWidget
    {
        Q_OBJECT

    public:
        PropertyRelationSettingsWidget();

        QComboBox* relation;

    private slots:
        void showWidget_(int index);

    private:
        int lastIndex_;
    };

    template< typename PropertyType >
    class PropertyRelationSettings
    {
    public:
        typedef typename PropertyRelation<PropertyType>::RawRelation RawRelation;

        PropertyRelationSettings();

        RawRelation relation() const;
        QWidget* widget() const;

        void load();
        void save();
        void reset();

    private:
        int relationIndex_;
        PropertyRelationSettingsWidget* widget_;

        QList<PropertyRelation<PropertyType>*> relations_;
    };
} // namespace Gecon

#include "PropertyRelationSettings.tpp"

#endif // GECON_PROPERTYRELATIONSETTINGS_HPP
