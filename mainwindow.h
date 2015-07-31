#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTimer>
#include <Windows.h>
#include <QTreeWidgetItem>

#define ADDR_SYS_SAVE 0x00939E40
#define ADDR_SYS_TIME 0x01DBFB94
#define ADDR_SYS_JAMESHEALTH 0x01FB4D3C
#define ADDR_SYS_NEARBYMONSTER 0x01FBBD04
#define ADDR_SYS_MELEEKILLS 0x01DBFB92
#define ADDR_SYS_GUNKILLS 0x01DBFB90
#define ADDR_ITM_HEALTHDRINKS 0x01F7E3EE
#define ADDR_ITM_FIRSTAID 0x01F7E3F0
#define ADDR_ITM_HANDGUNBULLETS 0x01FBBD10
#define ADDR_ITM_SHOTGUNSHELLS 0x01F7E3FA
#define ADDR_WPN_HANDGUNBULLETS 0x01F7E3F4
#define ADDR_WPN_SHOTGUNBULLETS 0x01F7E3F8
#define ADDR_EVT_LAURAKICKSKEY 0x00184292
#define ADDR_EVT_MEETEDDY 0x01DBFC7F
#define ADDR_EVT_APARTMENTS 0x01FB63A4
#define ADDR_EVT_HANGER01 0x01FB5874
#define ADDR_EVT_HANGER02 0x01FB693C
#define ADDR_EVT_HANGER03 0x01FB5E0C


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *tableModel;

    QTreeWidgetItem *qt_sys;
    QTreeWidgetItem *qt_sys_saves;
    QTreeWidgetItem *qt_sys_jameshealth;
    QTreeWidgetItem *qt_sys_nearbymonster;
    QTreeWidgetItem *qt_sys_meleekills;
    QTreeWidgetItem *qt_sys_gunkills;
    QTreeWidgetItem *qt_itm;
    QTreeWidgetItem *qt_itm_healthdrinks;
    QTreeWidgetItem *qt_itm_firstaid;
    QTreeWidgetItem *qt_itm_handgun_bullets;
    QTreeWidgetItem *qt_itm_shotgun_shells;
    QTreeWidgetItem *qt_wpn;
    QTreeWidgetItem *qt_wpn_handgun;
    QTreeWidgetItem *qt_wpn_shotgun;
    QTreeWidgetItem *qt_evt;
    QTreeWidgetItem *qt_evt_laurakickskey;
    QTreeWidgetItem *qt_evt_meeteddy;
    QTreeWidgetItem *qt_evt_apartments;
    QTreeWidgetItem *qt_evt_hangers;

    int sys_saves;
    UINT8 sys_meleekills;
    UINT8 sys_gunkills;
    UINT8 sys_nearbymonster;
    float sys_time;
    float sys_jameshealth;
    UINT8 itm_healthdrinks;
    UINT8 itm_firstaid;
    UINT8 itm_handgunbullets;
    UINT8 itm_shotgunshells;
    UINT8 wpn_handgun;
    UINT8 wpn_shotgun;
    UINT8 evt_laurakickskey;
    UINT8 evt_meeteddy;
    float evt_apartments;
    float evt_hanger01;
    float evt_hanger02;
    float evt_hanger03;
    bool evt_apartments_canTrigger;
    bool evt_hanger_canTrigger,evt_hanger_done;

    int hours;
    int minutes;
    float seconds;

    UINT8 tmp_meleekills,tmp_gunkills;

    UINT8 totalKills;

    HWND sh2Window;
    DWORD sh2ProcessID;
    HANDLE sh2Process;


    void handleEvents();
    void handleSystem();
    void handleItems();
    void handleWeapons();
private slots:
    void updateData();
    void on_action_Exit_triggered();
    void on_action_About_triggered();
    void on_treeWidget_itemExpanded(QTreeWidgetItem *item);
    void on_treeWidget_itemCollapsed(QTreeWidgetItem *item);
    void on_action_New_Run_triggered();
};

#endif // MAINWINDOW_H
