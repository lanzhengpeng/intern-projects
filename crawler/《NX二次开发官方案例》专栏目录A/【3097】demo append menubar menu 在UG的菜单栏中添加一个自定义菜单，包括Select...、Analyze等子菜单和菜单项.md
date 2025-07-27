### 【3097】demo append menubar menu 在UG的菜单栏中添加一个自定义菜单，包括Select...、Analyze等子菜单和菜单项

#### 代码

```cpp
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_exit.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select Objects", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
    }  
    static void Select ( void )  
    {  
        int  
            n;  
        tag_t  
            *objects;  
        if ( UF_UI_lock_ug_access( UF_UI_FROM_CUSTOM ) != UF_UI_LOCK_SET)  
        {  
             UF_UI_set_status("Can not lock Unigraphics");  
             return;  
        }  
        if ((n = select_objects("In Select CB", &objects)) > 0)  
            UF_free(objects);  
        UF_UI_unlock_ug_access( UF_UI_FROM_CUSTOM );  
    }  
    static void MassProp ( void )  
    {  
        ECHO(" Mass Property is activated \n");  
    }  
    static void Curvature ( void )  
    {  
        ECHO(" Curvature is activated \n");  
    }  
    static void Curing ( void )  
    {  
        ECHO(" Curing is activated \n");  
    }  
    static void Query ( void )  
    {  
        ECHO(" Query is activated \n");  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        UF_UI_menubar_item_t menu[] =  {  
            {UF_UI_MENU,  " CAD2000 "  , "BM_USER",FALSE, NULL , NULL },  
            {UF_UI_PUSH , "Select..." ,"USER_PULLDOWN1",TRUE, Select,NULL},  
            {UF_UI_CASCADE_MENU, "Analyze", "USER_PULLDOWN2", FALSE, NULL , NULL },  
            {UF_UI_TOGGLE , " Analysis ", NULL, FALSE, NULL, NULL },  
            {UF_UI_BUTTON_SEPARATOR , NULL, NULL, FALSE, NULL, NULL},  
            {UF_UI_PUSH , "Mass Properties...", NULL, FALSE, MassProp, NULL },  
            {UF_UI_PUSH , "Curvature..." ,  NULL, FALSE, Curvature, NULL },  
            {UF_UI_PUSH , "Curing..." ,  NULL,FALSE, Curing ,NULL },  
            UF_UI_END_CASCADE_MENU,  
            {UF_UI_PUSH , "QUERY..." ,  NULL, FALSE, Query, NULL },  
            UF_UI_END_MENU  
        };  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_UI_append_menubar_menu(menu, NULL, NULL));  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是NX Open的二次开发示例代码，主要实现了以下功能：
>
> 1. 定义了ECHO和UF_CALL宏，用于在控制台和日志窗口中打印信息，以及调用NX API函数并检查返回码。
> 2. 实现了一个select_objects函数，用于弹出选择对话框，让用户选择对象，并返回选中的对象数组。
> 3. 实现了Select、MassProp、Curvature、Curing和Query函数，分别用于选择对象、计算质量属性、分析曲率、分析固化过程和查询信息，这些函数会在对应的菜单项被点击时被调用。
> 4. 定义了一个菜单数组，其中包含了一个名为"CAD2000"的主菜单，以及Select、Analyze、Mass Properties、Curvature、Curing和Query子菜单项。
> 5. 在ufsta函数中，首先调用UF_initialize进行初始化，然后调用UF_UI_append_menubar_menu将自定义的菜单添加到NX菜单栏，最后调用UF_terminate进行清理。
> 6. ufsta函数是二次开发程序的入口点。
>
> 总的来说，这段代码通过自定义菜单和回调函数的方式，实现了在NX中添加一组自定义的菜单项，用于选择对象和执行分析操作。
>
