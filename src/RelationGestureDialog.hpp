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

#ifndef GECON_RELATIONGESTUREDIALOG_HPP
#define GECON_RELATIONGESTUREDIALOG_HPP

#include <QDialog>
#include <QList>

namespace Gecon
{
    namespace Ui
    {
        class RelationGestureDialog;
    }

    class ObjectRelation; // forward declaration
    class RelationGestureWrapper;
    class ObjectModel;
    class GestureModel;
    class TestDialog;

    class RelationGestureDialog : public QDialog
    {
        Q_OBJECT
        
    public:
        explicit RelationGestureDialog(GestureModel* gestureModel, ObjectModel* objectModel, TestDialog* testDialog, QWidget *parent = 0);
        ~RelationGestureDialog();

    public slots:
        int newGesture();
        int editGesture(RelationGestureWrapper* relationGesture);

        void addGesture();
        void updateGesture();
        void deleteGesture();

        void testGesture();
        void deleteTestedGesture();

        void reset();

        void setRelationSettings(int index);

        int exec();
        
    private:
        typedef QList<ObjectRelation*> ObjectRelationList;

        void initLeftPropertyComboBox_();

        ObjectModel* objectModel_;
        GestureModel* gestureModel_;

        TestDialog* testDialog_;
        RelationGestureWrapper* testedGesture_;

        RelationGestureWrapper* editedGesture_;

        ObjectRelationList relations_;
        ObjectRelation* currentRelation_;

        Ui::RelationGestureDialog *ui_;
    };
} // namespace Gecon
#endif // GECON_RELATIONGESTUREDIALOG_HPP
