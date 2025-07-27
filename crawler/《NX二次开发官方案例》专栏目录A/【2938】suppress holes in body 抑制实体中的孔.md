### 【2938】suppress holes in body 抑制实体中的孔

#### 代码

```cpp
    /*HEAD SUPPRESS_HOLES_IN_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新增功能。 */  
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
    /* qq3123197280 */  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* qq3123197280 */  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            count = 0;  
        tag_t  
            body = NULL_TAG,  
            feat = NULL_TAG;  
        uf_list_p_t  
            feat_list,    
            hole_list;     
        char  
            *feature_name = NULL;  
        while ((body = select_a_body("Select a Body:")) != NULL_TAG)  
        {  
            UF_CALL( UF_MODL_create_list( &hole_list ));  
            UF_CALL( UF_MODL_ask_body_feats( body, &feat_list ));  
            UF_CALL( UF_MODL_ask_list_count( feat_list, &count));  
            for( inx = 0; inx < count; inx++ )  
            {  
                UF_CALL( UF_MODL_ask_list_item( feat_list, inx, &feat ));  
                UF_CALL(UF_MODL_ask_feat_name( feat, &feature_name ));  
                if( strstr( feature_name, "HOLE" ) != 0 )  
                {  
                    UF_CALL( UF_MODL_put_list_item( hole_list, feat ));    
                }  
                UF_free( feature_name );  
            }  
            UF_CALL( UF_MODL_suppress_feature( hole_list) );  
            UF_CALL( UF_MODL_delete_list( &feat_list ));  
            UF_CALL( UF_MODL_delete_list( &hole_list ));  
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

> 这段NX二次开发代码的主要功能是选择实体，并抑制其中的孔特征。具体介绍如下：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 实体选择：定义了一个实体选择函数mask_for_bodies，设置了选择过滤器，只允许选择实体类型的特征体。
> 3. 选择一个实体：定义了一个选择实体的函数select_a_body，用于弹出对话框让用户选择一个实体，并返回其tag。
> 4. 抑制孔特征：定义了一个主函数do_it，用于遍历用户选择的每个实体，获取其特征列表，找出名称包含"HOLE"的特征，并将这些孔特征放入一个列表中。最后调用UF_MODL_suppress_feature函数抑制这个列表中的所有孔特征。
> 5. 用户自定义函数：定义了ufusr函数作为用户自定义函数的入口点，用于初始化NX并调用主函数do_it。还定义了ufusr_ask_unload函数，表示该程序可以被立即卸载。
>
> 通过以上功能，这段代码实现了在NX中选择实体并抑制其中的孔特征的功能。
>
