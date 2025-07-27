### 【2735】set background to color number 将背景色设置为颜色编号

#### 代码

```cpp
    /*HEAD SET_BACKGROUND_TO_COLOR_NUMBER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，可以翻译为：

UF_print_syslog是在V18中新增的，请只回答翻译，不要添加其他无关内容。 */  
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
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        double  
            clr_vals[3];  
        int  
            clr_no = 7;  
        char  
            b_msg[133],  
            *color_name;  
        while (prompt_for_an_integer("Enter color number", "Background", &clr_no))  
        {  
            if ((clr_no > 0) && (clr_no <= UF_OBJ_MAX_COLOR))  
            {  
                UF_CALL(UF_DISP_ask_color(clr_no, UF_DISP_rgb_model,  
                    &color_name, clr_vals));  
                UF_CALL(UF_DISP_set_color(UF_DISP_BACKGROUND_COLOR,  
                    UF_DISP_rgb_model, "", clr_vals ));  
                UF_CALL(UF_DISP_load_color_table());  
                sprintf(b_msg,"Background set to %s (%d)\n", color_name, clr_no);  
                WRITE(b_msg);  
                UF_free(color_name);  
            }  
            else  
            {  
                sprintf(b_msg, "Valid color numbers = 0 to %d", UF_OBJ_MAX_COLOR);  
                uc1601(b_msg, TRUE);  
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

> 这段代码是一个NX的二次开发示例，主要功能是设置NX的背景色。
>
> 关键点总结如下：
>
> 1. 引入了NX提供的头文件，如uf.h、uf_ui.h等，这些头文件提供了二次开发所需的函数声明。
> 2. 定义了宏UF_CALL，用于调用NX函数，并处理可能的错误。
> 3. 实现了report_error函数，用于在出错时打印错误信息。
> 4. 定义了宏WRITE，用于在列表窗口中打印信息。
> 5. 实现了prompt_for_an_integer函数，用于提示用户输入一个整数。
> 6. 实现了do_it函数，循环提示用户输入背景色编号，如果输入有效，则设置NX的背景色。
> 7. 实现了ufusr函数，这是NX二次开发的入口函数，初始化NX环境，调用do_it函数执行功能，然后终止NX环境。
> 8. 实现了ufusr_ask_unload函数，用于卸载二次开发环境。
>
> 总的来说，这段代码通过交互式的提示用户输入，实现了设置NX背景色的功能，并包含了错误处理和用户交互界面，是一个典型的NX二次开发示例。
>
