### 【2613】select a tolerance feature 选择公差特征

#### 代码

```cpp
    /*HEAD SELECT_A_TOLERANCE_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_gdt.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数。 */  
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
    static int mask_for_tol_feat_insts( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t    mask[1] = { UF_tol_feature_instance_type, 0, 0};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_tolerance_feature_inst( char *prompt)  
    {  
        int     resp;  
        double  cp[3];  
        tag_t   object, view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_tol_feat_insts, NULL, &resp,  
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
        tag_t   inst_tag;  
        tag_t   feat_tag;  
        inst_tag = select_a_tolerance_feature_inst("Select tolerance feature");  
            printf("Feature instance ID selected: %d\n", inst_tag);  
        UF_CALL( UF_GDT_ask_tol_feat_of_instance( inst_tag, &feat_tag));  
            printf("Feature tag of instance: %d\n", feat_tag);  
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

> 这段代码是一个NX的二次开发示例，主要功能是从用户界面中选择一个公差特征实例，并获取该实例对应的公差特征标签。
>
> 代码的主要功能包括：
>
> 1. 定义错误报告函数report_error，用于在调用UF函数出错时打印错误信息。
> 2. 定义选择过滤函数mask_for_tol_feat_insts，用于过滤出公差特征实例。
> 3. 定义选择函数select_a_tolerance_feature_inst，用于弹出对话框，让用户选择一个公差特征实例。
> 4. 在do_it函数中，首先调用select_a_tolerance_feature_inst函数获取用户选择的公差特征实例标签。然后调用UF_GDT_ask_tol_feat_of_instance函数获取该实例对应的公差特征标签，并打印出来。
> 5. 定义ufusr函数，用于初始化和调用do_it函数。同时，定义ufusr_ask_unload函数，表示二次开发模块可以立即卸载。
>
> 总体来说，这段代码通过UF函数实现了选择公差特征实例并获取其特征标签的功能，为NX的二次开发提供了一个简单的示例。
>
