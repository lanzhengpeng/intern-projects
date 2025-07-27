### 【1652】move feature 移动特征

#### 代码

```cpp
    /*HEAD MOVE_FEATURE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog 是 V18 中的新功能，请只提供翻译，不要添加其他无关内容。 */  
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
    static uf_list_p_t make_a_list( int count, tag_t *item_array)  
    {  
        uf_list_p_t list;  
        int         ii;  
        UF_CALL( UF_MODL_create_list( &list));  
        for( ii = 0; ii < count; ii++)  
            UF_CALL( UF_MODL_put_list_item( list, item_array[ii]));  
        return( list);  
    }  
    static int select_features(char *prompt, tag_t **feats)  
    {  
        int  
            cnt,  
            resp;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, feats, &resp));  
        if (resp == UF_UI_OK) return cnt;  
        else return 0;  
    }  
    static void do_it(void)  
    {  
        int     cnt = 0;  
        tag_t   *feats;  
        uf_list_p_t feat_list;  
        double  mat[2][12];  
        mat[0][0] = 0.0;  
        mat[0][1] = 0.0;  
        mat[0][2] = 0.0;  
        mat[1][0] = 2.0;  
        mat[1][1] = 0.0;  
        mat[1][2] = 0.0;  
        cnt = select_features("Select features", &feats);  
        feat_list = make_a_list( cnt, feats);  
        UF_CALL( UF_MODL_move_feature( feat_list, 0, mat));  
        UF_CALL( UF_MODL_update());  
        UF_free( feats);  
    }  
    /* qq3123197280 */  
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

> 这段代码是一个NX Open C++ API二次开发示例，用于实现移动特征功能。下面是代码的主要功能介绍：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了一个创建列表的函数make_a_list，用于将特征数组转换为一个特征列表。
> 3. 定义了一个选择特征的函数select_features，使用户可以交互式选择特征，并返回特征数组。
> 4. do_it函数实现了主要功能：提示用户选择特征，创建特征列表，定义移动的矩阵，调用UF_MODL_move_feature函数移动特征，并更新模型。
> 5. ufusr是二次开发程序的入口函数，首先初始化API，然后调用do_it函数实现功能，最后终止API。
> 6. ufusr_ask_unload函数用于卸载二次开发程序，此处返回立即卸载。
> 7. 使用了NX Open C++ API中的UF_MODL模块中的UF_MODL_move_feature函数来移动特征，并使用UF_UI模块中的函数来与用户交互选择特征。
> 8. 使用了宏定义来简化错误报告和函数调用。
> 9. 代码结构清晰，各个功能通过独立函数实现，便于阅读和维护。
>
> 综上所述，这段代码提供了一个NX二次开发的典型示例，通过交互式选择特征并移动，实现了特征移动功能。
>
