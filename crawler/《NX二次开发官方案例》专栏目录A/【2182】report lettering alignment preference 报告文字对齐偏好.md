### 【2182】report lettering alignment preference 报告文字对齐偏好

#### 代码

```cpp
    /*HEAD REPORT_LETTERING_ALIGNMENT_PREFERENCE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18中是新增的。 */  
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
        char  
            buf[132] = { "" };  
        UF_DRF_lettering_preferences_t   
            lettering_preferences;  
        UF_UI_open_listing_window();  
        UF_DRF_ask_lettering_preferences ( &lettering_preferences );  
         /**  
         report current alignment setting  
         **/  
        UF_UI_write_listing_window( "Current Alignment Setting:\n" );   
        switch( lettering_preferences.align_position )  
        {  
            case UF_DRF_ALIGN_TOP_LEFT:  
                sprintf( buf, "UF_DRF_ALIGN_TOP_LEFT \n");  
                break;  
            case UF_DRF_ALIGN_TOP_CENTER:  
                sprintf( buf, " UF_DRF_ALIGN_TOP_CENTER\n");  
                break;  
            case UF_DRF_ALIGN_TOP_RIGHT:  
                sprintf( buf, "UF_DRF_ALIGN_TOP_RIGHT \n");  
                break;  
            case UF_DRF_ALIGN_MID_LEFT:  
                sprintf( buf, "UF_DRF_ALIGN_MID_LEFT \n");  
                break;  
            case UF_DRF_ALIGN_MID_CENTER:  
                sprintf( buf, "UF_DRF_ALIGN_MID_CENTER \n");  
                break;  
            case UF_DRF_ALIGN_MID_RIGHT:  
                sprintf( buf, "UF_DRF_ALIGN_MID_RIGHT \n");  
                break;  
            case UF_DRF_ALIGN_BOTTOM_LEFT:  
                sprintf( buf, "UF_DRF_ALIGN_BOTTOM_LEFT \n");  
                break;  
            case UF_DRF_ALIGN_BOTTOM_CENTER:  
                sprintf( buf, "UF_DRF_ALIGN_BOTTOM_CENTER \n");  
                break;  
            case UF_DRF_ALIGN_BOTTOM_RIGHT:  
                sprintf( buf, "UF_DRF_ALIGN_BOTTOM_RIGHT \n");  
                break;  
            default :  
                sprintf( buf, "No Alignment Preference Set.\n");  
        }  
        UF_UI_write_listing_window( buf );  
        return;  
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

> 这段代码是一个NX Open的二次开发示例，其主要功能是获取和报告NX的标注对齐偏好设置。
>
> 主要特点如下：
>
> 1. 错误处理函数：定义了一个错误处理函数report_error，用于在UF函数调用失败时报告错误信息。
> 2. 标注对齐偏好：使用UF_DRF_ask_lettering_preferences函数获取当前的标注对齐偏好设置，并保存在lettering_preferences结构体中。
> 3. 报告对齐设置：通过switch语句判断lettering_preferences.align_position的值，并报告当前的对齐设置。
> 4. 主函数：ufusr函数中初始化NX Open，调用do_it函数获取和报告对齐设置，然后终止NX Open。
> 5. 卸载函数：ufusr_ask_unload函数用于设置立即卸载二次开发库。
>
> 总体来说，这段代码简单实现了获取和报告NX的标注对齐偏好设置的功能，并包含了错误处理、初始化和卸载等标准二次开发流程。
>
