### 【2491】report srfanl params 报告曲面分析参数

#### 代码

```cpp
    /*HEAD REPORT_SRFANL_PARAMS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    /*--------------------------------------------------------------------------  
        See the uf_disp.h header to decode the integer values which are  
        #defined constants.  
    --------------------------------------------------------------------------*/  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

UF_print_syslog是V18版本中新增的函数。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE6F(X)  (write_double_array_to_listing_window(#X, X, 2, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
    #define WRITEnF(X,n)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    /* qq3123197280 */  
    static void report_srfanl_params( UF_DISP_srfanl_data_t *params )  
    {  
        WRITE_F( params->spike_length );     
        WRITE6F( params->ref_plane );  
        WRITE3F( params->ref_vector );  
        WRITE_F( params->mid_value );  
        WRITE_F( params->scale_factor );  
        WRITE_F( params->anal_facet_edge_tol );  
        WRITE_F( params->anal_facet_chord_tol );  
        WRITE_F( params->anal_facet_angle_tol );  
        WRITE_F( params->anal_facet_width_tol );  
        WRITE_D( params->surf_anal_data );  
        WRITE_D( params->surf_anal_fineness );  
        WRITE_D( params->refl_type );  
        WRITE_D( params->num_refl_lines );  
        WRITE_D( params->refl_horiz_vert );  
        WRITE_D( params->thick_refl_lines );  
        WRITE_D( params->reflectivity );  
        WRITE_D( params->refl_move_type );  
        WRITE_D( params->refl_move_position );  
        WRITE_D( params->refl_smooth_lines );  
        WRITE_S( params->refl_user_image_filename );  
        WRITE_D( params->num_contours );  
        WRITE_D( params->surf_anal_display );  
        WRITE_D( params->legend_color_type_gaussian );  
        WRITE_D( params->legend_color_num_gaussian );  
        WRITE_D( params->surf_anal_display_mean );  
        WRITE_F( params->mid_value_mean );  
        WRITE_F( params->scale_factor_mean );  
        WRITE_D( params->legend_color_type_mean );  
        WRITE_D( params->legend_color_num_mean );  
        WRITE_D( params->surf_anal_display_max );  
        WRITE_F( params->mid_value_max );  
        WRITE_F( params->scale_factor_max );  
        WRITE_D( params->legend_color_type_max );  
        WRITE_D( params->legend_color_num_max );  
        WRITE_D( params->surf_anal_display_min );  
        WRITE_F( params->mid_value_min );  
        WRITE_F( params->scale_factor_min );  
        WRITE_D( params->legend_color_type_min );  
        WRITE_D( params->legend_color_num_min );  
        WRITE_D( params->surf_anal_display_normal );  
        WRITE_F( params->mid_value_normal );  
        WRITE_F( params->scale_factor_normal );  
        WRITE_D( params->legend_color_type_normal );  
        WRITE_D( params->legend_color_num_normal );  
        WRITE_D( params->surf_anal_display_sec );  
        WRITE_F( params->mid_value_sec );  
        WRITE_F( params->scale_factor_sec );  
        WRITE_D( params->legend_color_type_sec );  
        WRITE_D( params->legend_color_num_sec );  
        WRITE_D( params->surf_anal_display_u );  
        WRITE_F( params->mid_value_u );  
        WRITE_F( params->scale_factor_u );  
        WRITE_D( params->legend_color_type_u );  
        WRITE_D( params->legend_color_num_u );  
        WRITE_D( params->surf_anal_display_v );  
        WRITE_F( params->mid_value_v );  
        WRITE_F( params->scale_factor_v );  
        WRITE_D( params->legend_color_type_v );  
        WRITE_D( params->legend_color_num_v );  
        WRITE_D( params->surf_anal_display_slope );  
        WRITE_F( params->mid_value_slope );  
        WRITE_F( params->scale_factor_slope );  
        WRITE_D( params->legend_color_type_slope );  
        WRITE_D( params->legend_color_num_slope );  
        WRITE_D( params->surf_anal_display_distance );  
        WRITE_F( params->mid_value_distance );  
        WRITE_F( params->scale_factor_distance );  
        WRITE_D( params->legend_color_type_distance );  
        WRITE_D( params->legend_color_num_distance );  
        WRITE_D( params->surf_anal_display_refl );  
    }  
    static void do_it(void)  
    {  
        UF_DISP_srfanl_data_t   
            params;  
        if( !UF_CALL( UF_DISP_ask_srfanl_params( &params )) )  
        {  
            report_srfanl_params( &params );  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 错误报告功能：通过宏定义UF_CALL，在函数调用失败时打印错误信息。
> 2. 日志写入功能：通过宏定义WRITE_XXX，将变量值写入日志窗口。
> 3. 参数报告函数：定义了report_srfanl_params函数，用于打印NX曲面分析显示参数。
> 4. 主函数：do_it函数通过UF_DISP_ask_srfanl_params获取曲面分析参数，然后调用report_srfanl_params打印参数。
> 5. ufusr函数：这是NX二次开发的入口函数，用于初始化、执行主体逻辑、终止。
> 6. 卸载函数：定义了ufusr_ask_unload函数，用于返回立即卸载标志。
>
> 总体来说，该代码实现了在NX中获取并打印曲面分析参数的功能，同时提供了错误报告和日志写入的辅助功能。
>
