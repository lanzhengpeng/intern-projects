### 【2655】select drawings using uistyler 选择图纸使用UIStyler

#### 代码

```cpp
    /*HEAD SELECT_DRAWINGS_USING_UISTYLER CCC UFUN */  
    /*  To create a dialog to use with this program:  
        - Select Application->User Interface Styler  
        - Change the dialog's Prefix Name to "DWGS"  
        - Specify the Constructor Callback as DWGS_"const_cb"  
        - Add a Multi List object  
        - Save Dialog As "application/drawings"  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #include <uf_draw.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #define DWGS_MULTI_LIST_0              ("MULTI_LIST_0")  
    #define DWGS_CB_COUNT ( 2 + 1 ) /* Add 1 for the terminator 里海译:加上1作为终结符 */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，V18版本新增了UF_print_syslog函数。 */  
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
    /* qq3123197280 */  
    static int DWGS_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *drawings;  
        char  
            dname[MAX_ENTITY_NAME_SIZE+1];  
        UF_STYLER_item_value_type_t  
            set_value;  
        set_value.item_attr = UF_STYLER_LIST_INSERT;  
        set_value.item_id = DWGS_MULTI_LIST_0;  
        set_value.subitem_index = UF_STYLER_NO_SUB_INDEX;  
        set_value.indicator = UF_STYLER_STRING_VALUE;  
        set_value.value.string = dname;  
        UF_CALL(UF_DRAW_ask_drawings(&n, &drawings));  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_name(drawings[ii], dname));  
            UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        }  
        if (n > 0) UF_free(drawings);  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int DWGS_apply_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            ii;  
        UF_STYLER_item_value_type_t  
            ask_value;  
        ask_value.item_attr = UF_STYLER_VALUE;  
        ask_value.item_id = DWGS_MULTI_LIST_0;  
        ask_value.indicator = UF_STYLER_STRING_PTR_VALUE;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &ask_value));  
        for (ii = 0; ii < ask_value.count; ii++)  
            printf("Select drawing %s\n", ask_value.value.strings[ii]);  
        if (ask_value.count > 0) UF_STYLER_free_value(&ask_value);  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        UF_STYLER_callback_info_t DWGS_cbs[DWGS_CB_COUNT] = {  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, DWGS_const_cb},  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_APPLY_CB, 0, DWGS_apply_cb},  
            {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        if (UF_PART_ask_num_parts() > 0)  
            UF_CALL(UF_STYLER_create_dialog("drawings.dlg", DWGS_cbs,  
                DWGS_CB_COUNT, NULL, &resp));  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码主要用于创建一个对话框，以列出当前部件中的所有绘图，并允许用户选择一个或多个绘图。下面是对代码的详细介绍：
>
> 1. 引入头文件：包含了必要的NX Open API头文件。
> 2. 宏定义：定义了对话框中组件的名称，如多列表组件。
> 3. 错误报告函数：用于打印错误信息。
> 4. 构造回调函数：用于在对话框打开时，获取当前部件中的所有绘图，并添加到多列表组件中。
> 5. 应用回调函数：用于在对话框关闭时，获取用户选择的一个或多个绘图，并打印出来。
> 6. 主函数：用于创建并显示对话框，只有当前部件存在时才会显示。
> 7. NX Open初始化和终止函数：用于在进入和退出NX时进行初始化和清理。
> 8. 卸载函数：用于控制NX在卸载该程序时的行为。
>
> 通过以上代码，用户可以方便地在NX中选择一个或多个绘图，实现了选择绘图的功能。
>
