### 【0593】create angular law sweep 创建角度规律扫掠

#### 代码

```cpp
    /*HEAD CREATE_ANGULAR_LAW_SWEEP CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的函数，用于打印系统日志。 */  
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
    static void vec3_init(double vec[3])  
    {  
        vec[0] = 0;  
        vec[1] = 0;  
        vec[2] = 0;  
    }  
    static void do_it(void)  
    {  
        UF_CURVE_line_t  
            line_coords;  
        UF_STRING_t  
            s_guide,  
            s_section,  
            s_law,  
            s_spine;  
        UF_METHOD_t  
            orientation,  
            scaling;  
        int  
            alignment = 1,  
            body_type = 1,  
            inter = UF_MODL_LINEAR;  
        double  
            tol[3] = { 0, 0, 0 };  
        tag_t  
            body_id;  
        /*  Create the guide curves 译:创建导向曲线 */  
        UF_MODL_init_string_list(&s_guide);  
        UF_MODL_create_string_list(1, 1, &s_guide);  
        s_guide.num = 1;  
        s_guide.string[0] = 1;  
        s_guide.dir[0] = 1;  
        vec3_init(line_coords.start_point);  
        vec3_init(line_coords.end_point);  
        line_coords.end_point[0] = 10;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_guide.id[0] ));  
        /*  Create the section curves 译:创建截面曲线 */  
        UF_MODL_init_string_list(&s_section);  
        UF_MODL_create_string_list(1, 4, &s_section);  
        s_section.num = 1;  
        s_section.string[0] = 4;  
        s_section.dir[0] = 1;  
        vec3_init(line_coords.start_point);  
        vec3_init(line_coords.end_point);  
        line_coords.end_point[1] = 1;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[0] ));  
        UF_VEC3_copy(line_coords.end_point, line_coords.start_point);  
        line_coords.end_point[2] = 1;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[1] ));  
        UF_VEC3_copy(line_coords.end_point, line_coords.start_point);  
        line_coords.end_point[1] = 0;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[2] ));  
        UF_VEC3_copy(line_coords.end_point, line_coords.start_point);  
        line_coords.end_point[2] = 0;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[3] ));  
        /* Create law curve 译:Create law curve的翻译为：创建法律曲线。 */  
        UF_MODL_init_string_list(&s_law);  
        UF_MODL_create_string_list(1, 1, &s_law);  
        s_law.num = 1;  
        s_law.string[0] = 1;  
        s_law.dir[0] = 1;  
        vec3_init(line_coords.start_point);  
        vec3_init(line_coords.end_point);  
        line_coords.end_point[0] = 10;  
        line_coords.end_point[1] = 10;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_law.id[0] ));  
        /*  No spine 译:没有脊椎 */  
        UF_MODL_init_string_list(&s_spine);  
        orientation.method = 6;  
        orientation.s_curve = &s_law;  
        orientation.value[0] = orientation.value[2] = 0.0;  
        orientation.value[1] = 1.0;  
        orientation.id = 0;  
        orientation.inter = 0;  
        scaling.method = 1;  
        scaling.value[0] = 1;  
        UF_CALL( UF_MODL_create_sweep ( &s_guide,  
                                        &s_section,  
                                        &s_spine,  
                                        &orientation,  
                                        &scaling,  
                                        &alignment,  
                                        &inter,  
                                        &body_type,  
                                        UF_NULLSIGN,  
                                        tol,  
                                        &body_id ));  
         UF_MODL_free_string_list(&s_guide);  
         UF_MODL_free_string_list(&s_section);  
         UF_MODL_free_string_list(&s_spine);  
         UF_MODL_free_string_list(&s_law);  
    }  
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

> 这段代码是用于NX CAD系统的一个二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在调用UF函数失败时输出错误信息。
> 2. 定义了一个初始化三维向量的函数vec3_init。
> 3. 实现了主要功能函数do_it，其中：创建了一条直线作为引导曲线。创建了一条矩形作为截面曲线。创建了一条直线作为扫掠法则曲线。定义了扫掠的方向和比例。调用UF_MODL_create_sweep函数创建了一个扫掠体。
> 4. 创建了一条直线作为引导曲线。
> 5. 创建了一条矩形作为截面曲线。
> 6. 创建了一条直线作为扫掠法则曲线。
> 7. 定义了扫掠的方向和比例。
> 8. 调用UF_MODL_create_sweep函数创建了一个扫掠体。
> 9. 实现了ufusr函数，在NX启动时调用do_it函数。
> 10. 实现了ufusr_ask_unload函数，用于在NX卸载用户程序时调用。
>
> 总体来说，这段代码通过UF函数实现了从引导曲线、截面曲线和扫掠法则曲线创建一个扫掠体的功能，展示了NX二次开发中曲线和扫掠体的基本操作。
>
