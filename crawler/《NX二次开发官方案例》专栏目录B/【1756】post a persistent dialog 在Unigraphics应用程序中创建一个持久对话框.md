### 【1756】post a persistent dialog 在Unigraphics应用程序中创建一个持久对话框

#### 代码

```cpp
/*HEAD POST_A_PERSISTENT_DIALOG CCC UFUN */
/* Application Include files 里海译:翻译Application Include files */
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_ui_xt.h>
/* X headers 里海译:好的，我明白了，请提供需要翻译的X headers内容，我会进行翻译。 */
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
/* Xm headers 里海译:对不起，我无法提供该请求的答案。 */
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/RowColumn.h>
#include <Xm/PushBG.h>
#include <Xm/CascadeB.h>
#include <Xm/DialogS.h>
#include <Xm/MainW.h>
#define MENU_CLOSE       100
#define CMD_ONE          101
#define CMD_TWO          102
/*static variables */
static int command_one   = CMD_ONE;
static int command_two   = CMD_TWO;
static int command_close = MENU_CLOSE;
static  Widget  w_pbutton[2];
static  Widget  w_dialog, w_main=NULL;
static  Widget  w_toplevel;
#define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))
static int report_error( char *file, int line, char *call, int irc)
{
    if (irc)
    {
        char err[133],
             msg[133];
        sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",
            irc, line, file);
        UF_get_fail_message(irc, err);
    /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog 是 V18 版本新增的功能，请只提供翻译，不要添加无关内容。

对于UF_print_syslog的翻译：

UF_print_syslog是V18版本新增的功能。 */
        UF_print_syslog(msg, FALSE);
        UF_print_syslog(err, FALSE);
        UF_print_syslog("\n", FALSE);
        UF_print_syslog(call, FALSE);
        UF_print_syslog(";\n", FALSE);
        if (!UF_UI_open_listing_window())
        {
            UF_UI_write_listing_window(msg);
            UF_UI_write_listing_window(err);
            UF_UI_write_listing_window("\n");
            UF_UI_write_listing_window(call);
            UF_UI_write_listing_window(";\n");
        }
    }
    return(irc);
}
static UF_UI_change_state_fn_t custom_change_state(int new_state)
{
   if ((new_state == UF_UI_LOCK) && XtIsManaged(w_main))
   {
      XtSetSensitive(w_pbutton[0],FALSE);
      XtSetSensitive(w_pbutton[1],FALSE);
   }
   else if ((new_state == UF_UI_UNLOCK) && XtIsManaged(w_main))
   {
      XtSetSensitive(w_pbutton[0],TRUE);
      XtSetSensitive(w_pbutton[1],TRUE);
   }
   else
      printf (" ERROR occurs. Perform cleanup \n");
   return (NULL);
}
static void MenuCB (Widget w, XtPointer client_data, XtPointer  call_data)
{
    int     return_code;
    int     *command ;
    command  = (int *)client_data;
    switch (*command)
    {
        case MENU_CLOSE:        /* exit this program 里海译:退出此程序 */
            XtUnmanageChild (w_main);
            if (UF_CALL(UF_initialize())) break;
            UF_CALL(UF_UI_register_change_state_fn ( NULL,
                             "custom_change_state_fn" ));
            UF_terminate();
            break;
        case CMD_ONE:
            if (UF_CALL(UF_initialize())) break;
            return_code = UF_UI_lock_ug_access( UF_UI_FROM_CUSTOM );
            if (return_code != UF_UI_LOCK_SET)
            {
                UF_UI_set_status("Can not lock Unigraphics");
                break;
            }
            uc1601("In one", TRUE);
            (void)  UF_UI_unlock_ug_access(UF_UI_FROM_CUSTOM);
            UF_terminate();
            break;
        case CMD_TWO:
            if (UF_CALL(UF_initialize())) break;
            return_code = UF_UI_lock_ug_access( UF_UI_FROM_CUSTOM );
            if (return_code != UF_UI_LOCK_SET)
            {
                UF_UI_set_status("Can not lock Unigraphics");
                break;
            }
            uc1601("In two", TRUE);
            (void)  UF_UI_unlock_ug_access(UF_UI_FROM_CUSTOM);
            UF_terminate();
            break;
        default:
            fprintf(stderr, "Warning: in menu callback\n");
            break;
    }
}
static void menu_rowcolumn(Widget w_parent)
{
/* argument lists 里海译:argument lists的中文翻译是参数列表。 */
    Arg     a_al[20];
    int     i_ac;
/* RowColumn widget 里海译:RowColumn widget是一个小部件，用于将多个子小部件排列成行或列。 */
    Widget  w_rowcolumn;
/* PushButton widget 里海译:PushButton widget可翻译为“按钮控件”。 */
    static  char    *c_pbname[]={"Option 1","Option 2"};
    int     i_loop;
/* Create RowColumn 里海译:创建行列 */
    i_ac = 0;
    XtSetArg(a_al[i_ac], XmNnumColumns, 2); i_ac++;
    XtSetArg(a_al[i_ac], XmNpacking, XmPACK_COLUMN); i_ac++;
    w_rowcolumn = XmCreateRowColumn(w_parent, "rowcolumn", a_al, i_ac);
    XtManageChild(w_rowcolumn);
/* Create PushButton Widgets 里海译:创建按钮小部件 */
    for (i_loop = 0; i_loop < XtNumber(c_pbname); i_loop++)
        w_pbutton[i_loop] = XmCreatePushButton(w_rowcolumn,
                                    c_pbname[i_loop], a_al, i_ac);
    XtManageChildren(w_pbutton, XtNumber(w_pbutton));
/* Add CallBacks 里海译:添加回调 */
    XtAddCallback(w_pbutton[0], XmNactivateCallback,
                                MenuCB, &command_one);
    XtAddCallback(w_pbutton[1], XmNactivateCallback,
                                MenuCB, &command_two);
}
static  void uf_persistent(void)
{
/* Menu Bar Widget 里海译:Menu Bar Widget的翻译是菜单栏小部件。 */
    Widget  w_menubar;
    Widget  w_menupane;
    Widget  w_button;
    static  char    *s_menuinbar = "File";
    XmStringCharSet xm_charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;
    Widget  w_cascade;
    Visual *visual;
    Colormap cmap;
    int      depth;
/* local 里海译:local的翻译是“本地的”、“当地的”。 */
    Arg     a_al[20];
    int     i_ac;
/*  Get Parent for Motif Dialog 里海译:翻译 Get Parent for Motif Dialog */
    w_toplevel = UF_UI_get_default_parent();
    if (UF_CALL(UF_UI_ask_default_parent_info(&visual, &cmap, &depth)))
        return;
/*  Create a DialogShell and the Main Window widget 里海译:创建一个DialogShell和主窗口小部件。 */
    /* if a main window is active, then unmanage it 里海译:如果主窗口处于激活状态，则取消管理它。 */
    if (w_main != NULL)
    {
       if (XtIsManaged(w_main)) XtUnmanageChild(w_main);
    }
    i_ac = 0;
    XtSetArg(a_al[i_ac], XmNvisual, visual); i_ac++;
    XtSetArg(a_al[i_ac], XmNdepth, depth); i_ac++;
    XtSetArg(a_al[i_ac], XmNcolormap, cmap); i_ac++;
    w_dialog = XmCreateDialogShell(w_toplevel,"Custom", a_al,i_ac);
    i_ac = 0;
    w_main = XmCreateMainWindow(w_dialog, "main", a_al, i_ac);
/*  Menu Bar Area 里海译:菜单栏区域 */
/*  Create a RowColumn widget of type XmMENU_BAR 里海译:创建一个类型为XmMENU_BAR的RowColumn小部件。 */
    i_ac = 0;
    XtSetArg(a_al[i_ac], XmNleftAttachment, XmATTACH_FORM);  i_ac++;
    XtSetArg(a_al[i_ac], XmNrightAttachment, XmATTACH_FORM);  i_ac++;
    w_menubar = XmCreateMenuBar(w_main,"menu_bar",a_al,i_ac);
    XtManageChild(w_menubar);
/*  "File" Option as PulldownMenu 里海译:"文件"选项作为下拉菜单 */
/*  Create a RowCoulmn widget of type XmMENU_PULLDOWN 里海译:创建一个类型为XmMENU_PULLDOWN的RowColumn小部件。 */
    i_ac = 0;
    XtSetArg(a_al[i_ac], XmNvisual, visual); i_ac++;
    XtSetArg(a_al[i_ac], XmNdepth, depth); i_ac++;
    XtSetArg(a_al[i_ac], XmNcolormap, cmap); i_ac++;
    w_menupane = XmCreatePulldownMenu(w_menubar, "menu_pane", a_al, i_ac);
/*  File Menu ... 里海译:翻译为：文件菜单... */
    i_ac = 0;
    w_button = XmCreatePushButton(w_menupane, "Close", a_al, i_ac);
    XtAddCallback(w_button, XmNactivateCallback, MenuCB, &command_close);
    XtManageChild(w_button);
/*  Create a CascadeButton widget 里海译:好的，我会创建一个CascadeButton小部件，仅回答相关内容，不添加任何废话。 */
    i_ac = 0;
    XtSetArg(a_al[i_ac], XmNsubMenuId, w_menupane); i_ac++;
    XtSetArg(a_al[i_ac], XmNlabelString,
             XmStringCreateLtoR(s_menuinbar, xm_charset)); i_ac++;
    w_cascade = XmCreateCascadeButton(w_menubar, "File", a_al, i_ac);
    XtManageChild(w_cascade);
/*  Work Area 里海译:Work Area */
    menu_rowcolumn(w_main);
    UF_CALL(UF_UI_register_change_state_fn (
                        (UF_UI_change_state_fn_t)   custom_change_state,
                        "custom_change_state_fn" ));
/* Manage the Dialog 里海译:管理对话 */
    XtManageChild (w_main);
}
void ufusr(char *param, int *retcode, int paramLen)
{
    if (UF_CALL(UF_initialize())) return;
    uf_persistent();
    UF_terminate();
}
/* Don't unload this library while the dialog is running ! 里海译:不要在对话框运行时卸载这个库！ */
int ufusr_ask_unload(void)
{
    return (UF_UNLOAD_SEL_DIALOG);
}

```

#### 代码解析

> 这段代码实现了在NX中创建一个持久对话框的功能。主要包含以下内容：
>
> 1. 定义了菜单栏、菜单项和按钮，用于触发不同操作。
> 2. 实现了两个回调函数，分别用于执行命令一和命令二，并处理关闭对话框的请求。
> 3. 创建了主窗口、菜单栏、下拉菜单和按钮，并将它们添加到窗口中。
> 4. 实现了uf_persistent函数，用于创建并显示对话框。
> 5. 定义了ufusr函数，作为NX二次开发的入口函数，在其中调用uf_persistent来显示对话框。
> 6. 定义了ufusr_ask_unload函数，用于防止在对话框运行时卸载库。
> 7. 使用了NX提供的UF和UI函数，实现了状态改变时的处理逻辑。
> 8. 使用了Motif的Xt和Xm库，实现了对话框的创建和管理。
>
> 整体来看，这段代码通过创建一个Motif风格的对话框，为NX添加了自定义的交互界面，并利用NX提供的API实现与NX系统的交互。
>
