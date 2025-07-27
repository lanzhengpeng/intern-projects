### 【2120】report feature rpo constraints 报告特征RPO约束

#### 代码

```cpp
    /*HEAD REPORT_FEATURE_RPO_CONSTRAINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
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
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void report_feature_name(tag_t feat)  
    {  
        char  
            *string;  
        UF_CALL(UF_MODL_ask_feat_name(feat, &string));  
        printf("%d = %s\n", feat, string);  
        UF_free(string);  
    }  
    static void report_tag_type_and_subtype(char *name, tag_t object)  
    {  
        int  
            subtype,  
            type;  
        if (object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            printf("    %s = %d, type = %d, subtype = %d\n",  
                name, object, type, subtype);  
        }  
        else  
            printf("    %s = %d (NULL_TAG)\n", name, object);  
    }  
    static void report_feature_rpo_constraints(tag_t feat)  
    {  
        int  
            ii,  
            n_constraints;  
        tag_t  
            constraint,  
            part,  
            tool;  
        uf_list_p_t  
            constraint_list;  
        char  
            *constraint_type,  
            *how_far;  
        double  
            fixed1[2],  
            fixed2[2],  
            feature1[2],  
            feac[2];  
        report_feature_name(feat);  
        UF_CALL(UF_MODL_ask_constraints(feat, &constraint_list));  
        UF_CALL(UF_MODL_ask_list_count(constraint_list, &n_constraints));  
        for (ii = 0; ii < n_constraints; ii++)  
        {  
            UF_CALL(UF_MODL_ask_list_item(constraint_list, ii, &constraint));  
            UF_CALL(UF_MODL_ask_constraint_type(constraint, &constraint_type));  
            printf("  %d.  %s\n", ii+1, constraint_type);  
            if (!strcmp(constraint_type, "PERP_DIST_PARMS"))  
            {  
                UF_CALL(UF_MODL_ask_perp_dist(feat, constraint, 1, fixed1, fixed2,  
                    feature1, feac, &how_far, &tool, &part));  
                report_tag_type_and_subtype("tool", tool);  
                report_tag_type_and_subtype("part", part);  
                UF_free(how_far);  
            }  
            UF_free(constraint_type);  
        }  
        UF_CALL(UF_MODL_delete_list(&constraint_list));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        while ((feat = select_a_feature("Select feature")) != NULL_TAG)  
            report_feature_rpo_constraints(feat);  
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

> 这段代码是一个NX的二次开发示例，主要用于查询和报告特征的约束信息。
>
> 主要功能包括：
>
> 1. 错误报告函数：用于在函数调用失败时输出错误信息。
> 2. 选择特征函数：通过UI提示用户选择特征，并返回特征tag。
> 3. 报告特征名称函数：用于获取和打印特征名称。
> 4. 报告tag类型和子类型函数：用于获取和打印tag的类型和子类型信息。
> 5. 报告特征约束函数：查询特征的所有约束，并打印每个约束的类型。对于特定约束类型，还会获取和打印约束的详细信息，如工具和部件等。
> 6. 主函数：循环调用选择特征函数，对于每个选中的特征，调用报告特征约束函数。
> 7. ufusr函数：二次开发入口函数，用于初始化和调用主函数，在退出前进行清理。
> 8. 卸载函数：用于处理插件卸载逻辑。
>
> 该代码通过UI交互，让用户选择特征，并查询和打印特征的约束信息，实现了特征约束的查询功能。
>
