### 【0157】ask blend parameters 请求混合参数

#### 代码

```cpp
    /*HEAD ASK_BLEND_PARMS CCC UFUN */  
    #include <uf_exit.h>  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_ui.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：注意：UF_print_syslog 是 V18 新增的功能，请仅提供翻译，不要添加其他无关内容。 */  
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
        tag_t   feat;  
        char    *radius;  
        feat = select_a_feature( "Select feature");  
        UF_CALL( UF_MODL_ask_blend_parms( feat, 1, &radius));  
        printf("Radius: %s\n", radius);  
        UF_free( radius);  
    }  
    /* 里海 */  
    extern void ufusr(char *param, int *retcode, int paramLen)  
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

> 这段代码是NX的二次开发代码，主要功能是获取特征的混合参数。具体介绍如下：
>
> 1. 包含了NX二次开发所需的相关头文件。
> 2. 定义了宏ECHO和UF_CALL，用于输出变量值和调用NX函数，并在函数执行出错时输出错误信息。
> 3. 定义了report_error函数，用于在NX函数调用出错时输出错误信息。
> 4. 定义了select_a_feature函数，用于提示用户选择一个特征，并返回特征标签。
> 5. 定义了do_it函数，用于获取用户选择的特征混合参数，并输出半径值。
> 6. 定义了ufusr函数，作为NX的二次开发主函数，初始化NX后调用do_it函数，执行获取特征混合参数的操作，并在结束时终止NX。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发模块在卸载时不需要等待。
>
> 总的来说，这段代码实现了获取特征混合参数的功能，通过交互式选择特征，获取并输出特征的混合参数半径值。
>
