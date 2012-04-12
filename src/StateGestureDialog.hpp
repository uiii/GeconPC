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

#ifndef GECON_STATEGESTUREDIALOG_HPP
#define GECON_STATEGESTUREDIALOG_HPP

#include <QDialog>

//#include "Gecon/ColorObject.hpp"
//#include "Gecon/ObjectStateCondition.hpp"

#include "GestureModel.hpp"
#include "ObjectModel.hpp"

namespace Gecon
{
    namespace Ui
    {
        class StateGestureDialog;
    }

    class StateGestureWrapper; // forward declaration

    class StateGestureDialog : public QDialog
    {
        Q_OBJECT
        
    public:
        explicit StateGestureDialog(GestureModel* gestureModel, ObjectModel* objectModel, QWidget *parent = 0);
        ~StateGestureDialog();

    public slots:
        int newGesture();
        int editGesture(StateGestureWrapper* stateGesture);

        void addGesture();
        void updateGesture();
        void deleteGesture();

        void reset();

        void pack();
        void setPropertyStateSettings(int index);

        int exec();

    private:
        typedef QList<ObjectPropertyStateSettings*> ObjectPropertyStateSettingsList;

        void initObjectComboBox(ObjectModel* objectModel);
        void initPropertyComboBox();

        GestureModel* gestureModel_;
        ObjectModel* objectModel_;

        StateGestureWrapper* editedGesture_;

        ObjectPropertyStateSettingsList stateSettings_;
        ObjectPropertyStateSettings* currentStateSettings_;

        Ui::StateGestureDialog *ui_;
    };
} // namespace Gecon
#endif // GECON_STATEGESTUREDIALOG_HPP
