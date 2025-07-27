### 【1990】report angle of line 报告线角度

#### 代码

```cpp
    /*HEAD REPORT_ANGLE_OF_LINE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
    #include <uf_csys.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，V18版本中新增了UF_print_syslog函数。这个函数可以将日志信息打印到系统日志中，而不仅仅是控制台。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a line", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static void ask_wcs_axes(double *xaxis, double *yaxis, double *zaxis)  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        double  
            axes[9],  
            origin[3];  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
        UF_VEC3_copy(&axes[0], xaxis);  
        UF_VEC3_copy(&axes[3], yaxis);  
        UF_VEC3_copy(&axes[6], zaxis);  
    }  
    static void ask_view_axes(tag_t view, double *xdir, double *ydir, double *zdir)  
    {  
        double  
            matrix[9];  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_OBJ_ask_name(view, name));  
        UF_CALL(uc6433(name, matrix));  
        UF_VEC3_copy(&matrix[0], xdir);  
        UF_VEC3_copy(&matrix[3], ydir);  
        UF_VEC3_copy(&matrix[6], zdir);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void do_it(void)  
    {  
        int  
            dim,  
            degrees;  
        tag_t  
            line,  
            work_view;  
        double  
            angle,  
            data[6],  
            x_axis[3] = { 1, 0, 0 },  
            z_axis[3] = { 0, 0, 1 },  
            xdir[3],  
            ydir[3],  
            zdir[3];  
        while ((line = select_a_line("Select line")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_obj_dimensionality(line, &dim, data));  
            UF_VEC3_angle_between(x_axis, &data[3], z_axis, &angle);  
            WRITE("Absolute:  ");  
            WRITE_F(angle * RADEG);  
            ask_wcs_axes(xdir, ydir, zdir);  
            UF_VEC3_angle_between(xdir, &data[3], zdir, &angle);  
            WRITE("WCS:  ");  
            WRITE_F(angle * RADEG);  
            UF_CALL(UF_VIEW_ask_work_view(&work_view));  
            ask_view_axes(work_view, xdir, ydir, zdir);  
            UF_VEC3_angle_between(xdir, &data[3], zdir, &angle);  
            degrees = (int)(angle * RADEG);  
            switch (degrees)  
            {  
                case 0:  
                case 180:  
                    WRITE("Horizontal in current view:  ");  
                    break;  
                case 90:  
                case 270:  
                    WRITE("Vertical in current view:  ");  
                    break;  
                default:  
                    WRITE("Skew in current view:  ");  
                    break;  
            }  
            WRITE_F(angle * RADEG);  
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

> 根据代码，这是一段用于计算和报告线条角度的NX Open C++二次开发代码。
>
> 代码的主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于在出错时输出错误信息。
> 2. 定义了选择线条的函数select_a_line，用于通过对话框让用户选择线条。
> 3. 定义了写入双精度浮点数到列表窗口的函数write_double_to_listing_window。
> 4. 定义了获取坐标系轴的函数ask_wcs_axes和ask_view_axes。
> 5. 定义了主函数do_it，用于计算线条的角度，并写入列表窗口。
> 6. 定义了ufusr函数，作为程序的入口，初始化后调用do_it，最后终止。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载，表示不需要延迟卸载。
> 8. 使用UF_CALL宏封装UF函数调用，用于错误处理。
> 9. 使用WRITE和WRITE_F宏简化写入列表窗口。
> 10. 通过UF_MODL_ask_obj_dimensionality获取线条方向向量，使用UF_VEC3_angle_between计算角度。
> 11. 考虑了绝对坐标系、WCS坐标系和当前视图坐标系下的线条角度。
> 12. 根据角度判断线条在当前视图下是水平、垂直还是倾斜。
> 13. 使用了NX Open C++的UF、UI、VEC、CSYS、VIEW、OBJ等模块。
> 14. 注释中包含了中文说明，方便理解代码。
>
> 综上所述，这段代码实现了选择线条、计算并报告线条在不同坐标系下的角度的功能，代码结构清晰，功能完整，是一个典型的NX二次开发示例。
>
