#include "aboutwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sh2Process = 0;
    sh2ProcessID = 0;
    sh2Window = 0;

    evt_apartments_canTrigger = false;
    evt_hanger_canTrigger = false;
    evt_hanger_done = false;

    //System
    qt_sys = new QTreeWidgetItem(ui->treeWidget);
    qt_sys->setText(0,"System");
    qt_sys_saves = new QTreeWidgetItem(qt_sys);
    qt_sys_saves->setText(0,"Saves");
    qt_sys_jameshealth = new QTreeWidgetItem(qt_sys);
    qt_sys_jameshealth->setText(0,"James Health");
    qt_sys_meleekills = new QTreeWidgetItem(qt_sys);
    qt_sys_meleekills->setText(0,"Melee Kills");
    qt_sys_gunkills = new QTreeWidgetItem(qt_sys);
    qt_sys_gunkills->setText(0,"Gun Kills");

    //Events
    qt_evt = new QTreeWidgetItem(ui->treeWidget);
    qt_evt->setText(0,"Events");
    qt_evt_laurakickskey = new QTreeWidgetItem(qt_evt);
    qt_evt_laurakickskey->setText(0,"Laura Kicks Key");
    qt_evt_meeteddy = new QTreeWidgetItem(qt_evt);
    qt_evt_meeteddy->setText(0,"Meet Eddy");
    qt_evt_apartments=new QTreeWidgetItem(qt_evt);
    qt_evt_apartments->setText(0,"Apartments");
    qt_evt_hangers=new QTreeWidgetItem(qt_evt);
    qt_evt_hangers->setText(0,"Hangers");

    //Items
    qt_itm = new QTreeWidgetItem(ui->treeWidget);
    qt_itm->setText(0,"Items");
    qt_itm_healthdrinks = new QTreeWidgetItem(qt_itm);
    qt_itm_healthdrinks->setText(0,"Health Drinks");
    qt_itm_firstaid = new QTreeWidgetItem(qt_itm);
    qt_itm_firstaid->setText(0,"First-Aid Kits");
    qt_itm_handgun_bullets = new QTreeWidgetItem(qt_itm);
    qt_itm_handgun_bullets->setText(0,"Hand gun bullets");
    qt_itm_shotgun_shells = new QTreeWidgetItem(qt_itm);
    qt_itm_shotgun_shells->setText(0,"Shotgun shells");

    //Weapons
    qt_wpn = new QTreeWidgetItem(ui->treeWidget);
    qt_wpn->setText(0,"Weapons");
    qt_wpn_handgun = new QTreeWidgetItem(qt_wpn);
    qt_wpn_handgun->setText(0,"Handgun");
    qt_wpn_shotgun = new QTreeWidgetItem(qt_wpn);
    qt_wpn_shotgun->setText(0,"Shotgun");

    ui->treeWidget->resizeColumnToContents(0);
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateData()));
    timer->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleEvents()
{
    if(ReadProcessMemory(sh2Process,(LPVOID)ADDR_EVT_LAURAKICKSKEY,&evt_laurakickskey,(DWORD)sizeof(evt_laurakickskey),NULL)==0) {
        // We failed to read the event laura kicked the key
        qt_evt_laurakickskey->setText(1,"error");
    } else {
        if(evt_laurakickskey==0) {
            qt_evt_laurakickskey->setText(1,"false");
        } else {
            if(qt_evt_laurakickskey->text(1).compare("false")==0) {
                qt_evt_laurakickskey->setText(2,ui->sys_time->text());
                qt_evt_laurakickskey->setToolTip(2,QString::number(sys_time,'g',6));
            }
            qt_evt_laurakickskey->setText(1,"true");
        }
    }
    if(ReadProcessMemory(sh2Process,(LPVOID)ADDR_EVT_MEETEDDY,&evt_meeteddy,(DWORD)sizeof(evt_meeteddy),NULL)==0) {
        // We failed to read the event laura kicked the key
        qt_evt_meeteddy->setText(1,"error");
    } else {
        if(evt_meeteddy==0) {
            qt_evt_meeteddy->setText(1,"false");
            qt_evt_meeteddy->setText(2,"");
        } else {
            if(qt_evt_meeteddy->text(2).length() == 0) {
                qt_evt_meeteddy->setText(2,ui->sys_time->text());
                qt_evt_meeteddy->setToolTip(2,QString::number(sys_time,'g',6));
            }
            qt_evt_meeteddy->setText(1,"true");
        }
    }

    if(ReadProcessMemory(sh2Process,(LPVOID)ADDR_EVT_APARTMENTS,&evt_apartments,(DWORD)sizeof(evt_apartments),NULL)!=0) {
        if(evt_apartments>1) {
            evt_apartments_canTrigger = true;
        }
        if(evt_apartments==0 && evt_apartments_canTrigger) {
            if(qt_evt_apartments->text(2).length() == 0) {
                qt_evt_apartments->setText(2,ui->sys_time->text());
                qt_evt_apartments->setToolTip(2,QString::number(sys_time,'g',6));
            }
            qt_evt_apartments->setText(1,"true");
        } else {
            qt_evt_apartments->setText(1,"false");
        }
    }

    if(ReadProcessMemory(sh2Process,(LPVOID)ADDR_EVT_HANGER01,&evt_hanger01,(DWORD)sizeof(evt_hanger01),NULL)!=0) {

    }
    if(ReadProcessMemory(sh2Process,(LPVOID)ADDR_EVT_HANGER02,&evt_hanger02,(DWORD)sizeof(evt_hanger02),NULL)!=0) {

    }
    if(ReadProcessMemory(sh2Process,(LPVOID)ADDR_EVT_HANGER03,&evt_hanger03,(DWORD)sizeof(evt_hanger03),NULL)!=0) {

    }

    if(!evt_hanger_done && evt_hanger01 == 0 && evt_hanger02 == 0 && evt_hanger03==0) {
        tmp_meleekills = sys_meleekills;
        tmp_gunkills = sys_gunkills;
    }

    if(!evt_hanger_done && evt_hanger01 == 0 && evt_hanger02 == 0 && ((tmp_gunkills + tmp_meleekills)+2 == totalKills)) {
        evt_hanger_canTrigger = true;
    }

    if(evt_hanger_canTrigger && !evt_hanger_done && evt_hanger03 == 0.0f) {
        qt_evt_hangers->setText(1,"true");
        qt_evt_hangers->setText(2,ui->sys_time->text());
        qt_evt_hangers->setToolTip(2,QString::number(sys_time,'g',6));
        evt_hanger_canTrigger = false;
        evt_hanger_done = true;
    }

}

