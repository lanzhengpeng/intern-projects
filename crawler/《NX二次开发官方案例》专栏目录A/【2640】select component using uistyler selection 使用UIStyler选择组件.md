### 【2640】select component using uistyler selection 使用UIStyler选择组件

#### 代码

```cpp
    /*HEAD SELECT_COMPONENT_USING_UISTYLER_SELECTION CCC UFUN */  
    /*  
        To create a dialog to use with this code:  
            - Application-> User Interface Styler  
            - change prefix name to GTAC  
            - Add the "sel_cb" as the constructor callback  
            - Enable Selection - Single w/ Select and Deselect  
            - Save the dialog as UGII_USER_DIR/application/select_component.dlg  
    */  
    #include <string.h>  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中的新功能，可以翻译为：UF_print_syslog 是在 V18 中新增的功能。 */  
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
    static int get_comp(int n_sel, tag_p_t sel_objs, int n_dsel, tag_p_t dsel,  
        void *user_data, UF_UI_selection_p_t select)  
    {  
        tag_p_t  
            sel_comp;  
        sel_comp = user_data;  
        *sel_comp = sel_objs[0];  
        return UF_UI_CB_EXIT_DIALOG;  
    }  
    /* qq3123197280 */  
    static int GTAC_sel_cb ( int dialog_id, void * client_data,  
                       UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            sel_data;  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        sel_data.item_id = UF_STYLER_DIALOG_INDEX;  
        sel_data.item_attr  = UF_STYLER_SELECTION;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &sel_data));  
        UF_CALL(UF_UI_set_sel_mask(sel_data.value.selection,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask));  
        UF_CALL(UF_UI_set_sel_procs(sel_data.value.selection, NULL, get_comp,  
            client_data));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_component_name(tag_t component_tag)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[UF_CFI_MAX_PATH_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        WRITE_D(component_tag);  
        if (!UF_CALL(UF_ASSEM_ask_component_data(component_tag, part_name,  
            refset_name, instance_name, origin, csys_matrix, transform)))  
            WRITE_S(instance_name);  
    }  
    static void do_it(void)  
    {  
        int  
            response;  
        tag_t  
            component = NULL_TAG;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[2] = {  
                {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, GTAC_sel_cb},  
                {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        UF_CALL( UF_STYLER_create_dialog("select_component", GTAC_cbs, 2,  
            &component, &response));  
        if (component != NULL_TAG) report_component_name(component);  
    }  
    /* qq3123197280 */  
    extern void ufusr(char *param, int *retcode, int paramLen)  
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

> 这段代码是一个NX Open C++程序，用于在NX中创建一个对话框，用户可以通过该对话框选择一个组件。
>
> 主要功能包括：
>
> 1. 错误处理：通过宏UF_CALL封装UF函数调用，并在出错时打印错误信息。
> 2. 回调函数：定义了get_comp回调函数，用于获取用户选择的组件，并将其存储在用户数据中。
> 3. 创建对话框：GTAC_sel_cb函数用于初始化对话框，设置选择组件的过滤条件。
> 4. 报告组件信息：定义了report_component_name函数，用于报告选择的组件的名称。
> 5. 主函数：do_it函数用于创建并显示对话框，获取用户选择的组件，并报告组件信息。
> 6. ufusr函数：作为程序的入口点，初始化并调用do_it函数。
> 7. 卸载处理：ufusr_ask_unload函数用于设置程序卸载方式。
>
> 总的来说，这段代码实现了通过对话框选择组件并获取组件信息的功能，使用了NX Open提供的UI Styler对话框和UF选择功能。
>
