### 【2118】report feature of selected object 选择对象并报告其特征

#### 代码

```cpp
    /*HEAD REPORT_FEATURE_OF_SELECTED_OBJECT CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18中是新增的。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_feature_name_and_type(char *title, tag_t feat)  
    {  
        char  
            msg[MAX_LINE_SIZE+1],  
            *name,  
            *type;  
        if (!UF_CALL(UF_MODL_ask_feat_name(feat, &name)) &&  
            !UF_CALL(UF_MODL_ask_feat_type(feat, &type)))  
        {  
            UF_CALL(UF_UI_open_listing_window());  
            sprintf(msg, "%s = %d %s (%s)\n", title, feat, name, type);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            UF_free(name);  
            UF_free(type);  
        }  
    }  
    static tag_t select_an_object(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL,NULL,&resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feature,  
            object;  
        while ((object = select_an_object("Select object")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_object_feat(object, &feature));  
            WRITE_D(feature);  
            if (feature != NULL_TAG)  
                report_feature_name_and_type("feature", feature);  
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

> 这段NX二次开发代码的主要功能是选择装配体中的对象，并报告所选对象上的第一个特征的名称和类型。
>
> 具体来说，代码包括以下几个关键部分：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在函数调用出错时输出错误信息。
> 2. 整数写入列表窗口函数：定义了一个函数write_integer_to_listing_window，用于将整数值写入NX的列表窗口。
> 3. 特征名称和类型报告函数：定义了一个函数report_feature_name_and_type，用于查询特征名称和类型，并将它们写入列表窗口。
> 4. 对象选择函数：定义了一个函数select_an_object，用于使用NX的对话框选择装配体中的对象。
> 5. 主要功能函数：定义了一个函数do_it，用于循环选择对象，获取对象上的第一个特征，并报告特征信息。
> 6. ufusr入口函数：定义了NX的入口函数ufusr，用于初始化NX环境，执行主要功能函数，并终止NX环境。
> 7. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于设置NX立即卸载二次开发应用程序。
>
> 总的来说，这段代码实现了在NX中选取对象并查询其第一个特征的功能，同时包含了错误处理、日志输出等辅助功能。
>
