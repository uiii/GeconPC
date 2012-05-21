#ifndef GECON_DIALOGBASE_HPP
#define GECON_DIALOGBASE_HPP

#include <QDialog>

namespace Gecon
{
    class DialogBase : public QDialog
    {
        Q_OBJECT
    public:
        explicit DialogBase(QWidget *parent = 0);
        
    signals:
        
    public slots:
        void accept();
        void reject();

    protected:
        void closeEvent(QCloseEvent *);

    protected slots:
        virtual void beforeClose_() = 0;
    };
} // namespace Gecon

#endif // GECON_DIALOGBASE_HPP
