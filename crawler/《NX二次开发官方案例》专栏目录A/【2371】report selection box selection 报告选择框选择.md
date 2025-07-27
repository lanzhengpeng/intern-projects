### 【2371】report selection box selection 报告选择框选择

#### 代码

```cpp
    /*HEAD REPORT_SELECTION_BOX_SELECTION CCC UFUN */  
    /*  
        To create a dialog for use with this program:  
        Application->UIStyler  
        change CHANGE to GTAC  
        enter ok callback:  GTAC_ok_cb  
        enter constructor callback:  GTAC_const_cb  
        add a selection box object  
        File->Save As->selection_box  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    #define GTAC_SEL_BOX_0                 ("SEL_BOX_0")  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，翻译如下：

注意：UF_print_syslog是V18版本新增的函数，请只提供翻译，不要添加任何废话。 */  
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
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    struct sel_box_io_s  
    {  
        int  
            deflt,  
            n_items;  
        char  
            *cue,  
            *title,  
            **items,  
            selection[133];  
    };  
    typedef struct sel_box_io_s sel_box_io_t, *sel_box_io_p_t;  
    /* qq3123197280 */  
    static int GTAC_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        sel_box_io_p_t  
            io_data = (sel_box_io_p_t) client_data;  
        UF_STYLER_item_value_type_t  
            set_value;  
    /*  Set the title on the dialog 里海译:将标题设置在对话框上。 */  
        set_value.item_attr = UF_STYLER_LABEL;  
        set_value.item_id = UF_STYLER_DIALOG_INDEX;  
        set_value.value.string = io_data->title;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
    /*  Set the cue for the dialog 里海译:翻译：设置对话提示。 */  
        UF_CALL(UF_UI_set_prompt(io_data->cue));  
    /*  Set the items in the dialog 里海译:将对话框中的项目设置为 */  
        set_value.item_attr = UF_STYLER_SUBITEM_VALUES;  
        set_value.item_id = GTAC_SEL_BOX_0;  
        set_value.value.strings = io_data->items;  
        set_value.count = io_data->n_items;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
    /*  Set the default item in the dialog 里海译:将对话框中的默认项目设置为 */  
        set_value.item_attr = UF_STYLER_VALUE;  
        set_value.item_id = GTAC_SEL_BOX_0;  
        set_value.subitem_index = io_data->deflt - 1;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_ok_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        sel_box_io_p_t  
            io_data = (sel_box_io_p_t) client_data;  
        UF_STYLER_item_value_type_t  
            ask_value;  
        ask_value.item_attr = UF_STYLER_VALUE;  
        ask_value.item_id = GTAC_SEL_BOX_0;  
        ask_value.indicator = UF_STYLER_STRING_VALUE;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &ask_value));  
        strcpy(io_data->selection, ask_value.value.string);  
        UF_CALL(UF_STYLER_free_value(&ask_value));  
        return (UF_UI_CB_EXIT_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        char  
            *options_a[4] = { "One", "Two", "Three", "Four" },  
            *options_b[2] = { "First", "Second" };  
        sel_box_io_t  
            first_set,  
            second_set;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[3] = {  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB  , 0, GTAC_const_cb},  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_OK_CB           , 0, GTAC_ok_cb},  
            {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        first_set.cue = "Pick first selection";  
        first_set.title = "First set";  
        first_set.deflt = 3;  
        first_set.n_items = 4;  
        first_set.items = options_a;  
        UF_CALL(UF_STYLER_create_dialog("selection_box.dlg", GTAC_cbs, 3,  
            (void *)&first_set, &resp));  
        if (resp == UF_UI_OK) WRITE_S(first_set.selection);  
         second_set.cue = "Pick second selection";  
        second_set.title = "Second set";  
        second_set.deflt = 2;  
        second_set.n_items = 2;  
        second_set.items = options_b;  
        UF_CALL(UF_STYLER_create_dialog("selection_box.dlg", GTAC_cbs, 3,  
            (void *)&second_set, &resp));  
        if (resp == UF_UI_OK) WRITE_S(second_set.selection);  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个结构体sel_box_io_t，用于存储选择框的参数，包括标题、提示、项目数组、默认选择等。
> 2. 定义了GTAC_const_cb回调函数，用于在对话框构造时设置标题、提示、项目等参数。
> 3. 定义了GTAC_ok_cb回调函数，用于在用户点击确定时获取选择的项目。
> 4. do_it函数中创建了两个选择框，分别设置了标题、提示、项目和默认选择，然后显示对话框并获取用户的选择。
> 5. ufusr函数是NX二次开发的入口函数，其中调用了do_it函数来执行选择框的创建和交互。
> 6. ufusr_ask_unload函数用于在插件卸载时立即卸载。
> 7. 代码中使用了UF_STYLER相关的接口来创建和操作对话框，实现了选择框的功能。
> 8. 使用了宏WRITE_S来将选择结果输出到列表窗口。
> 9. 使用了宏UF_CALL来捕获函数调用错误并打印错误信息。
> 10. 使用了结构体数组GTAC_cbs来存储回调函数信息，以在对话框创建时绑定回调。
>
> 总体来说，这段代码实现了基于NX的UIStyler选择框的功能，包括创建、交互和错误处理。
>
