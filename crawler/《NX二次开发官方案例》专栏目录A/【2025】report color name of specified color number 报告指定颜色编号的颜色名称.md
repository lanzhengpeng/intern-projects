### 【2025】report color name of specified color number 报告指定颜色编号的颜色名称

#### 代码

```cpp
    /*HEAD REPORT_COLOR_NAME_OF_SPECIFIED_COLOR_NUMBER CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18版本新增的。 */  
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
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
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
    static void do_it(void)  
    {  
        int  
            clr_num = UF_OBJ_MIN_COLOR;  
        char  
            color[MAX_ENTITY_NAME_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        while (prompt_for_an_integer("Report color name", "color number", &clr_num))  
        {  
            if ((clr_num >= UF_OBJ_MIN_COLOR) && (clr_num <= UF_OBJ_MAX_COLOR))  
            {  
                ask_color_name(clr_num, color);  
                WRITE_S(color);  
                WRITE("\n");  
            }  
            else  
            {  
                sprintf(msg, "Valid color numbers are %d to %d\n",  
                    UF_OBJ_MIN_COLOR, UF_OBJ_MAX_COLOR);  
                uc1601(msg, TRUE);  
                clr_num = UF_OBJ_MIN_COLOR;  
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

> 这段代码是NX的二次开发代码，主要功能是根据用户输入的颜色编号，查询并打印对应颜色的名称。
>
> 主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印错误信息。
> 2. 定义了一个函数write_string_to_listing_window，用于将字符串写入列表窗口。
> 3. 定义了一个函数prompt_for_an_integer，用于提示用户输入一个整数。
> 4. 定义了一个函数ask_color_name，用于根据颜色编号查询颜色的名称。
> 5. 定义了一个主函数do_it，用于循环提示用户输入颜色编号，查询并打印颜色名称。
> 6. 定义了ufusr函数，用于初始化NX环境，调用do_it函数，并结束NX环境。
> 7. 定义了ufusr_ask_unload函数，用于在卸载UF时立即卸载。
>
> 整体来说，这段代码通过用户输入的颜色编号，实现了查询颜色名称并打印的功能。
>
