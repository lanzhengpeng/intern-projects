### 【0863】create relative dplane normal to line 创建与线垂直的相对平面

#### 代码

```cpp
    /*HEAD CREATE_RELATIVE_DPLANE_NORMAL_TO_LINE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_trns.h>  
    #include <uf_so.h>  
    #include <uf_defs.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_vec.h>  
    #include <uf_view.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog在V18版本中是新增的函数。

仅回答翻译，不要添加其他内容。

UF_print_syslog在V18版本中是新增的函数。 */  
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
    /* 里海 */  
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
    /* 里海 */  
    static void map_abs2view(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs_mx, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    /* 里海 */  
    static void map_view2abs(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs_mx, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    /* 里海 */  
    static void ask_pos_on_obj(tag_t obj, double loc[3])  
    {  
        tag_t  
            line;  
        double  
            cp[3],  
            dist;  
        UF_CURVE_line_t  
            lp;  
        map_abs2view(loc);  
        lp.start_point[0] = loc[0];  
        lp.start_point[1] = loc[1];  
        lp.start_point[2] = loc[2] + 1000;  
        lp.end_point[0] = loc[0];  
        lp.end_point[1] = loc[1];  
        lp.end_point[2] = loc[2] - 1000;  
        map_view2abs(lp.start_point);  
        map_view2abs(lp.end_point);  
        UF_CALL(UF_CURVE_create_line(&lp, &line));  
        UF_CALL(UF_MODL_ask_minimum_dist(obj, line, 0, cp, 0, cp, &dist, loc, cp));  
        UF_CALL(UF_OBJ_delete_object(line));  
    }  
    /* 里海 */  
    static void ask_line_close_endpoint(tag_t line, double pos[3], double *sclval)  
    {  
        double  
            parm[3];  
        UF_CURVE_line_t  
            line_coords;  
        UF_CALL(UF_CURVE_ask_line_data(line, &line_coords));  
        UF_CALL(UF_MODL_ask_curve_parm(line, line_coords.start_point, &parm[0],  
                                       line_coords.start_point));  
        UF_CALL(UF_MODL_ask_curve_parm(line, pos, &parm[1], pos));  
        UF_CALL(UF_MODL_ask_curve_parm(line, line_coords.end_point, &parm[2],  
                                       line_coords.end_point));  
        if (fabs(parm[1] - parm[0]) < fabs(parm[2] - parm[1]))  
        {  
             UF_VEC3_copy(line_coords.start_point, pos);  
             *sclval = 0.0;  
        }  
        else   
        {  
            UF_VEC3_copy(line_coords.end_point, pos);  
            *sclval = 1.0;  
       }  
    }  
    /* 里海 */  
    static tag_t select_a_line_endpoint( char *prompt, double pos[3],  
                                        double *scl_val )  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select line endpoint", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,  
            &object, pos, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, pos);  
            ask_line_close_endpoint(object, pos, scl_val);  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {          
        tag_t  
            new_dplane = NULL_TAG,  
            direction = NULL_TAG,  
            smart_point = NULL_TAG,  
            sclr = NULL_TAG,  
            line = NULL_TAG;      
        double  
            ep[3] = { 0., 0., 0. },  
            scale_value = 0.;  
        while( (line = select_a_line_endpoint( "Select a Line:",   
                                               ep, &scale_value )) !=NULL_TAG )  
        {          
            UF_CALL(UF_SO_create_scalar_double(line, UF_SO_update_within_modeling,  
                                                             scale_value, &sclr ));  
            UF_CALL(UF_SO_create_point_on_curve(line, UF_SO_update_within_modeling,  
                                                        line, sclr, &smart_point ));  
            /*         uncomment this if you want to see the smart point                               
            UF_CALL( UF_SO_set_visibility_option( smart_point, UF_SO_visible ));  
            */  
            UF_CALL( UF_SO_create_dirr_line( line, UF_SO_update_within_modeling,  
                                                        line, FALSE, &direction ));     
            UF_CALL( UF_MODL_create_point_dirr_dplane( smart_point, direction,   
                                                                  &new_dplane ));  
        }  
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

> 这段代码是用于NX的二次开发，其主要功能是在NX中创建一个垂直于选定直线的相对平面。
>
> 主要步骤包括：
>
> 1. 定义错误报告函数report_error，用于在发生错误时输出错误信息。
> 2. 定义mask_for_lines函数，用于设置选择过滤器，以只选择线类型。
> 3. 定义map_abs2view和map_view2abs函数，用于将绝对坐标系坐标转换为视图坐标系坐标，反之亦然。
> 4. 定义ask_pos_on_obj函数，用于在选定对象上获取一个点。
> 5. 定义ask_line_close_endpoint函数，用于获取直线的一个端点。
> 6. 定义select_a_line_endpoint函数，用于选择直线的一个端点。
> 7. 在do_it函数中，循环调用select_a_line_endpoint函数选择直线端点，然后创建一个标量，创建曲线上的一个点，创建直线的方向，最后创建垂直于直线的平面。
> 8. 定义ufusr函数，用于初始化NX环境，调用do_it函数，然后结束NX环境。
> 9. 定义ufusr_ask_unload函数，用于卸载用户函数。
>
> 这段代码通过NX的二次开发接口实现了在NX中创建垂直于选定直线的相对平面的功能。
>
