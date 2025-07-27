### 【0874】create set of features 创建特征集合

#### 代码

```cpp
    /*HEAD CREATE_SET_OF_FEATURES CCC UFUN */  
    #include <uf_exit.h>  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：
注意：UF_print_syslog 是 V18 中新增的功能，请只提供翻译，不要添加任何无关的内容。 */  
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
    static int select_features(char *prompt, tag_t **feats)  
    {  
        int  
            cnt,  
            resp;  
        UF_CALL(UF_UI_select_feature(prompt, UF_UI_FEAT_SEL_TYPE_BROWSEABLE, &cnt,  
            feats, &resp));  
        if (resp == UF_UI_OK) return cnt;  
        else return 0;  
    }  
    static void do_it(void)  
    {  
        tag_t   *features, grp_feat;  
        int     num_feats;  
        num_feats = select_features( "select features", &features);  
        UF_CALL( UF_MODL_create_set_of_feature( "Feature_set",  
            features, num_feats, 0, &grp_feat));  
    }  
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

> 这段代码是用于NX二次开发的功能，其主要功能是创建一个特征集。具体来说：
>
> 1. 引入了一些NX的库文件，如uf.h, uf_modl.h等。
> 2. 定义了一个宏ECHO，用于打印变量名和值。
> 3. 定义了一个错误报告函数report_error，用于打印出NX函数调用的错误信息。
> 4. 定义了一个选择特征的函数select_features，用于让用户选择特征并返回特征数组。
> 5. 定义了一个创建特征集的函数do_it，调用select_features获取特征数组，然后调用NX的UF_MODL_create_set_of_feature函数创建特征集。
> 6. 定义了ufusr函数，这是NX二次开发的入口函数。在这个函数中，先初始化NX环境，然后调用do_it函数执行创建特征集的功能，最后终止NX环境。
> 7. 定义了一个卸载函数ufusr_ask_unload，返回立即卸载的标志。
>
> 总的来说，这段代码的主要功能就是让用户选择特征，然后创建一个特征集。
>
