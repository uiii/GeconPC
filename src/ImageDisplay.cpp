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

#include "ImageDisplay.hpp"

namespace Gecon {
    
    ImageDisplay::ImageDisplay(QWidget *parent) :
        QLabel(parent)
    {
        reset();
    }

    void ImageDisplay::displayImage(const QImage &image)
    {
        setPixmap(QPixmap::fromImage(image));

        emit imageDisplayed();
    }

    void ImageDisplay::reset()
    {
        clear();
        setText(tr("Video"));
    }

    void ImageDisplay::mousePressEvent(QMouseEvent *ev)
    {
        emit clicked(ev);
    }
    
} // namespace Gecon
