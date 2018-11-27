#ifndef RENAMEPBTN_HPP
#define RENAMEPBTN_HPP

#include <QPushButton>

class renamepbtn : public QPushButton
{
    Q_OBJECT
private:
    QString mapkey;
public:
    void set_key(QString nkey);
    renamepbtn(const QIcon &icon, QString key);
    ~renamepbtn();
public slots:
    void slot_clicked(bool checked = false);
signals:
    void idclicked(QString key);
};

#endif // RENAMEPBTN_HPP
