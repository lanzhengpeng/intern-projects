### 【0703】create fixed dplane 创建固定平面

#### 代码

```cpp
    /*HEAD CREATE_FIXED_DPLANE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文中提到的内容，UF_print_syslog是V18版本中新增的函数。 */  
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
    static void report_feature_name(tag_t feat)  
    {  
        char  
            *name,  
            *type;  
        UF_CALL(UF_MODL_ask_feat_name(feat, &name));  
        UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
        printf("\n%d = %s (%s)\n", feat, name, type);  
        UF_free(name);  
        UF_free(type);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            dplane,  
            feat;  
        double  
            zero[3] = { 0, 0, 0 },  
            z_axis[3] = { 0, 0, 1 };  
        UF_CALL(UF_MODL_create_fixed_dplane(zero, z_axis, &dplane));  
        UF_CALL(UF_MODL_ask_object_feat(dplane, &feat));  
        report_feature_name(feat);  
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

> 这段代码是用于创建固定平面特征并输出其名称和类型的NX二次开发代码。具体包括以下几个部分：
>
> 1. 包含必要的头文件，如stdio.h、string.h等标准库头文件，以及NX二次开发相关的头文件uf.h、uf_ui.h、uf_modl.h。
> 2. 定义了report_error函数，用于输出函数调用失败的错误信息。它接受文件名、行号、函数名和错误码作为参数，并输出错误信息。
> 3. 定义了report_feature_name函数，用于输出特征的名字和类型。它接受特征tag作为参数，并调用UF_MODL_ask_feat_name和UF_MODL_ask_feat_type获取名称和类型，然后输出。
> 4. 定义了do_it函数，用于创建固定平面特征并输出其名称和类型。它使用零点和Z轴方向向量作为参数，调用UF_MODL_create_fixed_dplane创建固定平面，然后调用report_feature_name输出名称和类型。
> 5. 定义了ufusr函数，这是NX二次开发程序的主入口点。在初始化NX系统后，调用do_it函数，然后终止NX系统。
> 6. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY，表示当程序卸载时可以立即卸载。
>
> 总的来说，这段代码实现了在NX中创建固定平面特征并输出其名称和类型的功能，可以作为NX二次开发的示例代码。
>
