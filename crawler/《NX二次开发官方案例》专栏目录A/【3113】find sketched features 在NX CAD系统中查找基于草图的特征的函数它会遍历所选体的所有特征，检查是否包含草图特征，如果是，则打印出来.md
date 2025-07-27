### 【3113】find sketched features 在NX CAD系统中查找基于草图的特征的函数它会遍历所选体的所有特征，检查是否包含草图特征，如果是，则打印出来

#### 代码

```cpp
    /*HEAD FIND_SKETCHED_FEATURES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_sket.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据您的要求，UF_print_syslog 是 V18 新增的。 */  
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
    static tag_t select_a_body(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            body,  
            view;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY;  
        mask.object_type = UF_solid_type;  
        mask.object_subtype = 0;  
        mask.solid_type = UF_UI_SEL_FEATURE_BODY;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &resp, &body, cp, &view));  
        if (resp == 4 || resp == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(body, FALSE));  
            return body;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
              inx,  
              i,  
              list_count,  
              count;  
        uf_list_p_t  
              sket_list,  
              list;  
        char  
              *feat_type;  
        tag_t  
              sket_tag,  
              body,  
              feat;  
        while ((body = select_a_body("Select a body")) != 0)  
        {  
            UF_CALL(UF_MODL_ask_body_feats( body, &list));  
            UF_CALL(UF_MODL_ask_list_count( list, &count));  
            printf("number of features: %d\n", count);  
            for (i = 0; i < count; i++)  
            {  
                UF_CALL(UF_MODL_ask_list_item( list, i, &feat));  
                UF_CALL(UF_MODL_ask_feat_type( feat, &feat_type));  
                printf("Feature Type: %s\n", feat_type);  
                if(!strcmp("SWP104",feat_type))  
                {  
                    UF_CALL(UF_SKET_ask_feature_sketches(feat, &sket_list));  
                    if (sket_list)  
                    {  
                        UF_CALL(UF_MODL_ask_list_count(sket_list, &list_count));  
                        for(inx = 0; inx < list_count; inx ++)  
                        {  
                            UF_CALL(UF_MODL_ask_list_item( sket_list, inx,  
                                &sket_tag));  
                            printf("   SWEEP %d was created from a sketch\n",  
                                feat);  
                        }  
                        UF_MODL_delete_list(&sket_list);  
                    }  
                }  
                UF_free(feat_type);  
            }  
            UF_MODL_delete_list(&list);  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX Open C++代码主要用于在NX模型中查找由草图创建的扫掠特征。其主要功能和步骤包括：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 选择体函数：定义了一个选择体函数select_a_body，用于提示用户选择一个实体体，并返回其tag。
> 3. 主体功能函数：定义了一个主体功能函数do_it，用于遍历用户选择的每个实体体，获取其特征列表，并检查每个特征是否为扫掠特征，如果是，则进一步获取其草图信息并打印。
> 4. NX Open初始化：在ufusr函数中，首先调用UF_initialize进行NX Open初始化，然后调用do_it执行主体功能，最后调用UF_terminate终止NX Open。
> 5. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于在加载项卸载时返回立即卸载标志。
>
> 综上所述，这段代码实现了在NX模型中查找由草图创建的扫掠特征的功能，并提供了友好的用户交互提示。
>
