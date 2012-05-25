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

#include "PropertyValueSettings.hpp"

#include <QLabel>
#include <QHBoxLayout>

namespace Gecon
{
    PropertyValueSettings<int>::Widget::Widget():
        value(new QSpinBox)
    {
        value->setMaximum(10000);

        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(value);
    }

    PropertyValueSettings<int>::PropertyValueSettings():
        value_(0),
        widget_(new Widget)
    {
    }

    int PropertyValueSettings<int>::value() const
    {
        return value_;
    }

    QWidget *PropertyValueSettings<int>::widget() const
    {
        return widget_;
    }

    void PropertyValueSettings<int>::load()
    {
        widget_->value->setValue(value_);
    }

    void PropertyValueSettings<int>::save()
    {
        value_ = widget_->value->value();
    }

    void PropertyValueSettings<int>::reset()
    {
        widget_->value->setValue(0);
    }

    PropertyValueSettings<double>::Widget::Widget():
        value(new QDoubleSpinBox)
    {
        value->setMaximum(10000);
        value->setDecimals(2);
        value->setSingleStep(0.1);

        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(value);
    }

    PropertyValueSettings<double>::PropertyValueSettings():
        value_(0.0),
        widget_(new Widget)
    {
    }

    double PropertyValueSettings<double>::value() const
    {
        return value_;
    }

    QWidget *PropertyValueSettings<double>::widget() const
    {
        return widget_;
    }

    void PropertyValueSettings<double>::load()
    {
        widget_->value->setValue(value_);
    }

    void PropertyValueSettings<double>::save()
    {
        value_ = widget_->value->value();
    }

    void PropertyValueSettings<double>::reset()
    {
        widget_->value->setValue(0.0);
    }

    PropertyValueSettings<Fraction>::Widget::Widget():
        nominator(new QSpinBox),
        denominator(new QSpinBox)
    {
        nominator->setMaximum(10000);
        nominator->setMinimum(1);
        denominator->setMaximum(10000);
        denominator->setMinimum(1);

        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(nominator);
        layout->addWidget(new QLabel("/"));
        layout->addWidget(denominator);
    }

    PropertyValueSettings<Fraction>::PropertyValueSettings():
        value_(Fraction(1,1)),
        widget_(new Widget)
    {
    }

    Fraction PropertyValueSettings<Fraction>::value() const
    {
        return value_;
    }

    QWidget *PropertyValueSettings<Fraction>::widget() const
    {
        return widget_;
    }

    void PropertyValueSettings<Fraction>::load()
    {
        widget_->nominator->setValue(value_.nominator);
        widget_->denominator->setValue(value_.denominator);
    }

    void PropertyValueSettings<Fraction>::save()
    {
        value_ = Fraction(
            widget_->nominator->value(),
            widget_->denominator->value()
        );
    }

    void PropertyValueSettings<Fraction>::reset()
    {
        widget_->nominator->setValue(1);
        widget_->denominator->setValue(1);
    }

    PropertyValueSettings<bool>::Widget::Widget():
        value(new QComboBox)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(value);
    }

    PropertyValueSettings<bool>::PropertyValueSettings():
        PropertyValueSettings<bool>("true", "false")
    {
    }

    PropertyValueSettings<bool>::PropertyValueSettings(const QString& trueOption, const QString& falseOption):
        optionIndex_(0),
        widget_(new Widget)
    {
        widget_->value->addItem(trueOption, true);
        widget_->value->addItem(falseOption, false);
    }

    bool PropertyValueSettings<bool>::value() const
    {
        return widget_->value->itemData(optionIndex_).value<bool>();
    }

    QWidget *PropertyValueSettings<bool>::widget() const
    {
        return widget_;
    }

    void PropertyValueSettings<bool>::load()
    {
        widget_->value->setCurrentIndex(optionIndex_);
    }

    void PropertyValueSettings<bool>::save()
    {
        optionIndex_ = widget_->value->currentIndex();
    }

    void PropertyValueSettings<bool>::reset()
    {
        widget_->value->setCurrentIndex(0);
    }

    PropertyValueSettings<Point>::Widget::Widget():
        xValue(new QSpinBox),
        yValue(new QSpinBox)
    {
        xValue->setMaximum(10000);
        yValue->setMaximum(10000);

        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(xValue);
        layout->addWidget(yValue);
    }

    PropertyValueSettings<Point>::PropertyValueSettings():
        value_(Point(0.0, 0.0)),
        widget_(new Widget)
    {
    }

    Point PropertyValueSettings<Point>::value() const
    {
        std::cout << "value: " << value_.x << " " << value_.y << std::endl;
        return Point(value_.x / 100.0, value_.y / 100.0);
    }

    QWidget *PropertyValueSettings<Point>::widget() const
    {
        return widget_;
    }

    void PropertyValueSettings<Point>::load()
    {
        widget_->xValue->setValue(value_.x);
        widget_->yValue->setValue(value_.y);
    }

    void PropertyValueSettings<Point>::save()
    {
        value_ = Point(widget_->xValue->value(), widget_->yValue->value());
    }

    void PropertyValueSettings<Point>::reset()
    {
        widget_->xValue->setValue(0);
        widget_->yValue->setValue(0);
    }
} // namespace Gecon
