### 【2489】report spot welds 报告点焊

#### 代码

```cpp
    /*HEAD REPORT_SPOT_WELDS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_weld.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新功能，专门用于在日志中打印系统日志信息。 */  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            n_welds;  
        tag_t  
            part = UF_ASSEM_ask_work_part(),  
            *welds;  
        char  
            name[15];  
        UF_CALL(UF_WELD_locate_welds(part, UF_WELD_SPOT_TYPE, &n_welds, &welds));  
        WRITE_D(n_welds);  
        for (ii = 0; ii < n_welds; ii++)  
        {  
            sprintf(name, "welds[%d]", ii);  
            report_feature_name_and_type(name, welds[ii]);  
        }  
        if (n_welds > 0) UF_free(welds);  
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

> 这段代码是用于在NX中查找并报告所有点焊特征的二次开发代码。其主要功能和步骤如下：
>
> 1. 头文件包含：代码包含了一些必要的NX API头文件，用于进行UF调用。
> 2. 错误报告宏：定义了UF_CALL宏，用于进行UF函数调用并报告错误，如果调用失败会打印错误信息。
> 3. 写入整数宏：定义了WRITE_D宏，用于将整数变量写入列表窗口。
> 4. 报告特征名称和类型函数：定义了report_feature_name_and_type函数，用于获取特征的名称和类型并报告。
> 5. 主函数do_it：该函数首先获取当前工作部件，然后调用UF_WELD_locate_welds函数查找所有点焊特征，并报告点焊的数量、名称和类型。
> 6. ufusr函数：这是NX二次开发的入口函数，初始化UF系统后调用do_it函数，完成后再终止UF系统。
> 7. 卸载函数：定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总的来说，这段代码实现了在NX中查找并报告所有点焊特征的功能。
>
