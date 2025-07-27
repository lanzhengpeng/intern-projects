### 【2322】report selected datum axis parameters 报告选定的基准轴参数

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_DATUM_AXIS_PARAMETERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_so.h>  
    #include <uf_undo.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，V18版本中新增了UF_print_syslog函数。 */  
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
    static int mask_for_datum_axes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_datum_axis_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_datum_axis(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a datum axis", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_datum_axes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
    }  
    static void report_datum_axis_parms(tag_t daxis)  
    {  
        int  
            ii;  
        tag_t  
            assy_dirr,  
            assy_pnt,  
            comp_dirr,  
            comp_pnt,  
            feat,  
            xform,  
            xyz[3];  
        double  
            origin[3],  
            normal[3];  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_MODL_ask_object_feat(daxis, &feat));  
        UF_CALL(UF_MODL_ask_datum_axis_parms(feat, origin, normal));  
        if (UF_ASSEM_is_occurrence(daxis))  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
            UF_CALL(UF_SO_create_dirr_doubles(feat, UF_SO_update_after_modeling,  
                normal, &comp_dirr));  
            for (ii = 0; ii < 3; ii++)  
                UF_CALL(UF_SO_create_scalar_double(feat,  
                    UF_SO_update_after_modeling, origin[ii], &xyz[ii]));  
            UF_CALL(UF_SO_create_point_3_scalars(feat, UF_SO_update_after_modeling,  
                xyz, &comp_pnt));  
            UF_CALL(UF_SO_create_xform_assy_ctxt(daxis,  
                UF_ASSEM_ask_part_occurrence(daxis), NULL_TAG, &xform));  
            UF_CALL(UF_SO_create_dirr_extract(daxis, UF_SO_update_after_modeling,  
                comp_dirr, xform, &assy_dirr));  
            UF_CALL(UF_SO_create_point_extract(daxis, UF_SO_update_after_modeling,  
                comp_pnt, xform, &assy_pnt));  
            UF_CALL(UF_SO_ask_direction_of_dirr(assy_dirr, normal));  
            UF_CALL(UF_CURVE_ask_point_data(assy_pnt, origin));  
            UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
            UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
        }  
        display_conehead(origin, normal);  
        WRITE3F(origin);  
        WRITE3F(normal);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            daxis;  
        while ((daxis = select_a_datum_axis("Report parameters")) != NULL_TAG)  
            report_datum_axis_parms(daxis);  
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

> 根据代码内容，这是一个用于在NX中查询并显示基准轴参数的二次开发代码。
>
> 主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于在出错时输出错误信息。
> 2. 定义了选择基准轴的函数select_a_datum_axis，使用对话框让用户选择一个基准轴。
> 3. 定义了显示圆锥头的函数display_conehead，用于在基准轴上显示圆锥头。
> 4. 定义了查询并显示基准轴参数的函数report_datum_axis_parms，包括查询基准轴的创建特征、原点和方向，以及在图形区显示圆锥头。
> 5. 定义了主函数do_it，循环调用select_a_datum_axis选择基准轴，并调用report_datum_axis_parms函数报告每个基准轴的参数。
> 6. 定义了ufusr函数，作为程序的入口，初始化NX，调用do_it函数，然后终止NX。
> 7. 定义了卸载函数ufusr_ask_unload，用于卸载当前加载项。
>
> 总体来说，这是一个典型的NX二次开发代码，实现了选择对象、查询参数、显示结果等功能，并遵循了NX二次开发的规范。
>
