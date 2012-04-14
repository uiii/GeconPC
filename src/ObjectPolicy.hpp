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

#ifndef GECON_OBJECTPOLICY_HPP
#define GECON_OBJECTPOLICY_HPP

#include <memory>

#include <QObject>

#include <Gecon/ColorObjectPolicy.hpp>
#include <Gecon/Image.hpp>

namespace Gecon
{
    class ObjectPolicySignaler : public QObject
    {
        Q_OBJECT

    public:
        typedef Gecon::Image<RGB> Image;
        typedef ColorObjectPolicy::ObjectSet ObjectSet;

        void emitObjectsRecognized(Image original, Image segmented, const ObjectSet& objects);

    signals:
        void objectsRecognized(Image original, Image segmented, const ObjectSet& objects);
    };

    class ObjectPolicy : public ColorObjectPolicy
    {
    public:
        typedef Gecon::Image<RGB> Image;

        ObjectPolicy();

        ObjectSet recognizeObjects(const Image& image);

        const std::shared_ptr<ObjectPolicySignaler>& signaler() const;

    private:
        std::shared_ptr<ObjectPolicySignaler> signaler_;
    };
} // namespace Gecon

#endif // GECON_OBJECTPOLICY_HPP
