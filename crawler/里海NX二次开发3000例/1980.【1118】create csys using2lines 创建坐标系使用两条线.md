### 【1118】create csys using2lines 创建坐标系使用两条线

#### 代码

```cpp
    /*HEAD CREATE_CSYS_USING2LINES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_csys.h>  
    #include <uf_mtx.h>  
    #include <uf_vec.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的功能。 */  
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
    static int mask_for_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_line_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_line(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            csys_id,  
            matrix_id,  
            x_axis,  
            y_axis;  
        UF_CURVE_line_t  
            x_coords,  
            y_coords;  
        double  
            csys_matrix[9],  
            x_vec[3],  
            y_vec[3];  
        while (  
            ((x_axis = select_a_line("Create CSYS - Select X-axis")) != NULL_TAG)  
            &&  
            ((y_axis = select_a_line("Create CSYS - Select Y-axis")) != NULL_TAG))  
        {  
            UF_CALL(UF_CURVE_ask_line_data(x_axis, &x_coords));  
            UF_CALL(UF_CURVE_ask_line_data(y_axis, &y_coords));  
            UF_VEC3_sub(x_coords.end_point, x_coords.start_point, x_vec);  
            UF_VEC3_sub(y_coords.end_point, y_coords.start_point, y_vec);  
            if (!UF_CALL(UF_MTX3_initialize(x_vec, y_vec, csys_matrix)))  
            {  
                UF_CALL(UF_CSYS_create_matrix(csys_matrix, &matrix_id));  
                UF_CALL(UF_CSYS_create_csys(x_coords.start_point, matrix_id,  
                    &csys_id));  
            }  
        }  
    }  
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

> 这段代码是一个用于在NX中创建坐标系的用户函数。其主要功能包括：
>
> 1. 通过两个提示框，选择两条线作为坐标系的新X轴和Y轴。
> 2. 获取这两条线的起点和终点坐标，并计算出X轴和Y轴的方向向量。
> 3. 利用X轴和Y轴的方向向量，计算出一个新的坐标系矩阵。
> 4. 在第一条线的起点处，根据新计算的坐标系矩阵，创建一个新的坐标系。
> 5. 提供错误报告函数，用于打印出函数调用失败时的错误信息。
> 6. 提供卸载函数，用于立即卸载用户函数。
> 7. 用户函数通过UF_initialize和UF_terminate初始化和关闭NX API。
> 8. 用户函数在NX启动时自动运行。
> 9. 用户函数通过UF_CALL宏来简化NX API函数的调用。
> 10. 用户函数通过UF_PRINT_SYSLOG宏来打印日志信息。
>
> 总体来说，这是一个用于在NX中通过选择两条线创建新坐标系的二次开发示例代码。
>