void MainWindow::handleSystem()
{
    // If we reach this point, all the data should now be available to read
    if(ReadProcessMemory(sh2Process,(LPVOID)ADDR_SYS_SAVE,&sys_saves,(DWORD)sizeof(sys_saves),NULL)==0) {
        // We failed to read the saves
    } else {
        qt_sys_saves->setText(1,QString::number(sys_saves));
    }

    if(ReadProcessMemory(sh2Process,(LPVOID)ADDR_SYS_TIME,&sys_time,(DWORD)sizeof(sys_time),NULL)==0) {
        // We failed to read the time
    } else {
        minutes = sys_time / 60;
        seconds = sys_time - minutes*60;
        hours = minutes / 60;
        minutes-=hours*60;
        ui->sys_time->setText(QString("%1 : %2 : %3").arg(QString::number(hours),QString::number(minutes),QString::number(seconds,'g',4)));
    }

    if(ReadProcessMemory(sh2Process,(LPVOID)ADDR_SYS_MELEEKILLS,&sys_meleekills,(DWORD)sizeof(sys_meleekills),NULL)==0) {
        // We failed to read melee kills
    } else {
        qt_sys_meleekills->setText(1,QString::number(sys_meleekills));
    }
    if(ReadProcessMemory(sh2Process,(LPVOID)ADDR_SYS_GUNKILLS,&sys_gunkills,(DWORD)sizeof(sys_gunkills),NULL)==0) {
        // We failed to read gun kills
    } else {
        qt_sys_gunkills->setText(1,QString::number(sys_gunkills));
    }

    if(ReadProcessMemory(sh2Process,(LPVOID)ADDR_SYS_JAMESHEALTH,&sys_jameshealth,(DWORD)sizeof(sys_jameshealth),NULL)==0) {
        // We failed to read melee kills
    } else {
        qt_sys_jameshealth->setText(1,QString::number(sys_jameshealth,'g',5));
    }

    totalKills = (sys_gunkills + sys_meleekills);
}

