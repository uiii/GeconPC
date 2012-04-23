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

#include "PropertyRelationSettings.hpp"

#include <functional>

#include <QHBoxLayout>

namespace Gecon
{
    PropertyRelationSettingsWidget::PropertyRelationSettingsWidget():
        lastIndex_(-1)
    {
        relation = new QComboBox(this);

        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(relation);

        connect(relation, SIGNAL(currentIndexChanged(int)), this, SLOT(showWidget_(int)));
    }

    void PropertyRelationSettingsWidget::showWidget_(int index)
    {
        if(lastIndex_ > -1)
        {
            QWidget* widget = relation->itemData(lastIndex_, Qt::UserRole).value<QWidget*>();

            if(widget)
            {
                widget->hide();
            }
        }

        QWidget* widget = relation->itemData(index, Qt::UserRole).value<QWidget*>();
        if(widget)
        {
            widget->show();
        }

        lastIndex_ = index;
    }

    DistanceWidget::DistanceWidget()
    {
        distance = new QSpinBox(this);

        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(distance);
    }
}
