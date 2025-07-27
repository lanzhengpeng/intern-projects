### 【1985】report all welds 报告所有焊点

#### 代码

```cpp
    /*HEAD REPORT_ALL_WELDS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新增功能，具体功能为打印系统日志。 */  
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
        UF_CALL(UF_WELD_locate_welds(part, UF_WELD_ALL_TYPES, &n_welds, &welds));  
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

> 这段代码是一个NX Open C++的二次开发示例，其主要功能是报告当前工作部件中的所有焊接特征。以下是代码的详细解释：
>
> 1. 引入了必要的NX Open头文件，包括UF、UI、ASSEM、MODL和WELD模块。
> 2. 定义了UF_CALL宏，用于调用NX Open函数，并记录错误信息。
> 3. 定义了report_error函数，用于输出错误信息。
> 4. 定义了WRITE_D宏，用于将整数值写入列表窗口。
> 5. 定义了write_integer_to_listing_window函数，用于将整数值写入列表窗口。
> 6. 定义了report_feature_name_and_type函数，用于报告特征名称和类型。
> 7. 定义了do_it函数，用于查找工作部件中的所有焊接特征，并报告它们的名称和类型。
> 8. 定义了ufusr函数，作为程序的入口点，初始化NX Open，调用do_it函数，然后终止NX Open。
> 9. 定义了ufusr_ask_unload函数，用于卸载应用程序，这里设置为立即卸载。
> 10. 代码中使用了NX Open的Weld模块，通过UF_WELD_locate_welds函数查找工作部件中的所有焊接特征，然后遍历特征数组，使用MODL模块的函数获取特征名称和类型，并输出。
> 11. 输出信息可以通过UF_print_syslog函数打印到系统日志，或者通过UI模块的函数写入列表窗口。
> 12. 代码遵循了NX Open C++编程规范，使用UF_前缀的函数调用，并进行了错误处理。
> 13. 这是一个典型的NX Open二次开发示例，用于演示如何查找和报告模型中的特征信息。
>
