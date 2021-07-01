#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QPushButton>
#include <QWebChannel>
#include <QDesktopServices>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStyle>
#include <QTranslator>
#include <QTimer>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickWindow>

#include "parser.h"
#include "util.h"
#include "record.h"
#include "finder.h"
#include "webpage.h"
#include "loader.h"
#include "configmanager.h"
#include "application.h"

#include "widgets/statuslabel.h"
#include "dialogs/parsedialog.h"
#include "dialogs/settingsdialog.h"
#include "dialogs/downloaddialog.h"
#include "dialogs/statusdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->webview);
    if (App->config->contains("lastGeometry"))
        setGeometry(App->config->value("lastGeometry").toRect());
    m_finder = new Finder(this);
    ui->webview->registerObject("finder", m_finder);
    ui->webview->setUrl(QUrl("qrc:/web/index.html"));
    connect(ui->webview, &WebView::loadFinished,
            this, [this]() {
        show();
        disconnect(ui->webview, &WebView::loadFinished,
                   nullptr, nullptr);
    });
    connect(m_finder, &Finder::notReady,
            this, &MainWindow::on_actionStatus_triggered);
    connect(ui->webview->page(), &WebPage::request,
            m_finder, &Finder::handleRequest);
    load();
    addStatusIcon();
}

MainWindow::~MainWindow()
{
    App->config->setValue("lastGeometry", geometry());
    delete ui;
}

void MainWindow::showAboutBox(QPixmap pixmapIcon, const QString &info)
{
    auto box = new QMessageBox(this);
    box->setModal(false);
    box->setText(info);
    box->setIconPixmap(pixmapIcon.scaled(64, 64));
    box->setStandardButtons(QMessageBox::Ok);
    box->button(QMessageBox::Ok)->setText(tr("OK"));
    box->setDefaultButton(QMessageBox::Ok);
    box->show();
}

void MainWindow::setTranslator(QTranslator *translator_)
{
    translator = translator_;
}

void MainWindow::on_actionAboutQt_triggered()
{
    QIcon icon = style()->standardIcon(QStyle::SP_TitleBarMenuButton);
    showAboutBox(icon.pixmap(64), tr(
        "<b>About Qt</b><br><br>"
        "This program uses Qt version %1.<br><br>"
        "Qt is a C++ toolkit for cross-platform application development.<br><br>"
        "See <a href=\"https://www.qt.io/\">qt.io</a> for more information."
    ).arg(QT_VERSION_STR));
}

void MainWindow::on_actionAboutDBLParse_triggered()
{
    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl("qrc:/qml/AboutDBLParse.qml"));
    auto dialog = qobject_cast<QQuickWindow*>(component.create());
    dialog->show();
}

