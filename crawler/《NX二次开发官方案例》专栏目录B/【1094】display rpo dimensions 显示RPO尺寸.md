### 【1094】display rpo dimensions 显示RPO尺寸

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat = NULL_TAG;  
        while ((feat = select_a_feature("Display all RPO dimensions")) != NULL_TAG)  
        {  
            UF_DISP_refresh();  
            UF_CALL(UF_DISP_display_rpo_dimensions(feat, -1, NULL,  
                UF_DISP_VIEW_OF_LAST_CURSOR, UF_DISP_USE_SYSTEM_COLOR, 0));  
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

> 这段代码是一段NX二次开发代码，主要功能是选择一个特征，然后显示其RPO尺寸。下面是代码的详细解释：
>
> 1. 包含了必要的NX头文件，如uf.h、uf_disp.h等。
> 2. 定义了一个ECHO宏，用于在NX日志窗口打印信息。
> 3. 定义了一个UF_CALL宏，用于执行NX函数，并在失败时打印错误信息。
> 4. select_a_feature函数用于选择一个特征。它会提示用户选择一个特征，并返回第一个被选中的特征。
> 5. do_it函数用于循环选择特征，并显示其RPO尺寸。
> 6. ufusr函数是NX二次开发的主函数。它会初始化NX，执行do_it函数，然后终止NX。
> 7. ufusr_ask_unload函数用于卸载二次开发应用程序。
> 8. 整体来看，这段代码实现了一个简单的功能：选择一个特征，然后显示其RPO尺寸。代码结构清晰，通过宏和函数封装，实现了错误处理和功能解耦。
>
