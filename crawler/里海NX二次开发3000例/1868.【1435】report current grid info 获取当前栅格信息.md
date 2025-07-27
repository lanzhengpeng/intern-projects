### 【1435】report current grid info 获取当前栅格信息

#### 代码

```cpp
    /*HEAD REPORT_CURRENT_GRID_INFO CCC UFUN */  
    /*  
        New in UNIGRAPHICS NX  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的功能，用于打印系统日志。 */  
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
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        char  
            msg[132] = { "" };  
        UF_DISP_grid_t   
            output_grid;  
        UF_DISP_ask_grid_parameters ( UF_DISP_ask_current_grid_context ( ),   
                                                             &output_grid );      
        WRITE_L( output_grid.show_grid );   
        WRITE_L( output_grid.snap_to_grid );   
        WRITE_D( output_grid.grid_color );  
        if( output_grid.grid_type == UF_DISP_POLAR_GRID )  
           strcpy( msg, "output_grid.grid_type = UF_DISP_POLAR_GRID\n" );  
        if( output_grid.grid_type == UF_DISP_RECTANGULAR_GRID )  
           strcpy( msg, "output_grid.grid_type = UF_DISP_RECTANGULAR_GRID\n" );  
        UF_UI_write_listing_window( msg );  
        WRITE_L( output_grid.grid_non_uniform );  
        WRITE_L( output_grid.rectangular_grid_emphasis );  
        WRITE_F( output_grid.grid_unit_x );  
        WRITE_F( output_grid.grid_unit_y );  
        WRITE_F( output_grid.grid_line_every_x );  
        WRITE_F( output_grid.grid_line_every_y );  
        WRITE_F( output_grid.grid_emphasis_every_x );  
        WRITE_F( output_grid.grid_emphasis_every_y );  
        WRITE_L( output_grid.polar_grid_emphasis );  
        WRITE_F( output_grid.grid_angular_unit );  
        WRITE_D( output_grid.grid_angular_line_every );  
        WRITE_D( output_grid.grid_angular_emphasis_every );  
        WRITE_F( output_grid.grid_radial_unit );  
        WRITE_D( output_grid.grid_radial_circle_every );  
        WRITE_D( output_grid.grid_radial_emphasis_every );  
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

> 这是一段用于获取并报告当前NX网格信息的二次开发代码。主要包括以下关键点：
>
> 1. 包含了必要的NX头文件，用于调用NX提供的API。
> 2. 定义了宏UF_CALL，用于检测NX API调用是否成功，并在失败时打印错误信息。
> 3. 定义了宏WRITE_L、WRITE_D、WRITE_F，用于在列表窗口中输出逻辑值、整数和浮点数。
> 4. 定义了函数do_it，用于获取当前网格的参数，并调用WRITE_XXX宏输出到列表窗口。
> 5. ufusr函数是用户自定义函数，在NX启动时自动调用。在这个函数中，初始化NX环境，调用do_it函数获取并报告当前网格信息，然后终止NX环境。
> 6. ufusr_ask_unload函数用于卸载用户自定义函数，返回UF_UNLOAD_IMMEDIATELY表示立即卸载。
>
> 这段代码通过调用NX提供的API，获取当前网格的显示、捕捉、颜色、类型、单位等参数，并以列表形式输出到NX的列表窗口，以帮助用户了解当前网格的配置。
>
