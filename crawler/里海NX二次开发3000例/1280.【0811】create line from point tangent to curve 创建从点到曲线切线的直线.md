### 【0811】create line from point tangent to curve 创建从点到曲线切线的直线

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_csys.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_view.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
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
    static void do_it(void)  
    {  
        tag_t  
            pt = NULL_TAG,  
            pt2 = NULL_TAG,  
            arc = NULL_TAG,  
            wcs = NULL_TAG,  
            plane = NULL_TAG,  
            newLine = NULL_TAG,  
            csys_mtx = NULL_TAG;  
        double  
            p_loc[3] = { 25., 0., 0. },  
            p_loc2[3] = { 0., 10, 0. },  
            zero[3] = { 0, 0, 0 },  
            z_axis[3] = { 0, 0, 1 };  
        UF_CURVE_arc_t  
            arc_data = { NULL_TAG, 0, TWOPI, { 0., 0., 0. }, 10.0 };  
        UF_CURVE_help_data_t      
            help_data_p;  
        UF_CURVE_limit_t   
            limit[2];  
        UF_CURVE_limit_p_t   
            limit_p[2];  
        limit[0].limit_type = UF_CURVE_limit_value;  
        limit[1].limit_type = UF_CURVE_limit_value;  
        limit[0].value = 0.;  
        limit[1].value = 50.;  
        limit_p[0] = &limit[0];  
        limit_p[1] = &limit[1];        
        UF_CSYS_ask_wcs( & wcs );      
        UF_CALL( UF_CSYS_ask_matrix_of_object( wcs, &csys_mtx ));  
        arc_data.matrix_tag = csys_mtx;  
        UF_CALL( UF_DISP_set_name_display_status( UF_DISP_NAME_DISPLAY_ON ));  
        UF_CALL(UF_MODL_create_fixed_dplane(zero, z_axis, &plane));  
        UF_CALL( UF_CURVE_create_point( p_loc, &pt ));  
        UF_OBJ_set_name( pt, "POINT" );  
        UF_CALL( UF_CURVE_create_point( p_loc2, &pt2));  
        UF_OBJ_set_name( pt2, "HELPER" );  
        UF_CALL( UF_CURVE_create_arc(&arc_data, &arc));     
        help_data_p.help_data_type = UF_CURVE_help_data_value;   
        UF_VEC3_copy(p_loc2,help_data_p.value);  
        UF_CALL( UF_CURVE_create_line_point_tangent( pt, arc, &help_data_p, limit_p, plane, TRUE, &newLine ));   
        UF_VIEW_fit_view( NULL_TAG, 0.8 );  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize()))   
        {  
            WRITE( "\n\nIs this program signed?\n\n" );  
            WRITE( "Or is there an Author License available?\n\n" );  
            return;  
        }  
        else  
        {  
            do_it();  
        }  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一段NX Open C++ API编写的NX二次开发代码。主要功能是创建一个圆弧和一个点，然后根据这两个对象创建一条直线。
>
> 主要步骤包括：
>
> 1. 包含所需的NX Open C++ API头文件。
> 2. 定义了一个ECHO函数，用于打印信息到NX日志窗口。
> 3. 定义了一个UF_CALL宏，用于调用NX API函数，并捕获错误。
> 4. 定义了一个do_it函数，用于执行创建对象和直线的操作。
> 5. 在do_it函数中，首先获取当前工作坐标系。
> 6. 创建一个固定平面。
> 7. 创建一个圆弧，中心在原点，半径为10。
> 8. 创建一个点作为直线起点。
> 9. 创建一个辅助点，作为直线方向参考。
> 10. 调用UF_CURVE_create_line_point_tangent函数，根据起点、圆弧和辅助点创建直线。
> 11. 调用UF_VIEW_fit_view函数，将视图缩放到新创建的直线。
> 12. 在ufusr函数中，初始化NX，然后调用do_it函数。
> 13. 在ufusr_ask_unload函数中，返回立即卸载标志。
>
> 总体来说，这段代码通过NX Open C++ API实现了创建圆弧、点和直线的功能。
>
