### 【2099】report face props 报告面属性

#### 代码

```cpp
    /*HEAD REPORT_FACE_PROPS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_vec.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void map_abs2view(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_view2abs(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    /* qq3123197280 */  
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_point_on_face(char *prompt, double cp[3])  
    {  
        int  
            n,  
            resp;  
        tag_t  
            face,  
            line,  
            object,  
            view;  
        double  
            tol;  
        UF_CURVE_line_t  
            lp;  
        UF_MODL_intersect_info_p_t  
            *ints;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            map_abs2view(cp);  
            lp.start_point[0] = cp[0];  
            lp.start_point[1] = cp[1];  
            lp.start_point[2] = cp[2] + 1000;  
            lp.end_point[0] = cp[0];  
            lp.end_point[1] = cp[1];  
            lp.end_point[2] = cp[2] - 1000;  
            map_view2abs(lp.start_point);  
            map_view2abs(lp.end_point);  
            UF_CALL(UF_CURVE_create_line(&lp, &line));  
            face = object;  
            UF_MODL_ask_distance_tolerance(&tol);  
            UF_CALL(UF_MODL_intersect_objects(face, line, tol, &n, &ints));  
            if (n > 0)  
            {  
                UF_VEC3_copy(ints[0]->intersect.point.coords, cp);  
                UF_free(ints);  
            }  
            UF_CALL(UF_OBJ_delete_object(line));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_face_props(tag_t face, double center[3])  
    {  
        double  
            parm[2],  
            pnt[3],  
            dirU1[3],  
            dirU2[3],  
            dirV1[3],  
            dirV2[3],  
            norm_dir[3],  
            radii[2];  
        char  
            msg[256] = { "" };  
        UF_CALL(UF_MODL_ask_face_parm(face, center, parm, pnt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, dirU1, dirV1, dirU2,  
            dirV2, norm_dir, radii));  
        sprintf(msg, "\n  parm = %f, %f\n", parm[0], parm[1]);  
        WRITE( msg );  
        sprintf(msg, "  pnt = %f, %f, %f\n", pnt[0], pnt[1], pnt[2]);  
        WRITE( msg );  
        sprintf(msg, "  dirU1 = %f, %f, %f\n", dirU1[0], dirU1[1], dirU1[2]);  
        WRITE( msg );  
        sprintf(msg, "  dirV1 = %f, %f, %f\n", dirV1[0], dirV1[1], dirV1[2]);  
        WRITE( msg );  
        sprintf(msg, "  dirU2 = %f, %f, %f\n", dirU2[0], dirU2[1], dirU2[2]);  
        WRITE( msg );  
        sprintf(msg, "  dirV2 = %f, %f, %f\n", dirV2[0], dirV2[1], dirV2[2]);  
        WRITE( msg );  
        sprintf(msg, "  norm_dir = %f, %f, %f\n", norm_dir[0], norm_dir[1], norm_dir[2]);  
        WRITE( msg );  
        sprintf(msg, "  radii = %f, %f\n", radii[0], radii[1]);  
        WRITE( msg );  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face;  
        double  
            pos[3];  
        while ((face = select_point_on_face("Select face point", pos)) != NULL_TAG)  
            report_face_props(face, pos);  
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

> 这段代码是一个NX Open C++二次开发示例，用于查询选择的面要素的属性，并将这些属性输出到列表窗口。
>
> 主要功能包括：
>
> 1. 定义了错误报告宏和函数，用于捕捉函数调用错误。
> 2. 定义了坐标系转换函数，用于将绝对坐标系转换为当前视图坐标系。
> 3. 定义了面要素选择掩码函数，用于选择面要素。
> 4. 定义了基于选择的点查询面要素的函数，并计算与该点最近的面要素。
> 5. 定义了查询面要素属性并输出到列表窗口的函数。
> 6. 主函数中循环调用面要素选择和属性输出，直到用户取消选择。
> 7. 定义了ufusr函数，用于在NX环境中加载和执行该代码。
>
> 总体来说，这段代码通过二次开发接口，实现了选择面要素并查询其属性的功能。
>
