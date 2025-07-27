### 【2401】report udfs in part 报告部件中的UDF特征

#### 代码

```cpp
    /*HEAD REPORT_UDFS_IN_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数。 */  
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
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == 3) return feat;  
        return NULL_TAG;  
    }  
    static void report_udf(tag_t feat)  
    {  
        int  
            ii,  
            jj;  
        char  
            msg[133],  
            *name = { "Name unknown" },  
            *type = { "Type unknown" };  
        ii = UF_CALL(UF_MODL_ask_feat_name(feat, &name));  
        jj = UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
        if (strstr(type, "UDF"))  
        {  
            sprintf(msg, "%d = %s (%s)\n", feat, name, type);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            printf(msg);  
        }  
        if (ii == 0) UF_free(name);  
        if (jj == 0) UF_free(type);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        ECHO(part);  
        UF_CALL(UF_UI_open_listing_window());  
        while ((feat = ask_next_feature(part, feat)) != NULL_TAG)  
            report_udf(feat);  
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

> 这段代码是NX二次开发的用户函数，其主要功能是遍历当前显示的部件中的所有特征，并报告其中使用用户定义特征(UDF)的特征。
>
> 主要特点包括：
>
> 1. 使用UF库函数进行NX的二次开发，如UF_OBJ_cycle_objs_in_part遍历部件中的特征。
> 2. 定义了一个错误报告函数report_error，用于在出错时打印错误信息，包括文件名、行号、函数名和错误码。
> 3. ask_next_feature函数用于遍历部件中的特征，并返回下一个使用状态为3(已使用)的特征。
> 4. report_udf函数用于报告特征信息，如果特征类型中包含"UDF"，则打印特征名和类型。
> 5. do_it函数用于打开一个列表窗口，然后遍历当前显示部件的所有特征，并报告使用UDF的特征。
> 6. ufusr是用户函数的入口点，初始化NX环境，执行do_it，然后终止NX环境。
> 7. ufusr_ask_unload函数返回立即卸载标志，表示在调用ufusr后可以立即卸载该函数。
>
> 总体来说，这段代码实现了遍历特征并报告UDF特征的功能，具有较好的错误处理和资源释放，是典型的NX二次开发用户函数。
>
