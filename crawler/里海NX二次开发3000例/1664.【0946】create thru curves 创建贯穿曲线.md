### 【0946】create thru curves 创建贯穿曲线

#### 代码

```cpp
    /*HEAD CREATE_THRU_CURVES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
            s_section,  
            s_spine;  
        int  
            alignment = 1,  
            body_type = UF_MODL_SOLID_BODY,  
            c_flag[2] = { 0,0 },  
            patch = 1,  
            vdegree = 3,  
            vstatus = 0;  
        double  
            tol[3] = { 0, 0, 0 },  
            value[6] = { 0,0,0, 0,0,0 };  
        tag_t  
            body_id,  
            c_face[2] = { NULL_TAG, NULL_TAG };  
        /*  Create the section curves 里海译:创建截面曲线 */  
        UF_MODL_init_string_list(&s_section);  
        UF_MODL_create_string_list(3, 12, &s_section);  
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
        s_section.string[1] = 4;  
        s_section.dir[1] = 1;  
        vec3_init(line_coords.start_point);  
        vec3_init(line_coords.end_point);  
        line_coords.start_point[0] = 10;  
        line_coords.end_point[0] = 10;  
        line_coords.end_point[1] = 2;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[4] ));  
        UF_VEC3_copy(line_coords.end_point, line_coords.start_point);  
        line_coords.end_point[2] = 2;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[5] ));  
        UF_VEC3_copy(line_coords.end_point, line_coords.start_point);  
        line_coords.end_point[1] = 0;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[6] ));  
        UF_VEC3_copy(line_coords.end_point, line_coords.start_point);  
        line_coords.end_point[2] = 0;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[7] ));  
        s_section.string[2] = 4;  
        s_section.dir[2] = 1;  
        vec3_init(line_coords.start_point);  
        vec3_init(line_coords.end_point);  
        line_coords.start_point[0] = 20;  
        line_coords.end_point[0] = 20;  
        line_coords.end_point[1] = 3;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[8] ));  
        UF_VEC3_copy(line_coords.end_point, line_coords.start_point);  
        line_coords.end_point[2] = 3;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[9] ));  
        UF_VEC3_copy(line_coords.end_point, line_coords.start_point);  
        line_coords.end_point[1] = 0;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[10] ));  
        UF_VEC3_copy(line_coords.end_point, line_coords.start_point);  
        line_coords.end_point[2] = 0;  
        UF_CALL( UF_CURVE_create_line (&line_coords, &s_section.id[11] ));  
        /*  No spine 里海译:没有脊梁 */  
        UF_MODL_init_string_list(&s_spine);  
        UF_CALL( UF_MODL_create_thru_curves ( &s_section, &s_spine, &patch,  
            &alignment, value, &vdegree, &vstatus, &body_type, UF_NULLSIGN,  
            tol, c_face, c_flag, &body_id ));  
        UF_MODL_free_string_list(&s_section);  
        UF_MODL_free_string_list(&s_spine);  
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

> 这段代码是用于NX的二次开发，其主要功能是创建通过曲线扫掠出的实体。具体来说：
>
> 1. 定义了报告错误函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义了初始化向量函数vec3_init，用于将一个3维向量初始化为(0,0,0)。
> 3. 定义了主函数do_it，用于执行创建实体的操作。在主函数中：
>
> 1. 定义了ufusr函数，用于在NX环境中初始化、执行主函数，并在完成后进行清理。
> 2. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示该二次开发程序可以被立即卸载。
>
> 总的来说，这段代码实现了在NX中通过曲线扫掠生成实体的功能。
>
