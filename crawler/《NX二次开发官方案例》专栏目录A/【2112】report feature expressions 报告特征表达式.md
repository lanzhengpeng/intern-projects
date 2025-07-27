### 【2112】report feature expressions 报告特征表达式

#### 代码

```cpp
    /*HEAD REPORT_FEATURE_EXPRESSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中新增的功能。 */  
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
    static void report_feature_expressions(tag_t feat)  
    {  
        int  
            ii,  
            n_exps;  
        tag_t  
            *exps,  
            master;  
        char  
            msg[133],  
            **descs,  
            *dia,  
            *rad,  
            *string,  
            *type,  
            *width;  
        UF_CALL(UF_MODL_ask_feat_name(feat, &string));  
        UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
        if (strstr(string, "INSTANCE[0]"))  
        {  
            UF_CALL(UF_MODL_ask_master(feat, &master));  
            report_feature_expressions(master);  
        }  
        sprintf(msg, "\n%d - %s (%s)\n", feat, string, type);  
        if (!UF_UI_open_listing_window())  
        {  
            UF_UI_write_listing_window(msg);  
        }  
    /*  Begin work around for PR 724000 里海译:开始对PR 724000进行修补工作。 */  
        if (strstr(string, "BALL_END_GROOVE"))  
        {  
            UF_CALL(UF_MODL_ask_ball_groove_parms(feat, 1, &dia, &width));  
            sprintf(msg, "using UF_MODL_ask_ball_groove_parms\n");  
            UF_UI_write_listing_window(msg);  
            sprintf(msg, "  Diameter - %s\n", dia);  
            UF_UI_write_listing_window(msg);  
            sprintf(msg, "  Ball Diameter - %s\n", width);  
            UF_UI_write_listing_window(msg);  
            UF_free(dia);  
            UF_free(width);  
        }  
        if (strstr(string, "RECTANGULAR_GROOVE"))  
        {  
            UF_CALL(UF_MODL_ask_rect_groove_parms(feat, 1, &dia, &width));  
            sprintf(msg, "using UF_MODL_ask_rect_groove_parms\n");  
            UF_UI_write_listing_window(msg);  
            sprintf(msg, "  Diameter - %s\n", dia);  
            UF_UI_write_listing_window(msg);  
            sprintf(msg, "  Width - %s\n", width);  
            UF_UI_write_listing_window(msg);  
            UF_free(dia);  
            UF_free(width);  
        }  
        if (strstr(string, "U_GROOVE"))  
        {  
            UF_CALL(UF_MODL_ask_u_groove_parms(feat, 1, &dia, &width, &rad));  
            sprintf(msg, "using UF_MODL_ask_u_groove_parms\n");  
            UF_UI_write_listing_window(msg);  
            sprintf(msg, "  Diameter - %s\n", dia);  
            UF_UI_write_listing_window(msg);  
            sprintf(msg, "  Width - %s\n", width);  
            UF_UI_write_listing_window(msg);  
            sprintf(msg, "  Corner radius - %s\n", rad);  
            UF_UI_write_listing_window(msg);  
            UF_free(dia);  
            UF_free(width);  
            UF_free(rad);  
        }  
    /*  End work around for PR 724000 里海译:End work around for PR 724000

结束对PR 724000的临时修复 */  
        UF_free(string);  
        UF_free(type);  
        if(!UF_CALL(UF_MODL_ask_exps_of_feature(feat, &n_exps, &exps)))  
        {  
            sprintf(msg, "found %d exps using UF_MODL_ask_exps_of_feature\n", n_exps);  
            UF_UI_write_listing_window(msg);  
            for (ii = 0; ii < n_exps; ii++)  
            {  
                if (!UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &string)))  
                {  
                    sprintf(msg, "  %d.  %d - %s\n", ii+1, exps[ii], string);  
                    UF_UI_write_listing_window(msg);  
                    UF_free(string);  
                }  
            }  
            if (n_exps > 0) UF_free(exps);  
        }  
        if (!UF_CALL(UF_MODL_ask_exp_desc_of_frec(feat, &n_exps, &descs, &exps)))  
        {  
            sprintf(msg, "found %d exps using UF_MODL_ask_exp_desc_of_frec\n", n_exps);  
            UF_UI_write_listing_window(msg);  
            for (ii = 0; ii < n_exps; ii++)  
            {  
                sprintf(msg, "  %d.  %d - %s", ii+1, exps[ii], descs[ii]);  
                UF_UI_write_listing_window(msg);  
                if (strcmp(descs[ii], "Parm Constraint") &&  
                    !UF_MODL_ask_exp_tag_string(exps[ii], &string))  
                {  
                    sprintf(msg, " - %s\n", string);  
                    UF_UI_write_listing_window(msg);  
                    UF_free(string);  
                }  
                else  
                {   
                    sprintf(msg, "\n");  
                    UF_UI_write_listing_window(msg);  
                }  
            }  
            if (n_exps > 0)  
            {  
                UF_free_string_array(n_exps, descs);  
                UF_free(exps);  
            }  
        }  
        if (!UF_CALL(UF_MODL_ask_rpo_desc_of_frec(feat, &n_exps, &descs, &exps)))  
        {  
            sprintf(msg, "found %d exps using UF_MODL_ask_rpo_desc_of_frec\n", n_exps);  
                    UF_UI_write_listing_window(msg);  
            for (ii = 0; ii < n_exps; ii++)  
            {  
                sprintf(msg, "  %d.  %d - %s", ii+1, exps[ii], descs[ii]);  
                UF_UI_write_listing_window(msg);  
                if (!UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &string)))  
                {  
                    sprintf(msg, " %s\n", string);  
                    UF_UI_write_listing_window(msg);  
                    UF_free(string);  
                }  
            }  
            if (n_exps > 0)  
            {  
                UF_free_string_array(n_exps, descs);  
                UF_free(exps);  
            }  
        }  
    }  
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if (!subtype) return body;  
            }  
        } while (body);  
        return NULL_TAG;  
    }  
    static int ask_body_feats(tag_t body, tag_p_t *body_feats)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *feats;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_ask_body_feats(body, &feat_list));  
        UF_CALL(UF_MODL_ask_list_count(feat_list, &n));  
        *body_feats = (feats = (tag_t *)malloc(n * sizeof(tag_t)));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_MODL_ask_list_item(feat_list, ii, &feats[ii]));  
        UF_CALL(UF_MODL_delete_list(&feat_list));  
        return n;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_feats;  
        tag_t  
            body = NULL_TAG,  
            *feats,  
            part = UF_PART_ask_display_part();  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            n_feats = ask_body_feats(body, &feats);  
            for (ii = 0; ii < n_feats; ii++)  
            {  
                report_feature_expressions(feats[ii]);  
            }  
            free(feats);  
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

> 这段代码是用于在NX中遍历所有特征并输出特征名称和表达式的二次开发代码。具体包括以下内容：
>
> 1. 定义了错误报告函数report_error，用于输出UF函数的错误信息。
> 2. 定义了report_feature_expressions函数，用于输出特征名称、类型、表达式等信息。它首先获取特征名称和类型，然后输出。对于实例特征，会递归输出其主特征。接下来，根据特征名称获取不同的槽参数。最后，遍历特征的所有表达式，并输出表达式描述和标签。
> 3. 定义了ask_next_body函数，用于遍历零件中的所有实体。
> 4. 定义了ask_body_feats函数，用于获取实体中的所有特征。
> 5. 定义了do_it函数，用于遍历零件中的所有实体，获取每个实体的特征，然后输出特征信息。
> 6. 定义了ufusr函数，用于在NX启动时调用do_it函数，遍历输出特征信息。
> 7. 定义了ufusr_ask_unload函数，用于在NX关闭时卸载二次开发库。
>
> 总体来说，这段代码通过遍历零件中的所有特征，并输出特征名称和表达式信息，实现了对NX中特征信息的查询功能。
>
