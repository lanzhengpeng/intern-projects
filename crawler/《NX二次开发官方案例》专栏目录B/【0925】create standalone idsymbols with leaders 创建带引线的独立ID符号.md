### 【0925】create standalone idsymbols with leaders 创建带引线的独立ID符号

#### 代码

```cpp
    /*HEAD CREATE_STANDALONE_IDSYMBOLS_WITH_LEADERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
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

注意：UF_print_syslog在V18版本中是新增的。请只提供译文，不要添加其他无关内容。 */  
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
    static logical specify_position(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 0;  
        tag_t  
            id_symbol;  
        double  
            org[3] = {5.25, 5.25, 0},  
            pos[3];  
        char  
            up_text[MAX_ID_SYM_TEXT_LENGTH+1];  
        while (specify_position("Indicate position for ID symbol", pos))  
        {  
            sprintf(up_text, "%d", ++ii);  
            UF_CALL(UF_DRF_create_id_symbol(UF_DRF_sym_circle, up_text, NULL, pos,  
                UF_DRF_with_leader, UF_DRF_leader_attach_screen, NULL, org,  
                &id_symbol));  
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

> 这段代码是NX二次开发中的UF函数示例，用于创建带领导线的ID符号。
>
> 主要功能包括：
>
> 1. 初始化和终止：使用UF_initialize()和UF_terminate()进行NX系统的初始化和终止。
> 2. 错误报告函数：定义了report_error()函数，用于打印UF函数调用错误信息。
> 3. 获取位置：使用specify_position()函数获取用户指定的位置。
> 4. 创建ID符号：在do_it()函数中，循环调用specify_position()获取位置，然后调用UF_DRF_create_id_symbol()函数创建圆形ID符号，并带领导线。
> 5. 主函数：定义了ufusr()函数作为主函数，初始化后调用do_it()执行创建ID符号功能，最后终止NX系统。
> 6. 卸载请求：定义了ufusr_ask_unload()函数，用于卸载请求。
>
> 综上所述，这段代码实现了在NX中创建带领导线的圆形ID符号的功能，具有完整的初始化、执行和终止流程，并提供了错误报告和卸载请求处理。
>
