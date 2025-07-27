### 【1754】plot drawings after setting line widths 绘图线宽设置后进行绘图

#### 代码

```cpp
    /*HEAD PLOT_DRAWINGS_AFTER_SETTING_LINE_WIDTHS CCC UFUN */  
    /* This contains functions introduced in nx3 里海译:This contains functions introduced in nx3 */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_plot.h>  
    #include <uf_ui.h>  
    #include <uf_cgm.h>  
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
    /* qq3123197280 */  
    extern DllExport void ufusr( char *parm, int *returnCode, int rlen )  
    {  
        UF_PLOT_job_options_t       job_options;  
        UF_PLOT_banner_options_t    banner_options;  
        char                       *job_name;  
        int                         copies;  
        tag_t                       display_part;  
        tag_t                       drawing_sheet;  
        UF_PLOT_custom_widths_t session_original_custom_widths;  
        UF_PLOT_custom_widths_t sess_custom_widths;  
        /* Initialize the API environment 里海译:初始化API环境 */  
        if( UF_CALL(UF_initialize()) )   
        {  
            /* Failed to initialize 里海译:翻译为：初始化失败。 */  
            return;  
        }  
        /*  If no part is displayed, do nothing.  里海译:如果没有任何部件显示，则不执行任何操作。 */  
        display_part = UF_PART_ask_display_part();  
        if (display_part == NULL_TAG)  
        {  
            return;  
        }  
    /*  
        Get the default job options and banner options for use in  
        the "print" call for each sheet  
    */  
        UF_CALL( UF_PLOT_ask_default_job_options( &job_options) );  
        job_options.widths = UF_PLOT_CUSTOM_3_WIDTHS;  
        job_options.use_drawing_sheet_widths = FALSE;  
        UF_CALL( UF_PLOT_ask_default_banner_options( &banner_options ) );  
        UF_CALL(UF_PLOT_ask_session_custom_widths(&session_original_custom_widths));  
        UF_CALL(UF_PLOT_ask_session_custom_widths(&sess_custom_widths));  
        copies = 1;  
        drawing_sheet = NULL_TAG;  
        while ((UF_CALL(UF_OBJ_cycle_objs_in_part(display_part,  
                                                  UF_drawing_type,  
                                                  &drawing_sheet)) == ERROR_OK) &&  
               (drawing_sheet != NULL_TAG))  
        {  
            /*  Supplying a drawing sheet tag causes the default job  
                name to have the sheet name appended to it  
            */  
            UF_CALL( UF_PLOT_ask_default_job_name( drawing_sheet, &job_name ) );  
        sess_custom_widths.use = UF_CGM_width_by_width;  
        sess_custom_widths.single = UF_CGM_width_custom;  
        sess_custom_widths.custom[UF_OBJ_WIDTH_NORMAL].width = 0.35;  
        sess_custom_widths.custom[UF_OBJ_WIDTH_THICK].width = 5.65;  
        sess_custom_widths.custom[UF_OBJ_WIDTH_THIN].width = 0.05;  
        UF_CALL(UF_PLOT_set_session_custom_widths(&sess_custom_widths));  
            /*  Print the sheet. The "gtachpmx" printer MUST be  
                defined in the default Printer group for your system.  
                The "<System Profile>" profile is the default SDI  
                profile. You can alter these two parameters to match  
                your installation.  
            */  
            UF_CALL( UF_PLOT_print( drawing_sheet, &job_options, job_name,  
                                    &banner_options, "gtachpmx",  
                                    "<System Profile>", copies ) );  
            UF_CALL(UF_PLOT_set_session_custom_widths(&session_original_custom_widths));  
            UF_free( job_name );  
        }  
        /* Terminate the API environment 里海译:终止API环境 */  
        UF_CALL(UF_terminate());  
    }  
    /*****************************************************************************  
    **  Utilities  
    *****************************************************************************/  
    /* Unload Handler  
    **     This function specifies when to unload your application from Unigraphics.  
    **     If your application registers a callback (from a MenuScript item or a  
    **     User Defined Object for example), this function MUST return  
    **     "UF_UNLOAD_UG_TERMINATE". */  
    extern int ufusr_ask_unload( void )  
    {  
        return( UF_UNLOAD_IMMEDIATELY );  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发示例，其主要功能是打印当前打开部件中的所有图纸，并设置打印线的宽度。
>
> 主要步骤包括：
>
> 1. 初始化API环境
> 2. 获取当前打开的部件
> 3. 循环遍历部件中的所有图纸
> 4. 获取默认打印作业参数和图框参数
> 5. 设置打印线的宽度
> 6. 调用打印函数打印图纸
> 7. 恢复打印线的宽度
> 8. 释放作业名内存
> 9. 终止API环境
>
> 代码中定义了错误报告函数report_error，用于输出错误信息。在打印前，先获取默认的作业参数和图框参数，然后设置打印线的宽度，接着打印图纸，打印后恢复线宽。在打印时指定了打印机名称和系统配置文件。
>
> 此代码示例展示了NX二次开发中打印图纸的基本流程，并提供了设置打印参数的示例。
>