void MainWindow::on_actionAboutDBLP_triggered()
{
    showAboutBox(QPixmap(":/resources/dblp.png"), tr(
        "<b>DBLP</b><br><br>"
        "The <em>dblp computer science bibliography</em> provides "
        "open bibliographic information on major computer science "
        "journals and proceedings. Originally created at the "
        "<a href=\"https://www.uni-trier.de/\">University of Trier</a> "
        "in 1993, dblp is now operated and further developed by "
        "<a href=\"https://www.dagstuhl.de/\">Schloss Dagstuhl</a>."
        "<br/><br/>For more information <a href=\"https://dblp.uni-trier.de/faq/\">"
        "check out our F.A.Q.</a>"
    ));
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionOpen_triggered()
{
    QString lastOpenFileName = App->config->value("lastOpenFileName").toString();
    QString fileName;
    {
        QFileDialog dialog(this);
        dialog.setWindowTitle(tr("Open DBLP XML File"));
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.selectFile(lastOpenFileName);
        dialog.setDirectory(lastOpenFileName);
        dialog.setNameFilter(tr("XML file (*.xml)"));
        if (dialog.exec()) fileName = dialog.selectedFiles().first();
        else return ;
    }
    if(fileName.isEmpty()) return ;
    open(fileName);
}

void MainWindow::on_actionStatus_triggered()
{
    StatusDialog dialog(this);
    dialog.exec();
    /*
    QMessageBox box(this);
    QString text;
    QString parserStatus = Util::parsed()
                         ? "<font color=\"green\">OK</font>"
                         : "<font color=\"red\">NO</font>";
    QString loaderStatus = m_finder->loaded()
                         ? "<font color=\"green\">OK</font>"
                         : "<font color=\"red\">NO</font>";
    text = tr("Parser: %1 <br>Loader: %2").arg(parserStatus, loaderStatus);
    box.setText(text);
    if (Util::parsed()) {
        box.setStandardButtons(QMessageBox::Ok);
        box.button(QMessageBox::Ok)->setText(tr("OK"));
        box.setDefaultButton(QMessageBox::Ok);
    } else {
        box.setStandardButtons(QMessageBox::Open|QMessageBox::Cancel);
        box.button(QMessageBox::Open)->setText(tr("Open XML file"));
        box.button(QMessageBox::Cancel)->setText(tr("Cancel"));
        box.setDefaultButton(QMessageBox::Cancel);
    }
    if (box.exec() == QMessageBox::Open) on_actionOpen_triggered();
    */
}

void MainWindow::on_actionClearIndex_triggered()
{
    Util::clearIndexs();
    m_finder->clearIndex();
    statusBar()->showMessage(tr("Clear index file successful!"));
    statusLabel->setNo();
}

void MainWindow::on_actionOpenIndexFolder_triggered()
{
    QDesktopServices::openUrl(QUrl(QDir::currentPath()));
}

void MainWindow::load()
{
    if(!Util::parsed()) return ;
    auto loader = new Loader(m_finder);
    auto thread = new QThread();
    loader->moveToThread(thread);
    connect(thread, &QThread::finished,
            loader, &QObject::deleteLater);
    connect(loader, &Loader::stateChanged,
            this, [this](const QString &state) {
        statusBar()->showMessage(state);
    });
    connect(thread, &QThread::finished,
            thread, &QObject::deleteLater);
    
    connect(loader, &Loader::authorStacLoadDone,
            m_finder, &Finder::setAuthorStacLoaded);
    connect(loader, &Loader::yearWordLoadDone,
            m_finder, &Finder::setYearWordLoaded);
    connect(loader, &Loader::loadDone,
            m_finder, &Finder::setLoaded);
    connect(loader, &Loader::loadDone,
            this, [this]() {
        statusBar()->showMessage(tr("Load finished."), 3000); 
        statusLabel->setOk();
    });
    thread->start();
    QTimer::singleShot(0, loader, &Loader::run);
    m_finder->init();
}

void MainWindow::open(const QString &fileName)
{
    App->config->setValue("lastOpenFileName", fileName);
    App->config->setValue("lastDateTime", QDateTime::currentDateTime());
    // question when size greater than 64MiB
    if(QFile(fileName).size() > PROMOT_FILE_SIZE){
        QMessageBox box(this);
        box.resize(500, 170);
        box.setText(tr("Parsing the file will last for a while "
                       "and will take up a lot of memory."));
        box.setInformativeText(tr("Do you want to continue?"));
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        box.setDefaultButton(QMessageBox::Yes);
        box.button(QMessageBox::Yes)->setText(tr("Yes"));
        box.button(QMessageBox::No)->setText(tr("No"));
        if(box.exec() == QMessageBox::No) return ;
    }
    Util::clearIndexs();
    auto *dialog = new ParseDialog(this);
    auto thread = new QThread();
    auto parser = new Parser();
    parser->moveToThread(thread);
    connect(thread, &QThread::finished,
            parser, &QObject::deleteLater);
    connect(parser, &Parser::stateChanged,
            dialog, &ParseDialog::setState);
    connect(parser, &Parser::done,
            dialog, &ParseDialog::handleDone);
    connect(parser, &Parser::done,
            thread, &QThread::quit);
    connect(thread, &QThread::finished,
            thread, &QObject::deleteLater);
    connect(parser, &Parser::done,
            this, &MainWindow::load);
    dialog->open();
    thread->start();
    QTimer::singleShot(0, parser, &Parser::run);
}

void MainWindow::onLanguageChanged(const QString &locale)
{
    QLocale::setDefault(locale);
    translator->load("DBLParse_" + locale, ":/");
}

void MainWindow::addStatusIcon()
{
    statusLabel = new StatusLabel;
    connect(statusLabel, &StatusLabel::clicked,
            this, &MainWindow::on_actionStatus_triggered);
    ui->statusbar->addPermanentWidget(statusLabel);
}

void MainWindow::on_actionAuthorStac_triggered()
{
    static const int TOP_K = 100;
    if (!Util::parsed() || !m_finder->authorStacLoaded()) {
        on_actionStatus_triggered();
        return ;
    }

    auto *view = new WebView(this);
    view->setWindowFlag(Qt::Window);
    view->resize(600, 800);
    
    QVector<AuthorStac> authorStac = m_finder->authorStacs;
    QJsonArray authorStacArray;
    
    int num = authorStac.size() <= TOP_K ? authorStac.size() : TOP_K;
    for (qint32 t=0; t<num; t++) {
        QJsonObject obj;
        obj.insert("author", QString(authorStac[t].author));
        obj.insert("articleNum",QString::number(authorStac[t].stac));
        authorStacArray.append(obj);
    }
    
    auto html = Util::readFile(":/web/authorStac.html");
    auto data = QJsonDocument(authorStacArray).toJson();
    
    html.replace("<!-- DATA_HOLDER -->", data);
    view->setHtml(html, QUrl("qrc:/web/"));
    view->show();
}

void MainWindow::on_actionViewLog_triggered()
{
#ifdef QT_NO_DEBUG
    QDesktopServices::openUrl(QUrl::fromLocalFile(Util::getLogPath()));
#endif
}

void MainWindow::on_actionSettings_triggered()
{
    auto *dialog = new SettingsDialog(this);
    connect(dialog, &SettingsDialog::languageChanged,
            this, &MainWindow::onLanguageChanged);
    connect(dialog, &SettingsDialog::languageChanged,
            m_finder, &Finder::languageChanged);
    dialog->open();
}

void MainWindow::on_actionKeyWord_triggered()
{
    if(!Util::parsed() || !m_finder->yearWordLoaded()){
        on_actionStatus_triggered();
        return ;
    }

     auto *view = new WebView(this);
     view->setWindowFlag(Qt::Window);
     view->registerObject("finder", m_finder);
     view->resize(800,600);
     connect(view->page(), &WebPage::wordCloud,
             m_finder, &Finder::handleWordCloud);

     YearWord yearWord = m_finder->yearWord;
     auto it= yearWord.begin();
     QJsonArray yearWordArray;

     while (it != yearWord.end()) {
         QJsonObject obj;
         obj.insert("year", it.key());
         QJsonArray arr;
         for (auto &cw : it.value()) {
             QJsonObject o;
             o.insert("count", cw.count);
             o.insert("word", QString(cw.word));
             arr.append(o);
         }
         obj.insert("words", arr);
         yearWordArray.append(obj);
         it++;
     }

     auto html = Util::readFile(":/web/yearWord.html");
     auto data = QJsonDocument(yearWordArray).toJson();

     html.replace("<!-- DATA_HOLDER -->", data);
     view->setHtml(html, QUrl("qrc:/web/"));
     view->show();
}

void MainWindow::on_actionCountClique_triggered()
{
    if(!Util::parsed()){
        on_actionStatus_triggered();
        return ;
    }
    QFile file("data/authorclique.txt");
    auto *view = new WebView(this);
    view->setWindowFlag(Qt::Window);
    view->resize(850, 600);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream in(&file);
    QJsonObject o;
    int total;
    in >> total;
    for (int i = 1; i <= total; ++i) {
        QString n;
        QString cnt;
        in >> n >> cnt;
        o.insert(n, cnt);
    }
    in >> total;
    o.insert("total", total);
    auto html = Util::readFile(":/web/clique.html");
    auto data = QJsonDocument(o).toJson();
    html.replace("<!-- DATA_HOLDER -->", data);
    view->setHtml(html, QUrl("qrc:/web/"));
    view->show();
}

void MainWindow::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
    QMainWindow::changeEvent(e);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (!e->mimeData()->hasUrls()) return ;
    if (e->mimeData()->urls().size() != 1) return ;
    if (!e->mimeData()->urls().first().toLocalFile().endsWith(".xml")) return ;
    e->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *e)
{
    open(e->mimeData()->urls().first().toLocalFile());
}

