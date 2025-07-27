### 【1767】print ug image window 打印UG图像窗口

#### 代码

```cpp
    /*HEAD PRINT_UG_IMAGE_WINDOW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
    #include <uf_drf.h>  
    #include <uf_obj.h>  
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
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    #define WRITENZ(X) if (X != 0) ECHO("%s = %d\n", #X, X);  
    static void do_it(void)  
    {  
        tag_t  
            note;  
        double  
            center[3],  
            scale;  
        char  
            *text_string[1] = { "UF_DISP_print_window_ug_image" };  
        WRITENZ(uc6430("", center, &scale));  
        UF_CALL(UF_DRF_create_note(1, text_string, center, 0, &note));  
        UF_CALL(UF_DISP_print_window_ug_image(0, 3));  
        UF_CALL(UF_OBJ_delete_object(note));  
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

> 这段代码是一个NX Open C++的二次开发示例，主要用于演示如何在NX中显示临时文本和创建注释。
>
> 主要功能包括：
>
> 1. 包含必要的NX Open C++头文件，如uf.h、uf_ui.h等。
> 2. 定义了一个ECHO宏，用于在日志窗口中打印信息。
> 3. 定义了一个report_error函数，用于在调用NX API失败时打印错误信息。
> 4. 定义了一个display_temporary_text函数，用于在指定位置显示临时文本。
> 5. 定义了一个WRITENZ宏，用于打印变量值。
> 6. 定义了一个do_it函数，用于执行以下操作：
>
> 1. 定义了ufusr函数，用于初始化NX，执行do_it函数，然后终止NX。
> 2. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 该代码演示了在NX中显示文本和创建注释的基本流程，对于学习NX Open C++开发非常有帮助。
>
