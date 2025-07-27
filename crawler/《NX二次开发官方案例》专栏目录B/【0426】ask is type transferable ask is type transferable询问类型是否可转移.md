### 【0426】ask is type transferable ask is type transferable询问类型是否可转移

#### 代码

```cpp
    /*HEAD ASK_IS_TYPE_TRANSFERABLE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 中新增的函数。

翻译：
注意：UF_print_syslog 是 V18 新增的函数，请只回答译文，不要添加其他内容。 */  
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
    static int prompt_for_an_integer(char *prompt, char *item, int number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            return da[0];  
        }  
        else return 0;  
    }  
    static void do_it(void)  
    {  
        logical  
            flag;  
        int  
            n = 0;  
        char  
            msg[50];  
        while ((n = prompt_for_an_integer("Enter object type", "Type", n)) != 0)  
        {  
            UF_CALL(UF_OBJ_is_type_transferable(n, &flag));  
            if (flag)  
                sprintf(msg, "type %d IS transferable", n);  
            else  
                sprintf(msg, "type %d is NOT transferable", n);  
            uc1601(msg, TRUE);  
        }  
    }  
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

> 这段代码是NX的二次开发示例，其主要功能是判断NX对象类型是否可以传输。以下是代码的主要组成部分：
>
> 1. 包含的头文件：包含了NX二次开发需要用到的头文件，如uf.h、uf_ui.h、uf_obj.h等。
> 2. UF_CALL宏：这是一个宏，用于调用NX的UF函数，并在调用失败时打印错误信息。
> 3. report_error函数：用于报告错误，当UF函数调用失败时调用。它会获取错误码，并打印错误信息。
> 4. prompt_for_an_integer函数：用于提示用户输入一个整数，并返回用户输入的整数。
> 5. do_it函数：这是主逻辑函数。它会无限循环提示用户输入对象类型，然后调用UF_OBJ_is_type_transferable函数判断该类型是否可以传输，最后打印结果。
> 6. ufusr函数：这是NX二次开发的入口函数。它初始化NX环境，调用do_it函数，然后终止NX环境。
> 7. ufusr_ask_unload函数：用于卸载二次开发代码，这里设置为立即卸载。
>
> 总的来说，这段代码提供了一个简单的交互式NX二次开发示例，用于判断对象类型是否可以传输，并通过交互式输入进行测试。
>
