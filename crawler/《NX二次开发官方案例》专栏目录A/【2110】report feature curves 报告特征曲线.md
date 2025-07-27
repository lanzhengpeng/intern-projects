### 【2110】report feature curves 报告特征曲线

#### 代码

```cpp
    /*HEAD REPORT_FEATURE_CURVES CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，翻译为：

注意：UF_print_syslog 是 V18 新增的，只需回答翻译，不要写无关的内容。 */  
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
        UF_UI_feat_sel_type_t  
            add_booleans = UF_UI_FEAT_SEL_TYPE_BROWSEABLE;  
        UF_CALL(UF_UI_select_feature(prompt, &add_booleans, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void report_feat_name_and_type( tag_t feat)  
    {  
        char  
            msg[MAX_LINE_SIZE+1],  
            *name,  
            *type;  
        if (!UF_CALL(UF_MODL_ask_feat_name(feat, &name)) &&  
            !UF_CALL(UF_MODL_ask_feat_type(feat, &type)))  
        {  
            UF_CALL(UF_UI_open_listing_window());  
            sprintf(msg, "%d %s (%s)\n", feat, name, type);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            UF_free(name);  
            UF_free(type);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            *curves,  
            feat;  
        int  
            ii = 0,  
            n = 0;  
        char  
            msg[133];  
        while ((feat = select_a_feature("Select feature")) != NULL_TAG)  
        {  
            report_feat_name_and_type( feat);  
            UF_CALL(UF_CURVE_ask_feature_curves(feat, &n, &curves));  
            if( n > 0)  
            {  
                sprintf(msg, "%d Curves of feature:\n", n);  
                UF_CALL(UF_UI_write_listing_window(msg));  
            }  
            for (ii = 0; ii < n; ii++)   
            {  
                sprintf( msg, "%d. Tag: %d\n", ii, curves[ii]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
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

> 这段代码是一个NX的二次开发示例，其主要功能是选择特征并报告特征曲线的信息。
>
> 代码首先包含必要的NX头文件，并定义了错误报告函数report_error，用于在发生错误时打印错误信息。
>
> 接着，定义了选择特征的函数select_a_feature，它使用UF_UI_select_feature函数让用户选择一个特征，并返回特征标签。
>
> 然后，定义了报告特征名称和类型的函数report_feat_name_and_type，它使用UF_MODL_ask_feat_name和UF_MODL_ask_feat_type函数获取特征名称和类型，并在列表窗口中显示。
>
> 主体函数do_it使用一个循环，不断让用户选择特征，然后报告特征名称、类型，并调用UF_CURVE_ask_feature_curves函数获取特征曲线，最后在列表窗口中显示曲线的标签。
>
> 入口函数ufusr初始化NX，调用do_it函数，然后终止NX。
>
> 卸载函数ufusr_ask_unload立即卸载二次开发库。
>
> 总的来说，这段代码实现了选择特征并报告特征曲线的功能，使用了NX提供的UI、模型和曲线相关的UF函数。
>
