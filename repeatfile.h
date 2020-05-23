#ifndef REPEATFILE_H
#define REPEATFILE_H

#include <QDialog>

namespace Ui {
class repeatFile;
}

class repeatFile : public QDialog
{
    Q_OBJECT

public:
    explicit repeatFile(QWidget *parent = nullptr);
    ~repeatFile();

private slots:
    void on_repeatFileOk_clicked();

    void on_repeatFolder_currentIndexChanged();

private:
    Ui::repeatFile *ui;
};

#endif // REPEATFILE_H
