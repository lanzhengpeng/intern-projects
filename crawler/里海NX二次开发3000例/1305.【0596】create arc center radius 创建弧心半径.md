### 【0596】create arc center radius 创建弧心半径

#### 代码

```cpp
    /*HEAD CREATE_ARC_CENTER_RADIUS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_so.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_view.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void do_it(void)  
    {  
        tag_t   
            center = NULL_TAG,  
            help_point = NULL_TAG,    
            support_plane = NULL_TAG,  
            arc_feature_id  = NULL_TAG;    
        logical  
            is_asso = FALSE;    
        double  
            radius = 100.,  
            zero[3] = { 0., 0., 0. },  
            z_axis[3] = { 0., 0., 1. },  
            p1[3] = { 50., 50., 0. },  
            p2[3] = { 150., 50., 0. };  
        UF_CURVE_limit_t   
            limit[2];  
        UF_CURVE_limit_p_t   
            limit_p[2];  
        limit[0].limit_type = UF_CURVE_limit_value;  
        limit[1].limit_type = UF_CURVE_limit_value;  
        limit[0].value = 0.;  
        limit[1].value = 360.;  
        limit_p[0] = &limit[0];  
        limit_p[1] = &limit[1];        
        UF_CALL(UF_CURVE_create_point( p1, &center ));  
        UF_CALL(UF_CURVE_create_point( p2, &help_point ));  
        UF_CALL(UF_MODL_create_fixed_dplane(zero, z_axis, &support_plane));  
        UF_CALL( UF_CURVE_create_arc_center_radius( center,  radius,   
                                 help_point, limit_p, support_plane,   
                                            is_asso, &arc_feature_id ));  
        UF_VIEW_fit_view( NULL_TAG, 0.9 );  
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

> 这段NX二次开发代码的主要功能是创建一个圆心在(50, 50, 0)、半径为100的圆弧，并确保圆弧在XOY平面上。关键步骤如下：
>
> 1. 包含必要的NX开发库头文件。
> 2. 定义错误报告函数report_error，用于打印错误信息。
> 3. 定义一个中心点坐标为(50, 50, 0)。
> 4. 创建中心点坐标为(50, 50, 0)的点，并获取其tag。
> 5. 定义一个辅助点坐标为(150, 50, 0)，并获取其tag。
> 6. 创建固定在XOY平面上的平面，并获取其tag。
> 7. 设置圆弧的起始角度为0度，终止角度为360度。
> 8. 调用UF_CURVE_create_arc_center_radius函数，根据中心点、半径、辅助点、角度范围和平面创建圆弧，并获取圆弧特征的tag。
> 9. 调用UF_VIEW_fit_view函数，将视图缩放至显示整个圆弧。
> 10. 在ufusr函数中调用UF_initialize初始化NX，调用do_it执行创建圆弧操作，然后调用UF_terminate终止NX。
> 11. 提供ufusr_ask_unload函数，用于卸载用户自定义函数。
>
> 通过以上步骤，这段代码实现了在NX中创建一个给定圆心和半径的圆弧的功能。
>
