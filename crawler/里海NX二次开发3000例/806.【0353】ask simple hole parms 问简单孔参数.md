### 【0353】ask simple hole parms 问简单孔参数

#### 代码

```cpp
    /*HEAD ASK_SIMPLE_HOLE_PARMS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog是V18版本新增的，请只回答翻译，不要添加任何其他内容。 */  
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
        tag_t   feat_id;  
        int     edit=1; /* This is an unused flag 译:这是一个未使用的标志。 */  
        int     thru_flag;  
        char    *diam, *depth, *tip_angle;  
        feat_id = select_a_feature("Select hole feature");  
        UF_CALL( UF_MODL_ask_simple_hole_parms( feat_id, edit,  
                 &diam, &depth, &tip_angle, &thru_flag));  
        printf("Hole diameter: %s\n", diam);  
        printf("Hole depth: %s\n", depth);  
        printf("Tip angle: %s\n", tip_angle);  
        UF_free( diam);  
        UF_free( depth);  
        UF_free( tip_angle);  
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

> 这段代码是一个NX Open C++应用程序，主要用于从NX CAD模型中获取简单孔特征的参数。
>
> 主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用出错时打印错误信息。
> 2. 定义了一个选择特征的函数select_a_feature，用于提示用户选择一个简单孔特征。
> 3. 定义了一个主函数do_it，用于执行获取简单孔参数的操作。
> 4. 在主函数中，先调用select_a_feature选择一个简单孔特征，然后调用UF_MODL_ask_simple_hole_parms函数获取其直径、深度、尖角和是否贯穿的参数，并打印出来。
> 5. 定义了一个ufusr函数，用于初始化和终止NX Open，并调用do_it函数。
> 6. 定义了一个ufusr_ask_unload函数，用于卸载应用程序。
>
> 这个应用程序通过UI交互选择一个简单孔特征，然后获取并打印其参数，展示了NX Open C++应用程序的基本结构和函数调用方式。
>
