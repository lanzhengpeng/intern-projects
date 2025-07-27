### 【1315】flatten spline “展平样条线”

#### 代码

```cpp
    /*HEAD FLATTEN_SPLINE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_group.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的函数，用于打印系统日志消息。 */  
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
    static int mask_for_spline_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_spline_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_spline_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a spline curve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_spline_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void flatten_spline(tag_t spline)  
    {  
        int  
            n_members;  
        tag_t  
            grp,  
            *members,  
            plane;  
        double  
            point[3],  
            x[3],  
            z[3];  
        UF_CURVE_proj_t  
            data;  
        UF_CALL(UF_MODL_ask_curve_props(spline, 0.5, point, x, x, z, x, x));  
        UF_CALL(UF_MODL_create_plane(point, z, &plane));  
        data.proj_type = 1;  
        UF_CALL(UF_CURVE_create_proj_curves(1,&spline, 1,&plane, 1, &data, &grp));  
        UF_CALL(UF_GROUP_ask_group_data(grp, &members, &n_members));  
        UF_CALL(UF_GROUP_ungroup_all(grp));  
        if (n_members > 0)  
        {  
            UF_CALL(UF_OBJ_replace_object_data(spline, members[0]));  
            UF_free(members);  
        }  
        UF_CALL(UF_OBJ_delete_object(plane));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            spline;  
        while ((spline = select_a_spline_curve("Flatten")) != NULL_TAG)  
            flatten_spline(spline);  
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

> 根据代码的注释和结构，这是一段用于NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于打印出函数调用失败的错误信息。
> 2. 定义了mask_for_spline_curves函数，用于设置选择模式，只选择样条曲线。
> 3. 定义了select_a_spline_curve函数，用于通过对话框选择一条样条曲线，并高亮显示。
> 4. 定义了flatten_spline函数，用于将样条曲线投影到平面上，然后替换原始样条曲线。
> 5. 定义了do_it函数，用于循环选择样条曲线，并调用flatten_spline函数进行投影处理。
> 6. 定义了ufusr函数，作为程序的入口函数，调用do_it函数执行主逻辑。
> 7. 定义了ufusr_ask_unload函数，用于卸载该程序。
>
> 总的来说，这段代码提供了一个简单的人机交互界面，用于选择样条曲线，然后将其投影到平面上，实现了样条曲线的扁平化处理。代码结构清晰，功能明确。
>
