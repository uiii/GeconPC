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

namespace Gecon
{
    namespace Ui
    {
        class StateGestureDialog;
    }

    class StateGestureWrapper; // forward declaration
    class GestureModel;
    class ObjectModel;
    class ObjectState;
    class TestDialog;

    class StateGestureDialog : public QDialog
    {
        Q_OBJECT
        
    public:
        explicit StateGestureDialog(GestureModel* gestureModel, ObjectModel* objectModel, TestDialog* testDialog, QWidget *parent);
        ~StateGestureDialog();

    public slots:
        int newGesture();
        int editGesture(StateGestureWrapper* stateGesture);

        void addGesture();
        void updateGesture();
        void deleteGesture();

        void testGesture();
        void deleteTestedGesture();

        void reset();

        void setObjectStateSettings(int index);

        int exec();

    private:
        typedef QList<ObjectState*> PropertyStateList;

        void initPropertyComboBox();

        GestureModel* gestureModel_;
        ObjectModel* objectModel_;

        TestDialog* testDialog_;
        StateGestureWrapper* testedGesture_;

        StateGestureWrapper* editedGesture_;

        PropertyStateList states_;
        ObjectState* currentState_;

        Ui::StateGestureDialog *ui_;
    };
} // namespace Gecon
#endif // GECON_STATEGESTUREDIALOG_HPP
