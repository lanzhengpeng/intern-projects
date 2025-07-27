### 【2119】report feature relatives 报告特征关系

#### 代码

```cpp
    /*HEAD REPORT_FEATURE_RELATIVES CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18中是新的。 */  
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
    static void report_feature_name(tag_t feat)  
    {  
        char  
            *string,  
            msg[300] = { "" };  
        UF_CALL(UF_MODL_ask_feat_name(feat, &string));  
        sprintf(msg, "%d = %s\n", feat, string);  
        WRITE( msg );  
        UF_free(string);  
    }  
    static void report_feature_relatives(tag_t feat)  
    {  
        int  
            ii,  
            n_children,  
            n_parents;  
        tag_t  
            *children,  
            *parents;  
        char  
            msg[300] = { "" };  
        sprintf( msg, "\n");  
        WRITE( msg );  
        report_feature_name(feat);  
        UF_CALL(UF_MODL_ask_feat_relatives(feat, &n_parents, &parents, &n_children,  
            &children));  
        sprintf( msg, "    %d parent(s): \n", n_parents);  
        WRITE( msg );  
        for (ii = 0; ii < n_parents; ii++)  
        {  
            sprintf( msg, "        ");  
            WRITE( msg );  
            report_feature_name(parents[ii]);  
        }  
        UF_free(parents);  
        sprintf( msg, "    %d children: \n", n_children);  
        WRITE( msg );  
        for (ii = 0; ii < n_children; ii++)  
        {  
            sprintf( msg, "        ");  
            WRITE( msg );  
            report_feature_name(children[ii]);  
        }  
        UF_free(children);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        char  
            msg[300] = { "" };  
        sprintf( msg, "\n\n");  
        WRITE( msg );  
        while ((feat = select_a_feature("Select feature")) != NULL_TAG)  
            report_feature_relatives(feat);  
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

> 这是一段用于NX 10及以上版本的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在调用UF函数失败时输出错误信息。
> 2. 定义了一个特征选择函数select_a_feature，通过UF_UI_select_feature函数让用户选择一个特征，并返回特征tag。
> 3. 定义了一个特征名称报告函数report_feature_name，通过UF_MODL_ask_feat_name函数获取特征名称并输出。
> 4. 定义了一个特征关系报告函数report_feature_relatives，通过UF_MODL_ask_feat_relatives函数获取特征的父母和子特征，并输出。
> 5. 定义了一个主函数do_it，循环调用select_a_feature函数让用户选择特征，然后调用report_feature_relatives函数报告特征关系。
> 6. 定义了ufusr函数，这是NX二次开发的入口函数，初始化后调用do_it函数，然后结束。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这段代码通过UI函数和模型函数实现了选择特征并报告特征关系的目的，适用于NX 10及以上版本。
>
