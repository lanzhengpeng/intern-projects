### 【1072】display bounding box 显示边界框

#### 代码

```cpp
    /*HEAD DISPLAY_BOUNDING_BOX CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n*** ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 中新增的函数。

只回答译文，不要废话。

因此，翻译结果为：UF_print_syslog 是在 V18 版本中新增的函数。 */  
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
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
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
    static void display_bounding_box(tag_t object)  
    {  
        double  
            box[6],  
            end[3],  
            start[3];  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_MODL_ask_bounding_box(object, box));  
        write_double_array_to_listing_window("box", box, 2, 3);  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[1];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[1];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[1];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[1];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[1];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[4];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[4];  
        start[2] = box[2];  
        end[0] = box[3];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[3];  
        start[1] = box[4];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[5];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
        start[0] = box[0];  
        start[1] = box[4];  
        start[2] = box[5];  
        end[0] = box[0];  
        end[1] = box[4];  
        end[2] = box[2];  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        UF_timer_t  
            timer;  
        UF_timer_values_t  
            calc_time;  
        while ((object = select_anything("Select an object")) != NULL_TAG)  
        {  
            UF_CALL(UF_begin_timer(&timer));  
            display_bounding_box(object);  
            UF_CALL(UF_end_timer(timer, &calc_time));  
            WRITE_F(calc_time.cpu_time);  
            WRITE_F(calc_time.real_time);  
        }  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发示例代码，主要功能是选择一个NX对象并显示其边界框。
>
> 主要步骤包括：
>
> 1. 包含必要的NX Open头文件。
> 2. 定义错误报告函数report_error，用于在出错时输出错误信息。
> 3. 定义选择对象函数select_anything，用于弹出一个对话框让用户选择一个对象。
> 4. 定义写入数组到列表窗口的函数write_double_array_to_listing_window。
> 5. 定义显示边界框的函数display_bounding_box，其中使用UF_MODL_ask_bounding_box函数获取边界框，然后使用UF_DISP_display_temporary_line函数显示边界框的12条线。
> 6. 定义写入单个双精度数到列表窗口的函数write_double_to_listing_window。
> 7. 定义主函数do_it，其中循环调用select_anything函数选择对象，并调用display_bounding_box显示边界框，同时使用UF_timer函数计算时间。
> 8. 定义ufusr函数作为NX二次开发的入口函数，在初始化后调用do_it函数，最后进行终止。
> 9. 定义卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 通过这段代码，用户可以学习如何使用NX Open C++ API进行NX二次开发，实现选择对象并显示其边界框的功能。
>
