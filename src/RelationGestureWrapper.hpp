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

#ifndef GECON_RELATIONGESTUREWRAPPER_HPP
#define GECON_RELATIONGESTUREWRAPPER_HPP

#include "ObjectWrapper.hpp"
#include "ObjectRelationSettings.hpp"

#include "GestureWrapper.hpp"

#include <Gecon/ObjectRelationGesture.hpp>

namespace Gecon
{
    class RelationGestureDialog; // forward declaration

    class RelationGestureWrapper : public GestureWrapper
    {
    public:
        static RelationGestureDialog* dialog;

        RelationGestureWrapper(const QString& name, ObjectWrapper* leftObject, ObjectWrapper* rightObject, ObjectRelation* relation);
        RelationGestureWrapper(const RelationGestureWrapper& another);

        virtual ~RelationGestureWrapper();

        const Events& events() const;

        ObjectWrapper* leftObject();
        ObjectWrapper* rightObject();
        ObjectRelation* relation();

        void setLeftObject(ObjectWrapper* object);
        void setRightObject(ObjectWrapper* object);
        void setRelation(ObjectRelation* relation);

        void edit();

        ControlInfo::RelationGesture* rawGesture();

    public:
        Events events_;

        ObjectWrapper* left_;
        ObjectWrapper* right_;
        ObjectRelation* relation_;

        ControlInfo::RelationGesture* rawGesture_;
    };
} // namespace Gecon

#endif // GECON_RELATIONGESTUREWRAPPER_HPP
