### 【0926】create standalone idsymbols 创建独立的标识符号

#### 代码

```cpp
    /*HEAD CREATE_STANDALONE_IDSYMBOLS CCC UFUN */  
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

注意：UF_print_syslog 是 V18 中的新功能，请只提供翻译，不要添加任何废话。 */  
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
            pos[3];  
        char  
            up_text[MAX_ID_SYM_TEXT_LENGTH+1];  
        while (specify_position("Indicate position for ID symbol", pos))  
        {  
            sprintf(up_text, "%d", ++ii);  
            UF_CALL(UF_DRF_create_id_symbol(UF_DRF_sym_circle, up_text, NULL, pos,  
                UF_DRF_without_leader, UF_DRF_leader_attach_screen, NULL, pos,  
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

> 这是一段NX的二次开发代码，主要实现了以下功能：
>
> 1. 包含了必要的头文件，如stdio.h、string.h、uf.h、uf_ui.h、uf_drf.h等，用于支持代码中的函数调用。
> 2. 定义了宏UF_CALL，用于检测UF函数的返回值，如果返回值不为0，则打印错误信息。
> 3. 定义了report_error函数，用于打印UF函数调用失败的错误信息。
> 4. 定义了specify_position函数，用于获取用户指定的ID符号位置。
> 5. 定义了do_it函数，用于在用户指定的位置创建圆形ID符号。函数使用循环调用specify_position获取位置，然后调用UF_DRF_create_id_symbol函数创建ID符号。
> 6. 定义了ufusr函数，用于初始化UF库，调用do_it函数，然后终止UF库。
> 7. 定义了ufusr_ask_unload函数，用于在NX关闭时卸载该代码。
> 8. 代码整体实现了在NX中创建圆形ID符号的功能，用户可以指定位置多次创建，直到取消指定为止。
>
