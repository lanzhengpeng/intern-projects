### 【1298】find sketched features 找到草图特征

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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是一个新引入V18版本的功能。该功能用于打印系统日志。 */  
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

> 这段代码的主要功能是遍历NX中的body，并找出由草图创建的SWEEP特征。
>
> 代码首先定义了错误处理函数report_error，用于打印错误信息。然后定义了选择body的函数select_a_body，该函数会提示用户选择一个body，并返回body的tag。
>
> 在主函数do_it中，通过循环调用select_a_body函数，获取用户选择的每个body。然后调用UF_MODL_ask_body_feats函数获取body上的所有特征，遍历特征列表，获取每个特征的类型。如果特征类型是SWP104（表示SWEEP特征），则调用UF_SKET_ask_feature_sketches函数获取该特征对应的草图列表。如果草图列表不为空，说明该SWEEP特征是由草图创建的，打印相关信息。
>
> 最后，主函数ufusr负责初始化NX环境，调用do_it函数，然后清理环境。unload函数返回立即卸载。
>
> 总的来说，这段代码通过遍历body和特征，实现了查找由草图创建的SWEEP特征的功能。
>