void MainWindow::handleItems()
{
    if(ReadProcessMemory(sh2Process,(LPVOID)ADDR_ITM_HEALTHDRINKS,&itm_healthdrinks,(DWORD)sizeof(itm_healthdrinks),NULL)==0) {
        // We failed to read the qty of health drinks
    } else {
        qt_itm_healthdrinks->setText(1,QString::number(itm_healthdrinks));
    }
    if(ReadProcessMemory(sh2Process,(LPVOID)ADDR_ITM_FIRSTAID,&itm_firstaid,(DWORD)sizeof(itm_firstaid),NULL)==0) {
        // We failed to read the qty of first aid kits
    } else {
        qt_itm_firstaid->setText(1,QString::number(itm_firstaid));
    }

    if(ReadProcessMemory(sh2Process,(LPVOID)ADDR_ITM_HANDGUNBULLETS,&itm_handgunbullets,(DWORD)sizeof(itm_handgunbullets),NULL)==0) {
        // We failed to read the qty of handgun bullets available
    } else {
        qt_itm_handgun_bullets->setText(1,QString::number(itm_handgunbullets));
    }
    if(ReadProcessMemory(sh2Process,(LPVOID)ADDR_ITM_SHOTGUNSHELLS,&itm_shotgunshells,(DWORD)sizeof(itm_shotgunshells),NULL)==0) {
        // We failed to read the qty of handgun bullets available
    } else {
        qt_itm_shotgun_shells->setText(1,QString::number(itm_shotgunshells));
    }
}

void MainWindow::handleWeapons()
{
    if(ReadProcessMemory(sh2Process,(LPVOID)ADDR_WPN_HANDGUNBULLETS,&wpn_handgun,(DWORD)sizeof(wpn_handgun),NULL)==0) {
        // We failed to read the qty of remaining handgun bullets equipped
    } else {
        qt_wpn_handgun->setText(1,QString::number(wpn_handgun));
    }

    if(ReadProcessMemory(sh2Process,(LPVOID)ADDR_WPN_SHOTGUNBULLETS,&wpn_shotgun,(DWORD)sizeof(wpn_shotgun),NULL)==0) {
        // We failed to read the qty of remaining shotgun shells equipped
    } else {
        qt_wpn_shotgun->setText(1,QString::number(wpn_shotgun));
    }
}

void MainWindow::updateData()
{
    if(sh2Window==0) {
        QString wTitle = "SH2PC Title";

        sh2Window = FindWindow(0,(const wchar_t*)wTitle.utf16());
        if(sh2Window==0) {
            return;
        }
    }

    if(sh2ProcessID==0) {
        if(!GetWindowThreadProcessId(sh2Window,&sh2ProcessID)) {
            sh2Window = 0;
            sh2ProcessID = 0;
            return;
        }
    }

    if(sh2Process==0) {
        sh2Process = OpenProcess(PROCESS_ALL_ACCESS,FALSE,sh2ProcessID);
        if(sh2Process==0) {
            sh2Window = 0;
            sh2ProcessID = 0;
            sh2Process = 0;
            return;
        }
    }


    handleSystem();
    handleItems();
    handleWeapons();
    handleEvents();

    ui->treeWidget->resizeColumnToContents(0);
    ui->treeWidget->resizeColumnToContents(1);
    ui->treeWidget->resizeColumnToContents(2);
}

void MainWindow::on_action_Exit_triggered()
{
    close();
}

void MainWindow::on_action_About_triggered()
{
    AboutWindow *newWin = new AboutWindow(this);
    newWin->exec();
}

void MainWindow::on_treeWidget_itemExpanded(QTreeWidgetItem *item)
{
    ui->treeWidget->resizeColumnToContents(0);
    ui->treeWidget->resizeColumnToContents(1);
    ui->treeWidget->resizeColumnToContents(2);
}

void MainWindow::on_treeWidget_itemCollapsed(QTreeWidgetItem *item)
{
    ui->treeWidget->resizeColumnToContents(0);
    ui->treeWidget->resizeColumnToContents(1);
    ui->treeWidget->resizeColumnToContents(2);
}

void MainWindow::on_action_New_Run_triggered()
{
    tmp_meleekills = 0;
    tmp_gunkills = 0;
    evt_apartments_canTrigger = true;
    evt_hanger_done = false;
    evt_hanger_canTrigger = false;

    qt_evt_hangers->setText(1,"false");
    qt_evt_apartments->setText(1,"false");
    qt_evt_laurakickskey->setText(1,"false");
    qt_evt_meeteddy->setText(1,"false");
}
