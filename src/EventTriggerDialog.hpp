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

#ifndef GECON_EVENTTRIGGERDIALOG_HPP
#define GECON_EVENTTRIGGERDIALOG_HPP

#include <QDialog>
#include <QStringList>

#include "EventWrapper.hpp"

namespace Gecon
{
    namespace Ui
    {
        class EventTriggerDialog;
    }

    class EventTriggerModel;
    class EventTriggerWrapper;
    class GestureModel;
    class ObjectModel;
    class GestureEventDialog;
    class ActionSettings;

    class EventTriggerDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit EventTriggerDialog(EventTriggerModel* eventTriggerModel_, GestureModel* gestureModel, ObjectModel *objectModel, QWidget *parent);
        ~EventTriggerDialog();

    public slots:
        int newTrigger();
        int editTrigger(Gecon::EventTriggerWrapper *trigger);

        int exec();

    private slots:
        void addTrigger_();
        void updateTrigger_();
        void deleteTrigger_();

        void addSwitch_();
        void removeSwitch_();

        void setActionSettings_(int index);

        void reset_();

    private:
        EventTriggerModel* eventTriggerModel_;
        GestureModel* gestureModel_;
        ObjectModel* objectModel_;

        GestureEventDialog* eventDialog_;

        QList<EventWrapper*> onEvents_;
        QList<EventWrapper*> offEvents_;
        QStringList switchDescriptions_;

        EventTriggerWrapper* editedTrigger_;

        QList<ActionSettings*> actions_;
        ActionSettings* currentAction_;

        Ui::EventTriggerDialog* ui_;
    };
}

#endif // GECON_EVENTTRIGGERDIALOG_HPP