void MainWindow::on_actionOpenDataFolder_triggered()
{
    if (Util::parsed()) {
        Util::openFolder(Util::getXmlFileName());
    } else {
        auto box = new QMessageBox(this);
        box->setText(tr("Do not find data file."));
        box->show();
    }
}

void MainWindow::on_actionDownloadData_triggered()
{
    auto dialog = new DownloadDialog(this);
    dialog->show();
}

void MainWindow::on_actionChangeLog_triggered()
{
    Util::showMarkdown(tr(":/docs/changelog.md"), this);
}

void MainWindow::on_actionFeedback_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/tootal/DBLParse/issues"));
}

void MainWindow::on_actionDocumentation_triggered()
{
    Util::showMarkdown(tr(":/docs/README.md"), this);
}

void MainWindow::on_actionAbout_DBLParse_old_triggered()
{
    showAboutBox(QPixmap(":/resources/DBLParse.png"), tr(
        "<b>DBLParse</b><br>Version: %1<br><br>Built on %2<br><br>"
        "DBLParse is an application that bases on dblp computer "
        "science bibliography.<br/><br/>Please visit "
        "<a href=\"https://github.com/tootal/DBLParse\">DBLParse</a> "
        "for more information."
    ).arg(VERSION_STR, __TIMESTAMP__));
}

