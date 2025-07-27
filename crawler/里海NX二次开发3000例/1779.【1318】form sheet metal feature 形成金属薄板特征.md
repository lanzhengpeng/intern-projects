### 【1318】form sheet metal feature 形成金属薄板特征

#### 代码

```cpp
    /*HEAD FORM_SHEET_METAL_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_modl_smd.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数，用于打印系统日志。该函数将日志信息输出到系统日志文件中，确保了日志信息可以持久保存，方便后续查阅和分析。 */  
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
        logical   
            is_update_required;   
        int  
            error_code = 0;  
        char  
            *type;    
        tag_t  
    /* Work around for PR 4335472 UF_MODL_form_feature memory access violation 里海译:对PR 4335472 UF_MODL_form_feature内存访问违规的解决方法 */      
            *feature_tag = UF_allocate_memory (sizeof (tag_t), &error_code);  
        while ((feature_tag[0] = select_a_feature("Select a Sheet Metal Feature"))   
                != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_feat_type(feature_tag[0], &type));  
            if (!strcmp(type, "FLANGE") ||  
                !strcmp(type, "INSET_FLANGE") ||  
                !strcmp(type, "GENERAL_FLANGE") ||  
                !strcmp(type, "SMBRIDGE") ||  
                !strcmp(type, "SMBEND"))  
            {  
                UF_CALL(UF_MODL_form_features( 1, feature_tag,   
                    &is_update_required));  
                if (is_update_required)  
                    UF_CALL(UF_MODL_update());              
            }   
            UF_free(type);  
        }  
    /*  Work around for PR 4335472 UF_MODL_form_feature  memory access violation 里海译:针对PR 4335472 UF_MODL_form_feature内存访问违规的解决方案 */      
        UF_free(feature_tag);  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 初始化NX环境，并定义了错误处理函数report_error，用于打印错误信息到系统日志和列表窗口。
> 2. 定义了选择特征的函数select_a_feature，通过弹窗提示用户选择一个钣金特征。
> 3. 主函数do_it中，循环调用select_a_feature函数，获取用户选择的钣金特征，然后判断特征类型，如果是折弯、凸缘等类型，则调用UF_MODL_form_features函数进行特征成形，并更新模型。
> 4. 在主函数ufusr中，初始化NX环境，调用do_it函数，然后终止NX环境。
> 5. 定义了卸载函数ufusr_ask_unload，返回立即卸载标志。
> 6. 使用了NX提供的API函数，如UF_MODL_ask_feat_type、UF_MODL_form_features等，来实现钣金特征的查询和成形等操作。
> 7. 代码结构清晰，注释详细，便于理解。
>
