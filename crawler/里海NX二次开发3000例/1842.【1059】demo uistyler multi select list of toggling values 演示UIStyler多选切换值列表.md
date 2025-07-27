### 【1059】demo uistyler multi select list of toggling values 演示UIStyler多选切换值列表

#### 代码

```cpp
    /*HEAD DEMO_UISTYLER_MULTI_SELECT_LIST_OF_TOGGLING_VALUES CCC UFUN */  
    /*  To create a dialog to use with this program:  
        - Select Application->User Interface Styler  
        - Change the dialog's Prefix Name to "GTAC"  
        - Specify the Constructor Callback as "const_cb"  
        - Add a Multi List object  
        - Set the Activate callback of the Multi List to "update_cb"  
        - Save Dialog As "$UGII_USER_DIR/application/toggle_list"  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据您的要求，我翻译了原文中的内容：

注意：UF_print_syslog 是 V18 中新增的。

仅提供翻译，无需其他评论。 */  
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
    #define GTAC_MULTI_LIST_0              ("MULTI_LIST_0")  
    #define N_ITEMS 10  
    /* qq3123197280 */  
    static int GTAC_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            ii;  
        UF_STYLER_item_value_type_t  
            value;  
        value.item_attr = UF_STYLER_LIST_INSERT;  
        value.item_id = GTAC_MULTI_LIST_0;  
        value.subitem_index = UF_STYLER_NO_SUB_INDEX;  
        value.indicator = UF_STYLER_STRING_VALUE;  
        value.value.string = "N";  
    /*  load the list with "N"s 里海译:翻译 load the list with "N"s：用"N"填充列表 */  
        for (ii = 0; ii < N_ITEMS; ii++)  
            UF_CALL(UF_STYLER_set_value(dialog_id, &value));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_update_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            ii, jj;  
        UF_STYLER_item_value_type_t  
            value[2];  
    /*  Ask the current item values and the indices of the selected items 里海译:询问当前项的值和选中项的索引 */  
        value[0].item_attr = UF_STYLER_VALUE;  
        value[0].item_id = GTAC_MULTI_LIST_0;  
        value[0].indicator = UF_STYLER_INTEGER_PTR_VALUE;  
        value[1].item_attr = UF_STYLER_SUBITEM_VALUES;  
        value[1].item_id = GTAC_MULTI_LIST_0;  
        UF_CALL(UF_STYLER_ask_values(dialog_id, 2, value, &ii));  
        for (ii = 0, jj = 0; ii < value[1].count; ii++)  
            if (value[0].value.integers[jj] == ii)  
            {  
                if (!strcmp(value[1].value.strings[ii], "N"))  
                    strcpy(value[1].value.strings[ii], "Y");  
                else  
                    strcpy(value[1].value.strings[ii], "N");  
                jj++;  
            }  
        UF_CALL(UF_STYLER_set_value(dialog_id, &value[1]));  
        for (ii = 0; ii < 2; ii++)  
            UF_CALL(UF_STYLER_free_value(&value[ii]));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[3] = {  
     {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB  , 0, GTAC_const_cb},  
     {GTAC_MULTI_LIST_0     , UF_STYLER_ACTIVATE_CB     , 0, GTAC_update_cb},  
     {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        UF_CALL(UF_STYLER_create_dialog("toggle_list", GTAC_cbs, 3, NULL, &resp));  
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

> 这段NX Open代码实现了以下功能：
>
> 1. 创建了一个多选列表对话框，用于显示10个可切换的"N"和"Y"选项。
> 2. 在对话框构造时，初始化所有选项为"N"。
> 3. 当用户点击某个选项时，该选项的值会在"N"和"Y"之间切换。
> 4. 使用UF_STYLER组件来实现对话框和多选列表的创建。
> 5. 使用回调函数GTAC_const_cb在对话框构造时初始化列表项。
> 6. 使用回调函数GTAC_update_cb在列表项激活时切换值。
> 7. 主函数do_it()负责创建和显示对话框。
> 8. ufusr()是NX Open程序的入口函数，在这里调用do_it()来显示对话框。
> 9. ufusr_ask_unload()用于卸载动态链接库。
> 10. 使用宏UF_CALL来封装UF函数调用，并打印错误信息。
> 11. 使用UF_print_syslog和UF_UI函数在控制台和对话框中输出信息。
> 12. 使用UF_STYLER的回调机制来响应列表项的激活事件。
> 13. 整体上，这段代码实现了一个简单的多选列表，用户可以点击切换每个列表项的值。
>
