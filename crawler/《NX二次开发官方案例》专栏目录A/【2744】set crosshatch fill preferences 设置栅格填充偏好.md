### 【2744】set crosshatch fill preferences 设置栅格填充偏好

#### 代码

```cpp
    /*HEAD SET_CROSSHATCH_FILL_PREFERENCES CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_view.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_curve.h>  
    #include <uf_csys.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，V18版本新增了一个名为UF_print_syslog的函数。 */  
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
    static void set_hatch_fill_preferences(void)  
    {  
        UF_DRF_hatch_fill_preferences_t  
            prefs;     
        UF_CALL(UF_DRF_ask_hatch_fill_preferences(&prefs));  
        prefs.hatch_distance = (prefs.hatch_distance*2);  
        UF_CALL(UF_DRF_set_hatch_fill_preferences(&prefs));    
    }  
    static logical specify_position(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            wcs,  
            arc[1],  
            view,  
            xhatch;  
        int  
            obj_bnd[1] = { 1 };  
        double  
            pos[3];  
        char  
            view_name[30];  
        UF_CURVE_arc_t   
            arc_coords;    
        while (specify_position("Indicate Arc center", pos))  
        {  
            arc_coords.start_angle = 0.0;  
            arc_coords.end_angle = 360.0 * DEGRA;  
            arc_coords.arc_center[0] = pos[0];  
            arc_coords.arc_center[1] = pos[1];  
            arc_coords.arc_center[2] = pos[2];  
            arc_coords.radius = 2.0;  
            UF_CALL(UF_CSYS_ask_wcs(&wcs));  
            UF_CALL(UF_CSYS_ask_matrix_of_object(wcs, &arc_coords.matrix_tag));  
            UF_CALL(UF_CURVE_create_arc(&arc_coords, &arc[0]));      
            UF_CALL(uc1653(view_name));  
            printf("view name is : %s\n", view_name);  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view));  
            set_hatch_fill_preferences();  
            UF_CALL(UF_DRF_create_crosshatch(1, obj_bnd, arc, view, &xhatch));  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++的二次开发示例，用于创建交叉线填充图案。
>
> 代码的主要功能包括：
>
> 1. 设置交叉线填充的偏好设置，包括线间距加倍。
> 2. 提示用户指定圆弧中心位置，并根据该位置创建一个圆弧。
> 3. 提示用户输入视图名称，并获取该视图的标签。
> 4. 设置交叉线填充的偏好设置。
> 5. 在指定的视图上，为创建的圆弧创建交叉线填充图案。
> 6. 通过错误处理函数，输出错误信息到系统日志和列表窗口。
> 7. 通过UF_initialize和UF_terminate初始化和终止NX Open。
> 8. 提供卸载函数，以便立即卸载用户自定义函数。
>
> 该代码通过交互式提示用户输入圆弧中心和视图名称，然后创建交叉线填充图案，展示了NX二次开发的基本流程和交互方式。
>
