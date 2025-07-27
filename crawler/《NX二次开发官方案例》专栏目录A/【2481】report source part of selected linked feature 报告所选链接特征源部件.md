### 【2481】report source part of selected linked feature 报告所选链接特征源部件

#### 代码

```cpp
    /*HEAD REPORT_SOURCE_PART_OF_SELECTED_LINKED_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_wave.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog 是 V18 版本新增的功能。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    /* qq3123197280 */  
    static void do_it(void)  
    {  
        tag_t  
            feat,  
            part_tag,  
            source_tag;  
        char  
            *feat_name,  
            *feat_type;  
        char  
            owning_part[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        while ((feat = select_a_feature("Select a LINKED feature")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_feat_name(feat, &feat_name));  
            WRITE_S(feat_name);  
            UF_free(feat_name);  
            UF_CALL(UF_MODL_ask_feat_type(feat, &feat_type));  
            WRITE_S(feat_type);  
            UF_free(feat_type);  
            if (!UF_CALL(UF_WAVE_ask_link_source(feat, TRUE, &source_tag)))  
            {  
                UF_CALL(UF_OBJ_ask_owning_part(source_tag, &part_tag ));  
                UF_PART_ask_part_name(part_tag, owning_part );  
                WRITE_S(owning_part);  
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

> 这段代码是一个NX Open的UF程序，用于在NX中实现选择链接特征并获取其源特征所在组件名称的功能。
>
> 主要功能包括：
>
> 1. 定义了一个报告错误函数report_error，用于在函数调用出错时打印错误信息。
> 2. 定义了一个向列表窗口写入字符串的函数write_string_to_listing_window。
> 3. 定义了一个选择链接特征的函数select_a_feature，用户可以输入提示选择一个链接特征。
> 4. 在主函数do_it中，通过循环调用select_a_feature选择链接特征，并获取特征名称、类型和源特征所在的组件名称，然后打印到列表窗口。
> 5. 定义了UFusr入口函数，初始化后调用do_it，最后终止。
> 6. 定义了卸载函数，返回立即卸载。
>
> 总的来说，这段代码实现了在NX中选择链接特征并获取其源特征所在组件名称的功能。通过列表窗口交互选择特征，并打印出特征的相关信息，方便用户进行查看。
>
