### 【1434】report current grid display status 报告当前网格显示状态

#### 代码

```cpp
    /*HEAD REPORT_CURRENT_GRID_DISPLAY_STATUS CCC UFUN */  
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
    static void do_it(void)  
    {  
        UF_DISP_grid_t   
            output_grid;  
        UF_DISP_ask_grid_parameters ( UF_DISP_ask_current_grid_context ( ),   
                                                             &output_grid );      
        if( output_grid.show_grid == TRUE )   
            uc1601( "The GRID is ON.", TRUE );  
        if( output_grid.show_grid == FALSE )   
            uc1601( "The GRID is OFF.", TRUE );  
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

> 根据代码内容，这是一段用于NX 8.5及以上版本的二次开发代码，主要功能是获取当前栅格显示状态，并在UI窗口中输出“栅格开启”或“栅格关闭”。
>
> 主要代码逻辑如下：
>
> 1. 包含必要的NX头文件。
> 2. 定义了一个错误报告函数report_error，用于输出错误信息。
> 3. 定义了一个函数do_it，用于获取当前栅格参数并判断栅格显示状态。
> 4. 在do_it函数中，使用UF_DISP_ask_current_grid_context获取当前栅格上下文，然后使用UF_DISP_ask_grid_parameters获取栅格参数。
> 5. 根据获取到的栅格参数中的show_grid字段判断栅格是否显示，并使用uc1601函数在UI窗口中输出相应的提示信息。
> 6. 定义了ufusr函数，用于初始化NX环境、调用do_it函数，然后终止NX环境。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载标志。
> 8. 在注释中注明了NX版本号。
>
> 综上所述，这段代码的主要功能是获取当前栅格显示状态，并在UI窗口中输出提示信息。它通过NX提供的UF_DISP相关函数实现，适用于NX 8.5及以上版本。
>
