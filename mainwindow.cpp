#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    translator = new QTranslator(this);
    qApp->installTranslator(translator);

    ui->plainTextEdit->setFocus();
    createMenu();
}

void MainWindow::createMenu()
{
    QMenu *menu = menuBar()->addMenu(tr("Файл"));

    QAction *newFileActionMenu = menu->addAction(tr("Новый файл"));
    newFileActionMenu->setShortcut(Qt::ControlModifier | Qt::Key_N);
    connect(newFileActionMenu, &QAction::triggered, this, &MainWindow::createNewDoc);

    QAction *openActionMenu = menu->addAction(tr("Открыть..."));
    openActionMenu->setShortcut(Qt::ControlModifier | Qt::Key_O);
    connect(openActionMenu, &QAction::triggered, this, &MainWindow::on_openButton_clicked);

    QAction *saveActionMenu = menu->addAction(tr("Сохранить..."));
    saveActionMenu->setShortcut(Qt::ControlModifier | Qt::Key_S);
    connect(saveActionMenu, &QAction::triggered, this, &MainWindow::on_saveButton_clicked);

    QAction *printActionMenu = menu->addAction(tr("Печать..."));
    printActionMenu->setShortcut(Qt::ControlModifier | Qt::Key_P);
    connect(printActionMenu, &QAction::triggered, this, &MainWindow::printText);

    QAction *exitActionMenu = menu->addAction(tr("Выход"));
    exitActionMenu->setShortcut(Qt::ControlModifier | Qt::Key_Q);
    connect(exitActionMenu, &QAction::triggered, this, &MainWindow::closeFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_saveButton_clicked()
{
    QString const s = QFileDialog::getSaveFileName(this, tr("Окно сохранения"), QDir::current().path(), filter);
    if (s.length() > 0) {
        QString ext = QString(&(s.data()[s.length() - 4]));
        if (".txt" == ext) {
            QFile file(s);
            if (file.open(QFile::WriteOnly | QFile::NewOnly)) {
                QTextStream stream (&file);
                stream << ui->plainTextEdit->toPlainText();
                file.close();
            }
        }
    }
}

void MainWindow::on_radioButtonOnlyRead_clicked(bool checked)
{
    if (checked){
        checkOpenButton = true;
    }else {
        checkOpenButton = false;
        ui->plainTextEdit->setReadOnly(checkOpenButton);
    }
}

void MainWindow::on_openButton_clicked()
{
    QString const s = QFileDialog::getOpenFileName(this, tr("Окно открытия"), QDir::current().path(), filter);
    if(s.length() > 0) {
        int index = s.indexOf(".txt");
        if (index !=-1 && s.length()-4 == index) {
            QFile file(s);
            if (!checkOpenButton){
                if (file.open(QFile::ReadOnly | QFile::ExistingOnly)) {
                    QTextStream stream(&file);
                    ui->plainTextEdit->setPlainText(stream.readAll());
                    file.close();
                }
            } else {
                if (file.open(QFile::ReadOnly)) {
                    QTextStream stream(&file);
                    ui->plainTextEdit->setPlainText(stream.readAll());
                    ui->plainTextEdit->setReadOnly(checkOpenButton);
                    file.close();
                }
            }
        }
    }
}

void MainWindow::on_descriptionButton_clicked()
{
    description = new QTextEdit;
    description->setWindowTitle(tr("Справка"));
    description->setReadOnly(true);
    QString s = "://Description.txt";
    QFile file(s);
    if (file.open(QFile::ReadOnly)) {
        QTextStream stream(&file);
        description->setPlainText(stream.readAll());
        file.close();
    }
    description->show();
}

void MainWindow::changeTranslator(QString postfix)
{
    QApplication::removeTranslator(translator);
    translator = new QTranslator(this);
    static_cast<void>(translator->load("D:/Qt projects/Lesson_5/QtLanguage_" + postfix));
    QApplication::installTranslator(translator);
}

bool MainWindow::warningWindow()
{
    QMessageBox choise;
    choise.setWindowTitle(tr("Вы уверены?"));
    choise.setText(tr("Все несохраненные данные будут утеряны!"));
    choise.addButton(tr("Да"), QMessageBox::YesRole);
    choise.addButton(tr("Нет"), QMessageBox::NoRole);
    if (choise.exec() == false){
         return true;
    } else {
        choise.close();
        return false;
    }
}

void MainWindow::closeFile()
{
    if(ui->plainTextEdit->toPlainText().isEmpty())
    {
        MainWindow::close();
    }
    else if(warningWindow())
        MainWindow::close();
}

void MainWindow::printText()
{
    QPrinter printer;
    QPrintDialog dlg(&printer, this);
    dlg.setWindowTitle(tr("Печать"));
    if (dlg.exec() != QDialog::Accepted)
        return;
    ui->plainTextEdit->print(&printer);
}

void MainWindow::on_comboBoxLanguages_currentIndexChanged(int index)
{
    if (index == 0) {
        changeTranslator("ru");
    }
    if (index == 1) {
        changeTranslator("en");
    }
}

void MainWindow::createNewDoc()
{
    if(warningWindow())
    {
        ui->plainTextEdit->clear();
        ui->radioButtonOnlyRead->setChecked(0);
    }
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        setWindowTitle(tr("Главное окно"));
        ui->descriptionButton->setText(tr("Справка"));
        ui->radioButtonOnlyRead->setText(tr("Только для чтения"));
        ui->descriptionButton->setToolTip(tr("Нажмите для подсказки"));
        ui->comboBoxLanguages->setItemText(0, tr("Русский"));
        ui->comboBoxLanguages->setItemText(1, tr("Английский"));
        ui->radioButtonLightTheme->setText(tr("Светлая тема"));
        ui->radioButtonDarkTheme->setText(tr("Темная тема"));
    } else QMainWindow::changeEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event){

   if (event->type() == QKeyEvent::KeyPress) {
        if (event->modifiers() == Qt::ControlModifier){
            if (event->key() == Qt::Key_O)
            {
               on_openButton_clicked();
            }
            else if (event->key() == Qt::Key_S)
            {
               on_saveButton_clicked();
            }
            else if (event->key() == Qt::Key_Q)
            {
               closeFile();
            }
            else if (event->key() == Qt::Key_N)
            {
               createNewDoc();
            }
            else if (event->key() == Qt::Key_P)
            {
                printText();
            }
        }
    }
}

void MainWindow::on_radioButtonDarkTheme_clicked()
{
    QString darkButton = ("QPushButton { color: rgb(255, 255, 255); border: 1px solid black; }");
    QString darkMainW = ("QMainWindow { color: rgb(255, 255, 255); background-color: grey; border: 1px solid black;  }"
                         "QMainWindow::title { background-color: rgb(49, 49, 49);}");
    QString darkWidget = ("QWidget { color: rgb(255, 255, 255); background-color: rgb(49, 49, 49); }");
    QString darkRadioButton = ("QRadioButton { color: rgb(255, 255, 255);");
    QString darkComboBox = ("QComboBox { color: rgb(255, 255, 255); }");
    QString darkPlainText = ("QPlainTexEdit { color: rgb(255, 255, 255);"
                             "selection-background-color: rgb(154, 154, 154);"
                             "border: 1px solid black;  }");
    qApp->setStyleSheet( darkButton + darkMainW + darkWidget +
                         darkRadioButton + darkComboBox + darkPlainText);
}

void MainWindow::on_radioButtonLightTheme_clicked()
{
    qApp->setStyleSheet("");
}

void MainWindow::on_printButton_clicked()
{
    printText();
}

void MainWindow::on_newFileButton_clicked()
{
    createNewDoc();
}

void MainWindow::on_leftAligmentButton_clicked()
{
    ui->plainTextEdit->setAlignment(Qt::AlignLeft);
    ui->plainTextEdit->setFocus();
}

void MainWindow::on_centralAligmentButton_clicked()
{
    ui->plainTextEdit->setAlignment(Qt::AlignCenter);
    ui->plainTextEdit->setFocus();
}

void MainWindow::on_rightAligmentButton_clicked()
{
    ui->plainTextEdit->setAlignment(Qt::AlignRight);
    ui->plainTextEdit->setFocus();
}

void MainWindow::on_widthAligmentButton_clicked()
{
    ui->plainTextEdit->setAlignment(Qt::AlignJustify);
    ui->plainTextEdit->setFocus();
}

void MainWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
    QTextCharFormat format;
    format.setFont(f);
    if(ui->plainTextEdit->textCursor().hasSelection())
    {
        ui->plainTextEdit->textCursor().setCharFormat(format);
    }else
    {
        ui->plainTextEdit->setCurrentCharFormat(format);
    }
    ui->plainTextEdit->setFocus();
}

void MainWindow::on_sizeFontBox_valueChanged(int arg1)
{
    ui->plainTextEdit->setFontPointSize(arg1);
}

void MainWindow::on_plainTextEdit_cursorPositionChanged()
{
    QTextCursor cursor = ui->plainTextEdit->textCursor();
    if(!cursor.hasSelection())
        ui->fontComboBox->setCurrentIndex(ui->fontComboBox->findText(QFontInfo(cursor.charFormat().font()).family()));
    ui->sizeFontBox->setValue(cursor.charFormat().font().pointSize());

    if (ui->plainTextEdit->alignment() == Qt::AlignmentFlag::AlignCenter)
        ui->centralAligmentButton->setChecked(true);
    else if (ui->plainTextEdit->alignment() == Qt::AlignmentFlag::AlignLeft)
        ui->leftAligmentButton->setChecked(true);
    else if (ui->plainTextEdit->alignment() == Qt::AlignmentFlag::AlignRight)
        ui->rightAligmentButton->setChecked(true);
    else if (ui->plainTextEdit->alignment() == Qt::AlignmentFlag::AlignJustify)
        ui->widthAligmentButton->setChecked(true);
}
