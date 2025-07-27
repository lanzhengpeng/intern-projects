### 【1876】rename sketches 重命名草图

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
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
    static tag_t select_a_sketch(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            sketch;  
        if (!UF_CALL(UF_UI_select_sketch(prompt, NULL, &sketch, &resp))  
        && (resp == UF_UI_OK))  
           return sketch;  
        else  
            return NULL_TAG;  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat,  
            sketch;  
        char  
            theName[MAX_ENTITY_NAME_SIZE+1];  
        while ((sketch = select_a_sketch("Rename")) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(sketch, theName));  
            if (prompt_for_text("New name", theName))  
            {  
                UF_CALL(UF_OBJ_set_name(sketch, theName));  
            //  To update Part Navigator, the feature must be modified too  
                UF_CALL(UF_MODL_ask_object_feat(sketch, &feat));  
                UF_CALL(UF_OBJ_set_name(feat, "XXX"));  
                UF_CALL(UF_OBJ_delete_name(feat));  
            }  
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

> 这是段NX的二次开发代码，主要实现以下功能：
>
> 1. 错误报告：定义了错误报告函数report_error，用于在调用NX API出错时输出错误信息，包括文件名、行号、错误码和错误消息。
> 2. 提示选择草图：定义了选择草图的函数select_a_sketch，通过UI交互提示用户选择一个草图。
> 3. 提示输入文本：定义了提示输入文本的函数prompt_for_text，用于提示用户输入文本。
> 4. 重命名功能：在do_it函数中，通过循环调用select_a_sketch函数选择草图，然后调用NX API获取草图名称，提示用户输入新名称，并调用NX API重命名草图和其对应的特征。
> 5. 初始化和终止：在ufusr函数中，先调用NX API进行初始化，然后调用do_it执行重命名功能，最后进行终止。
> 6. 卸载提示：定义了卸载提示函数ufusr_ask_unload，用于提示立即卸载该程序。
>
> 整体来看，这段代码通过交互式方式实现了NX草图的重命名功能，并进行了错误处理和卸载提示。
>
