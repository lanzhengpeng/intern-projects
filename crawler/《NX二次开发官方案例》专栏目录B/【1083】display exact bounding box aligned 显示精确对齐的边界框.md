### 【1083】display exact bounding box aligned 显示精确对齐的边界框

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_curve.h>  
    #include <uf_vec.h>  
    #include <uf_csys.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_trns.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static tag_t select_an_object(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL,NULL,&resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void map_abs2csys(tag_t input_csys, double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, csys));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
        UF_VEC3_sub(vec, csys, vec);  
    }  
    static void display_temporary_box(double llc[3], double urc[3], tag_t csys)  
    {  
        double  
            dirs[9],  
            delta_x[3],  
            delta_y[3],  
            delta_z[3],  
            end[3],  
            mag,  
            sizes[3],  
            start[3];  
        tag_t  
            csys_mx;  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        if (csys == NULL_TAG)  
            UF_CALL(UF_CSYS_ask_wcs(&csys));  
        UF_VEC3_copy(llc, start);  
        map_abs2csys(csys, start);  
        UF_VEC3_copy(urc, end);  
        map_abs2csys(csys, end);  
        UF_VEC3_sub(end, start, sizes);  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &csys_mx, dirs));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, dirs));  
        UF_VEC3_unitize(&dirs[0], 0, &mag, &dirs[0]);  
        UF_VEC3_unitize(&dirs[3], 0, &mag, &dirs[3]);  
        UF_VEC3_unitize(&dirs[6], 0, &mag, &dirs[6]);  
        UF_VEC3_scale(sizes[0], &dirs[0], delta_x);  
        UF_VEC3_scale(sizes[1], &dirs[3], delta_y);  
        UF_VEC3_scale(sizes[2], &dirs[6], delta_z);  
        UF_VEC3_add(llc, delta_x, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            llc, end, &props));  
        UF_VEC3_add(llc, delta_y, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            llc, end, &props));  
        UF_VEC3_add(llc, delta_z, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            llc, end, &props));  
        UF_VEC3_sub(urc, delta_x, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            urc, end, &props));  
        UF_VEC3_sub(urc, delta_y, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            urc, end, &props));  
        UF_VEC3_sub(urc, delta_z, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            urc, end, &props));  
        UF_VEC3_add(llc, delta_x, start);  
        UF_VEC3_add(start, delta_y, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
        UF_VEC3_add(start, delta_z, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
        UF_VEC3_sub(end, delta_x, start);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
        UF_VEC3_add(start, delta_y, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
        UF_VEC3_sub(end, delta_z, start);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
        UF_VEC3_add(start, delta_x, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
    }  
    static void display_exact_bounding_box_aligned(tag_t object, tag_t csys)  
    {  
        int  
            ii,jj;  
        double  
            min[3],  
            dir[3][3],  
            dist[3],  
            max[3];  
        UF_CALL(UF_MODL_ask_bounding_box_exact(object, csys, min, dir, dist ));  
        for (ii = 0; ii < 3; ii++)  
        {  
            max[ii] = min[ii];  
            for (jj = 0; jj < 3; jj++)  
                max[ii] += dir[jj][ii] * dist[jj];  
        }  
        display_temporary_box( min, max, csys);  
    }  
    static tag_t specify_csys(char *title)  
    {  
        int  
            method = 4, /* inferred 里海译:推断的 */  
            irc;  
        tag_t  
            csys = NULL_TAG;  
        double  
            matrix[9],  
            origin[3];  
        irc = UF_UI_specify_csys(title, &method, matrix, origin, &csys);  
        return csys;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            csys,  
            object;  
        while (((object = select_an_object("Display exact bounding box")) != NULL_TAG) &&  
               ((csys = specify_csys("Display exact bounding box")) != NULL_TAG))  
        {  
            UF_DISP_refresh();  
            display_exact_bounding_box_aligned(object, csys);  
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

> 这段NX二次开发代码的主要功能是在NX环境中选择一个对象，并显示其精确的边界框。
>
> 主要步骤包括：
>
> 1. 定义错误报告函数ECHO，用于在窗口和日志中输出错误信息。
> 2. 定义选择对象的函数select_an_object，通过对话框让用户选择一个对象，并返回其tag。
> 3. 定义坐标系转换函数map_abs2csys，将绝对坐标系下的点转换到指定坐标系下。
> 4. 定义显示临时盒子的函数display_temporary_box，根据指定的坐标系和边界框，显示边界框的8个顶点和12条边。
> 5. 定义显示对象精确边界框的函数display_exact_bounding_box_aligned，首先获取对象在指定坐标系下的精确边界框，然后调用display_temporary_box显示。
> 6. 定义指定坐标系的函数specify_csys，通过对话框让用户指定坐标系。
> 7. 定义主函数do_it，循环调用select_an_object和specify_csys选择对象和坐标系，然后调用display_exact_bounding_box_aligned显示边界框。
> 8. 定义ufusr函数，初始化NX环境，调用do_it，然后终止NX环境。
> 9. 定义卸载函数ufusr_ask_unload，用于卸载该代码。
>
> 总体来说，这段代码通过对话框交互，实现了在NX中指定坐标系下显示对象精确边界框的功能，主要用到了UF_MODL、UF_DISP等NX提供的二次开发接口。
>
