### 【2026】report color number of name 报告颜色名称对应的颜色编号

#### 代码

```cpp
    /*HEAD REPORT_COLOR_NUMBER_OF_NAME CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，UF_print_syslog 在 V18 中是新增的。

翻译：UF_print_syslog 是在 V18 中新增的。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /*  Prior to NX 4.0, the returned full color name looked like this:  
        full_color_name = "2 (Green)"  
        So the method shown here cleans it up and returns just the name.  
    */  
    static void ask_color_name(int color_num, char *color_name)  
    {  
        double  
            clr_values[3];  
        char  
            *full_color_name;  
        UF_CALL(UF_DISP_ask_color(color_num, UF_DISP_rgb_model, &full_color_name,  
            clr_values));  
        if (strchr(full_color_name, '('))  
        {  
            strcpy(color_name, strchr(full_color_name, '(')+1);  
            strcpy(strchr(color_name, ')'), "");  
        }  
        else  
            strcpy(color_name, full_color_name);  
        UF_free(full_color_name);  
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
        int  
            cnt,  
            col_no = UF_OBJ_MIN_COLOR;  
        char  
            color[MAX_ENTITY_NAME_SIZE+1],  
            colorq[MAX_ENTITY_NAME_SIZE+1] = {"Aquamarine"},  
            msg[MAX_LINE_SIZE+1];  
        logical  
            found=FALSE;  
        UF_CALL(UF_DISP_ask_color_count(&cnt));  
        if (prompt_for_text("What color would you like?", colorq))  
        {  
            for (col_no = 0; col_no < cnt; col_no++)  
            {  
                ask_color_name(col_no, color);  
                if (strstr(color, colorq))  
                {  
                    if (!strcmp(color, colorq)) found = TRUE;  
                    WRITE_D(col_no);  
                    WRITE_S(color);  
                }  
            }  
        }  
        if (found)  
        {  
            sprintf(msg, "Selected color, %s, associates to %d, in the color table",  
                 colorq, col_no);  
            uc1601(msg, TRUE);  
        }  
        else  
        {  
            for (col_no = 0; col_no < cnt; col_no++)  
            {  
                    ask_color_name(col_no, color);  
                    WRITE_D(col_no);  
                    WRITE_S(color);  
            }  
            sprintf(msg, "Selected color, %s, is not found in the color table",  
                colorq );  
            uc1601(msg, TRUE);  
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

> 这段代码是NX的二次开发示例，其主要功能是查询NX的颜色表，并根据用户输入的颜色名称找到对应的颜色编号。
>
> 主要步骤如下：
>
> 1. 包含NX的二次开发头文件。
> 2. 定义一个错误报告函数，用于在函数调用失败时输出错误信息。
> 3. 定义一个写字符串到列表窗口的函数。
> 4. 定义一个写整数到列表窗口的函数。
> 5. 定义一个查询颜色名称的函数，它会根据颜色编号获取颜色的全名，然后提取颜色名称。
> 6. 定义一个提示输入文本的函数。
> 7. 定义一个主函数do_it，用于实现主要逻辑。
> 8. 查询颜色表中的颜色数量。
> 9. 提示用户输入要查询的颜色名称。
> 10. 遍历颜色表，找到与用户输入匹配的颜色名称，并输出对应的颜色编号。
> 11. 如果找到匹配的颜色，提示用户已找到。
> 12. 如果未找到，提示用户该颜色不在颜色表中，并输出所有颜色编号和名称。
> 13. 在ufusr函数中调用主函数。
> 14. 定义卸载函数，返回立即卸载。
>
> 通过该代码，用户可以根据颜色名称查询到对应的颜色编号，实现了颜色表查询的功能。代码结构清晰，功能明确，是一个典型的NX二次开发示例。
>
