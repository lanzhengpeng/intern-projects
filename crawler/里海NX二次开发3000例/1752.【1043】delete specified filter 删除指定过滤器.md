### 【1043】delete specified filter 删除指定过滤器

#### 代码

```cpp
    /*HEAD DELETE_SPECIFIED_FILTER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新功能。因此，回答只包含译文，不包含其他无关内容。

UF_print_syslog是V18版本中的新功能。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_filter(tag_t part, tag_t filter)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_filter_type, &filter));  
        return (filter);  
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
        logical  
            done = FALSE;  
        int  
            cnt;  
        tag_t  
            filter,  
            part = UF_PART_ask_display_part();  
        char  
            filter_name[UF_OBJ_NAME_LEN + 1] = { "" };  
        do  
        {  
            WRITE("filters in part\n");  
            filter = NULL_TAG;  
            cnt = 0;  
            while ((filter = ask_next_filter(part, filter)) != NULL_TAG)  
            {  
                cnt++;  
                WRITE_D(filter);  
                UF_CALL(UF_OBJ_ask_name(filter, filter_name));  
                WRITE_S(filter_name);  
            }  
            if ((cnt > 0) && prompt_for_text("Delete filter", filter_name))  
            {  
                UF_UI_close_listing_window();  
                UF_CALL(UF_OBJ_cycle_by_name_and_type(part, filter_name,  
                    UF_filter_type, FALSE, &filter));  
                WRITE_D(filter);  
                if (filter != NULL_TAG)  
                {  
                    WRITE("deleting - ");  
                    WRITE_S(filter_name);  
                    UF_CALL(UF_OBJ_delete_object(filter));  
                }  
                WRITE("\n");  
            }  
            else done = TRUE;  
        } while (!done);  
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

> 这段代码是一个NX的二次开发代码示例，主要实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于在出错时打印错误信息。
> 2. 定义了WRITE系列宏，用于向列表窗口打印文本信息。
> 3. 实现了ask_next_filter函数，用于遍历部件中的过滤器对象。
> 4. 实现了prompt_for_text函数，用于提示用户输入文本并返回是否成功。
> 5. 实现了do_it函数，用于遍历部件中的过滤器对象，打印过滤器名称，提示用户输入要删除的过滤器名称，并删除指定的过滤器对象。
> 6. 实现了ufusr函数，用于初始化NX环境，调用do_it函数执行主逻辑，并终止NX环境。
> 7. 实现了ufusr_ask_unload函数，用于在卸载用户函数时立即卸载。
>
> 总的来说，这段代码实现了通过用户输入删除指定过滤器的功能。
>
