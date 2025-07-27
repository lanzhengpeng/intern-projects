### 【1564】localize color names in color table 本地化颜色表中的颜色名称

#### 代码

```cpp
    /*HEAD LOCALIZE_COLOR_NAMES_IN_COLOR_TABLE CCC UFUN */  
    /*  
        This program can be used to translate English color names in the color  
        table of the displayed part to the local language as defined by  
        UGII_LANG.  
        It will not translate non-English color names.  For example, if the  
        names have previously been converted to German, they cannot be  
        translated directly to French.  If necessary, import an english CDF  
        file first.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_undo.h>  
    #include <uf_error_bases.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        int  
            cnt,  
            ii;  
        double  
            clr_values[3];  
        char  
            *color_name,  
            msg[MAX_LINE_SIZE+1];  
        set_undo_mark("Translate color table");  
        UF_CALL(UF_DISP_ask_color_count(&cnt));  
        for (ii = UF_OBJ_MIN_COLOR; ii < cnt; ii++)  
        {  
            UF_CALL(UF_DISP_ask_color(ii, UF_DISP_rgb_model, &color_name,  
                clr_values));  
            if (strlen(color_name) > UF_DISP_MAX_NAME_SIZE)  
            {  
                sprintf(msg, "color # %d name \"%s\" shortened to ",  
                    ii, color_name);  
                strcpy(&color_name[UF_DISP_MAX_NAME_SIZE-1], ".");  
                strcat(msg, color_name);  
                strcat(msg, "\n");  
                ECHO(msg);  
            }  
            UF_CALL(UF_DISP_set_color(ii, UF_DISP_rgb_model, color_name,  
                clr_values));  
            UF_free(color_name);  
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

> 这段代码用于在NX中本地化颜色表中的颜色名称。具体来说：
>
> 1. 首先，代码包含必要的头文件，用于NX的二次开发。
> 2. 定义了一些宏，用于打印错误信息和日志。
> 3. 实现了一个错误报告函数report_error，用于在函数调用出错时打印错误信息。
> 4. 实现了一个设置撤销标记的函数set_undo_mark。
> 5. 实现了主函数do_it，用于遍历颜色表，获取每个颜色的名称，如果名称超过最大长度，则进行截断，然后重新设置颜色名称。
> 6. 实现了ufusr函数，用于初始化NX，调用do_it函数，然后终止NX。
> 7. 实现了ufusr_ask_unload函数，用于卸载用户函数。
>
> 总的来说，这段代码通过遍历颜色表，将颜色名称本地化，实现了颜色名称的本地化显示，同时处理了名称过长的问题。
>
