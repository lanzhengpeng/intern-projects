### 【2482】report sphere diameter using ask sphere parms 报告使用询问球参数功能的球直径

#### 代码

```cpp
    /*HEAD REPORT_SPHERE_DIAMETER_USING_ASK_SPHERE_PARMS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能。 */  
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
    /* qq3123197280 */  
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
    /* qq3123197280 */  
    static void do_it(void)  
    {  
        tag_t  
            sphere_feat = NULL_TAG;  
        char  
            *diameter = NULL;  
        if( (sphere_feat = select_a_feature("Select a Sphere:")) != NULL_TAG )  
        {  
            UF_CALL( UF_MODL_ask_sphere_parms ( sphere_feat, 0, &diameter ));  
            WRITE( "Sphere diameter: " );  
            WRITE( diameter );  
            WRITE( "\n" );  
            if( diameter != NULL ) UF_free( diameter );  
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

> 这段代码是NX的二次开发代码，其主要功能是选择一个球特征并输出其直径。
>
> 主要步骤如下：
>
> 1. 包含所需的NX头文件。
> 2. 定义宏REPORT_ERROR和WRITE，用于报告错误和输出信息。
> 3. 定义函数select_a_feature，用于让用户选择一个特征，并返回特征tag。
> 4. 定义函数do_it，用于获取选择的球特征，并输出其直径。
> 5. 定义ufusr函数，用于初始化NX，调用do_it，然后终止NX。
> 6. 定义ufusr_ask_unload函数，用于立即卸载UF。
>
> 代码的核心逻辑是，在ufusr函数中调用do_it，在do_it中调用select_a_feature获取用户选择的球特征，然后调用UF_MODL_ask_sphere_parms获取球特征的直径参数，并输出。
>
