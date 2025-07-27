### 【0780】create new ncgeom group object 创建一个新的NC几何组对象

#### 代码

```cpp
    /*HEAD CREATE_NEW_NCGEOM_GROUP_OBJECT CCC UFUN */  
    #include <uf_exit.h>  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ncgroup.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的功能。 */  
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
    static void do_it(void)   
    {   
        tag_t   new_object = NULL_TAG;  
        char    mssg[132];  
        UF_CALL( UF_NCGEOM_create( "mill_planar", "mill_bnd", &new_object));  
        if( new_object != NULL_TAG)   
        {  
            sprintf( mssg, "Tag of new object: %d", new_object);  
            uc1601( mssg, 1);  
        }  
    }   
    /* 里海 */  
    extern void ufusr(char *param, int *retcode, int paramLen)  
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

> 这段代码是一段NX的二次开发代码，主要功能是创建一个新的NCGEOM几何体组对象。
>
> 代码的详细解释如下：
>
> 1. 头文件包含：代码包含了UF框架的头文件，如uf.h、uf_ncgroup.h、uf_modl.h等，这些头文件提供了UF框架的API接口。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于在发生错误时输出错误信息。
> 3. 创建NCGEOM几何体组对象：在do_it函数中，调用UF_NCGEOM_create API创建一个新的NCGEOM几何体组对象。参数"mill_planar"和"mill_bnd"分别指定了NC加工操作的类型和边界类型。
> 4. 输出对象标识：如果创建成功，调用sprintf函数格式化输出新对象的标识。
> 5. ufusr入口函数：ufusr是NX二次开发的入口函数，调用UF_initialize初始化UF框架，调用do_it执行创建操作，最后调用UF_terminate终止UF框架。
> 6. 卸载处理：ufusr_ask_unload函数返回UF_UNLOAD_IMMEDIATELY，表示在卸载时立即卸载该二次开发程序。
>
> 总体来说，这段代码实现了在NX中创建一个新的NCGEOM几何体组对象的功能，并提供了错误报告和日志输出机制，是典型的NX二次开发代码示例。
>
