### 【2579】select a limited number of objects 选择有限数量的对象

#### 代码

```cpp
    /*HEAD SELECT_A_LIMITED_NUMBER_OF_OBJECTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中的新功能。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static int enforce_limit(int num_selected, tag_p_t selected_objects, int num_deselected,  
        tag_p_t  deselected_objects, void *user_data, UF_UI_selection_p_t select_)  
    {  
        int  
            *nos = (int *)user_data;  
        nos[1] = nos[1] + num_selected - num_deselected;  
        if (nos[1] >= nos[0])  
            return UF_UI_CB_EXIT_DIALOG;  
        else  
            return UF_UI_CB_CONTINUE_DIALOG;  
    }  
    static int set_sel_limit( UF_UI_selection_p_t select, void *limit_data)  
    {  
        if (!UF_CALL(UF_UI_set_sel_procs(select, NULL, enforce_limit, limit_data)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_upto_n_objects(char *prompt, tag_t *objects, int n)  
    {  
        int  
            cnt,  
            ii,  
            resp,  
            limit_data[2];  
        tag_t  
            *temp_objs;  
        limit_data[0] = n;  
        limit_data[1] = 0;  
        UF_CALL(UF_UI_select_with_class_dialog("Select Objects", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, set_sel_limit, limit_data, &resp, &cnt, &temp_objs));  
        WRITE_D(cnt);  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(temp_objs[ii], 0));  
        for (ii = 0; (ii < n ) && (ii < cnt); ii++)  
            objects[ii] = temp_objs[ii];  
        if (cnt <= n) return cnt;  
        else return n;  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_objects(tag_t *objects, int n_objects)  
    {  
        logical  
            is_displayable;  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            UF_CALL(UF_OBJ_is_displayable(objects[ii], &is_displayable));  
            if (!is_displayable) continue;  
            sprintf(msg, "%d", ii+1);  
            if (UF_OBJ_ask_name_origin(objects[ii], loc))  
            {  
                UF_CALL(UF_OBJ_set_name(objects[ii], "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(objects[ii], loc));  
                UF_CALL(UF_OBJ_delete_name(objects[ii]));  
            }  
            display_temporary_text(msg, loc);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            cnt;  
        tag_t  
            objects[5];  
        cnt = select_upto_n_objects("Select upto 5 objects", objects, 5);  
        WRITE_D(cnt);  
        number_objects(objects, cnt);  
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

> 这段代码是一个NX Open C++ API开发的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数执行失败时打印错误信息。
> 2. 定义了一个整数写入列表窗口的函数write_integer_to_listing_window，用于将整数写入列表窗口。
> 3. 实现了一个强制限制选择数量的函数enforce_limit，该函数作为UF_UI_set_sel_procs的回调函数，用于限制用户选择对象的数量不超过预设的最大值。
> 4. 定义了一个设置选择限制的函数set_sel_limit，用于将强制限制选择数量的回调函数绑定到选择对话框。
> 5. 实现了一个选择最多N个对象的函数select_upto_n_objects，该函数弹出一个选择对话框，允许用户选择最多N个对象，并返回实际选择的对象数量。
> 6. 定义了一个显示临时文字的函数display_temporary_text，用于在对象上显示临时文字。
> 7. 实现了一个给对象编号的函数number_objects，用于给一组对象编号并显示。
> 8. 定义了一个主函数do_it，用于实现选择最多5个对象并给它们编号的功能。
> 9. 定义了ufusr函数，作为二次开发的入口函数，调用do_it函数实现主要功能。
> 10. 定义了卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 通过以上功能，这段代码实现了NX中弹出一个对话框让用户选择最多5个对象，并给这些对象编号的功能。
>
