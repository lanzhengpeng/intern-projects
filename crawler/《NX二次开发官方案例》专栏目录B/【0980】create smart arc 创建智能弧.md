### 【0980】create smart arc 创建智能弧

#### 代码

```cpp
    /*HEAD CREATE_SMART_ARC CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_so.h>  
    #include <uf_curve.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog 是在 V18 版本中新增的函数。因此，针对这个问题，我需要翻译的是：

UF_print_syslog 是在 V18 版本中新增的函数。 */  
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
    static void do_it(void)  
    {  
        tag_t   object_in_part, xform, arc;  
        tag_t   radius_scalar, angle_scalar[2];  
        double  point[3] = { 0, 0, 0};  /* Center point of the arc 译:弧的中心点 */  
        double  x_dir[3] = { 1, 0, 0},  /* x-axis of arc csys      译:弧的x轴坐标 */  
                y_dir[3] = { 0, 1, 0},  /* y-axis of arc csys      译:弧形坐标系的y轴 */  
                scale = 1.0;  
        double  radius = 1.5,           /* radius of the arc       译:弧的半径 */  
                angle1  = 0,  
                angle2  = 3.14;  
        UF_CALL( UF_CURVE_create_point( point, &object_in_part ));  
        UF_CALL( UF_SO_create_scalar_double( object_in_part, UF_SO_update_within_modeling,  
                                             radius, &radius_scalar));  
        UF_CALL( UF_SO_create_scalar_double( object_in_part, UF_SO_update_within_modeling,  
                                             angle1, &angle_scalar[0]));  
        UF_CALL( UF_SO_create_scalar_double( object_in_part, UF_SO_update_within_modeling,  
                                             angle2, &angle_scalar[1]));  
        UF_CALL( UF_SO_create_xform_doubles( object_in_part, UF_SO_update_within_modeling,  
                                             point, x_dir, y_dir, scale, &xform));  
        UF_CALL( UF_SO_create_arc_radius_angles( object_in_part, UF_SO_update_within_modeling,  
                                                 xform, radius_scalar, angle_scalar, &arc));  
        UF_CALL( UF_SO_set_visibility_option( arc, UF_SO_visible));  
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

> 这段代码是NX的二次开发代码，主要用于创建一个智能弧，并设置其属性。代码的主要功能如下：
>
> 1. 包含必要的NX API头文件，如uf.h、uf_ui.h等。
> 2. 定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 3. 定义了主函数do_it，用于创建和设置智能弧。
> 4. 在do_it函数中，首先创建一个点作为弧的中心点。
> 5. 接着，创建弧的半径和两个角度参数的标量，并将其设置为1.5和0到3.14。
> 6. 然后，创建弧所在坐标系的变换。
> 7. 利用上述参数，创建一个智能弧。
> 8. 最后，设置弧的可见性。
> 9. ufusr是NX调用二次开发程序的入口函数，其中初始化NX环境，调用do_it函数，并终止NX环境。
> 10. ufusr_ask_unload用于设置二次开发程序的卸载方式。
>
> 总体而言，这段代码通过NX提供的API，实现了创建一个指定参数的智能弧的功能。
>
