### 【1640】modify lettering alignment position 修改标注对齐位置

#### 代码

```cpp
    /*HEAD MODIFY_LETTERING_ALIGNMENT_POSITION CCC UFUN */  
    /**  
    This program shows how to use UF_DRF_ask_lettering_preferences() and   
    UF_DRF_set_lettering_preferences() to change the alignment position of the lettering.  
    It reads the current setting and increments it by one, unless it is at the  
    maximum value, in which case it resets it to the minimum.  
    You can see the behavior reflected in the UG session if you are in the Drafting module,  
    by selecting Preferences->Annotation, and then hovering your cursor over the  
    Alignment Position button.  Note the setting and then run the program,  
    and note the new setting.  
    **/  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_drf.h>  
    #include <uf_ui.h>  
    #include <uf_ui_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的功能，用于打印系统日志。 */  
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
        UF_DRF_lettering_preferences_t   
            lettering_preferences;  
         /**  
         get current alignment setting  
         **/  
        UF_CALL(UF_DRF_ask_lettering_preferences ( &lettering_preferences ));  
        /** modify the setting by adding one if it is less than the max **/  
        if( lettering_preferences.align_position < UF_DRF_ALIGN_BOTTOM_RIGHT )  
        {  
            lettering_preferences.align_position += 1;  
        }  
        /** or resetting it to the lowest value if it is at the highest value **/  
        else  
        {  
            lettering_preferences.align_position = UF_DRF_ALIGN_TOP_LEFT;      
        }  
        /**   
            set the new alignment position preference.  
        **/  
        UF_CALL(UF_DRF_set_lettering_preferences ( &lettering_preferences ));  
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

> 这段代码是一个NX的二次开发示例，用于演示如何使用UF_DRF_ask_lettering_preferences和UF_DRF_set_lettering_preferences函数来修改标注的对齐位置。具体步骤如下：
>
> 1. 定义一个UF_DRF_lettering_preferences_t结构体，用于存储标注的偏好设置。
> 2. 使用UF_DRF_ask_lettering_preferences函数获取当前标注的对齐位置设置。
> 3. 根据当前设置，将其加1，除非已经是最大值，如果是则重置为最小值。
> 4. 使用UF_DRF_set_lettering_preferences函数设置新的对齐位置。
> 5. 在ufusr函数中初始化NX，调用do_it函数，然后终止NX。
> 6. 提供一个ufusr_ask_unload函数，以便在卸载用户函数时立即卸载。
>
> 这段代码通过获取和设置标注的对齐位置，实现了标注对齐位置的修改，并提供了错误处理和日志打印功能。可以作为二次开发修改标注设置的参考。
>
