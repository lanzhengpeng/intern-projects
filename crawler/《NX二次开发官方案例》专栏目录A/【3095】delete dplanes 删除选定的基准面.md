### 【3095】delete dplanes 删除选定的基准面

#### 代码

```cpp
    /*HEAD DELETE_DPLANES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 新增的函数。 */  
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
    static int mask_for_dplanes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_datum_plane_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_dplanes(char *prompt, tag_t **dplanes)  
    {  
        int  
            ii,  
            n,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_dplanes, NULL, &resp, &n, dplanes));  
        objs = *dplanes;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return n;  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *dplanes,  
            *dfeats;  
        uf_list_p_t  
            feat_list;  
        while ((n = select_dplanes("Select datum planes to delete", &dplanes)) > 0)  
        {  
            dfeats = (tag_t *)malloc(n * sizeof(tag_t));  
            for (ii = 0; ii < n; ii++)  
                UF_CALL(UF_MODL_ask_object_feat(dplanes[ii], &dfeats[ii]));  
            feat_list = make_a_list(n, dfeats);  
            UF_CALL(UF_MODL_delete_feature(feat_list));  
            UF_CALL(UF_MODL_delete_list(&feat_list));  
            free(dfeats);  
            UF_free(dplanes);  
        }  
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

> 根据提供的NX二次开发代码，这是一段用于删除选定基准平面的函数。具体步骤如下：
>
> 1. 包含所需的头文件，包括uf.h等NX API头文件。
> 2. 定义UF_CALL宏，用于调用NX API函数，并报告错误。
> 3. 定义report_error函数，用于打印错误信息。
> 4. 定义mask_for_dplanes函数，用于设置选择过滤器，只允许选择基准平面。
> 5. 定义select_dplanes函数，用于通过对话框让用户选择要删除的基准平面，并返回选择的基准平面数组。
> 6. 定义make_a_list函数，用于创建一个特征列表。
> 7. 定义do_it函数，用于删除选择的基准平面。首先选择基准平面，然后获取每个基准平面对应的特征，创建特征列表，调用UF_MODL_delete_feature删除特征列表，最后释放内存。
> 8. 定义ufusr函数，这是NX二次开发的入口函数。初始化NX，调用do_it函数，终止NX。
> 9. 定义ufusr_ask_unload函数，用于设置二次开发模块卸载方式。
>
> 总体来说，这段代码通过对话框让用户选择基准平面，然后删除这些基准平面及其对应的特征，实现了基准平面的删除功能。
>
