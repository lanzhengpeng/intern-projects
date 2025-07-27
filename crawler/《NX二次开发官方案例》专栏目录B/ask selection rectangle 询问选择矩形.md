### ask selection rectangle 询问选择矩形

#### 代码

```cpp
    /*HEAD ASK_SELECTION_RECTANGLE CCC UFUN */  
    /*  To test this example - you must create a simple dialog:  
        1.  Change CHANGE to GTAC  
        2.  Assign the constructor "const_cb" on the Callbacks tab  
        3.  Enable selection on the Selection tab  
        4.  Turn on Rectangle Position on the Selection tab  
        5.  Save the dialog to your application directory as gtac.dlg  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，我理解了UF_print_syslog函数是V18版本中新增的函数。 */  
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
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    /* 里海 */  
    static int sel_cb (int num_sel, tag_p_t sel_objects, int num_deselected,  
        tag_p_t deselected_objs,  void *user_data, UF_UI_selection_p_t select)  
    {  
        int  
            descriptor;  
        tag_t  
            view;  
        double  
            pos1[3],  
            pos2[3],  
            pos3[3],  
            pos4[3];  
        UF_CALL(UF_UI_ask_sel_descriptor(select, &descriptor));  
        if ((descriptor & UF_UI_SEL_DESC_RECTANGLE) &&  
            !UF_CALL(UF_UI_ask_sel_rectangle_pos(select, &view, pos1, pos2, pos3,  
                pos4)))  
        {  
            WRITE_D(view);  
            WRITE3F(pos1);  
            WRITE3F(pos2);  
            WRITE3F(pos3);  
            WRITE3F(pos4);  
        }  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* 里海 */  
    static int GTAC_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            ask_value;  
        ask_value.item_id = UF_STYLER_DIALOG_INDEX;  
        ask_value.item_attr = UF_STYLER_SELECTION;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &ask_value));  
        UF_CALL(UF_UI_set_sel_procs(ask_value.value.selection, NULL, sel_cb,  
            NULL));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        UF_STYLER_callback_info_t  
            GTAC_cbs[2] =  
        {  
         {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, GTAC_const_cb},  
         {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }  
        };  
        UF_CALL(UF_STYLER_create_dialog("gtac.dlg", GTAC_cbs, 2, NULL,  
            &resp));  
    }  
    /* 里海 */  
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

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用出错时打印错误信息。
> 2. 定义了整数写入列表窗口的函数write_integer_to_listing_window，用于在列表窗口中打印整数值。
> 3. 定义了双精度数组写入列表窗口的函数write_double_array_to_listing_window，用于在列表窗口中打印双精度数组。
> 4. 定义了选择回调函数sel_cb，当用户选择对象时，可以获取选择的矩形区域的位置，并打印到列表窗口。
> 5. 定义了GTAC对话框的构造回调函数GTAC_const_cb，用于初始化选择回调函数。
> 6. do_it函数负责创建GTAC对话框，并传入构造回调函数。
> 7. ufusr是NX的二次开发主函数，负责初始化、调用do_it，然后结束。
> 8. ufusr_ask_unload函数用于在卸载NX二次开发时执行一些清理工作。
>
> 总的来说，这段代码实现了在GTAC对话框中，当用户选择对象时，可以获取选择的矩形区域的位置，并打印到列表窗口的功能。
>
