### 【0514】compute and report model bounds 计算并报告模型边界

#### 代码

```cpp
    /*HEAD COMPUTE_AND_REPORT_MODEL_BOUNDS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的函数。

仅提供翻译，不添加任何其他内容。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void do_it(void)  
    {  
        logical   
            bounds_computed;  
        double   
            model_bounds[6];  
        char  
            msg[132] = { "" };  
        UF_CALL( UF_DISP_compute_model_bounds( &bounds_computed, model_bounds ));  
        if( bounds_computed == FALSE )  
        {  
            WRITE("Model bounds could not be computed.\n");  
            return;  
        }  
        sprintf( msg, "Minimum XYZ: %f  %f  %f\n",   
                 model_bounds[0], model_bounds[2], model_bounds[4]);  
        WRITE(msg);  
        sprintf( msg, "Maximum XYZ: %f  %f  %f\n",   
                 model_bounds[1], model_bounds[3], model_bounds[5]);  
        WRITE(msg);  
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

> 这段NX二次开发代码的主要功能是计算并报告模型的边界，具体步骤如下：
>
> 1. 首先定义了报告错误函数report_error，用于输出函数调用失败时的错误信息。
> 2. 定义了WRITE宏，用于打开窗口并写入信息。
> 3. 定义了do_it函数，该函数调用UF_DISP_compute_model_bounds计算模型的边界，并将最小和最大XYZ坐标输出到窗口。
> 4. 在ufusr函数中，先调用UF_initialize初始化，然后调用do_it函数计算边界，最后调用UF_terminate终止。
> 5. ufusr_ask_unload函数返回UF_UNLOAD_IMMEDIATELY，表示用户退出时立即卸载二次开发代码。
>
> 通过以上步骤，该代码实现了在NX中计算并报告模型边界的功能。
>
