### 【1138】double check feature expression correlation 双重检查特征表达式关联

#### 代码

```cpp
    /*HEAD DOUBLE_CHECK_FEATURE_EXPRESSION_CORRELATION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中的新功能。在回答问题时，请只提供译文，不要添加其他无关内容。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE) return feat;  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii, jj, kk,  
            mismatches = 0,  
            n_exp_feats,  
            n_feat_exps,  
            n_part_exps;  
        tag_t  
            *exp_feats,  
            *feat_exps,  
            part_feat = NULL_TAG,  
            *part_exps,  
            part = UF_ASSEM_ask_work_part();  
        char  
            *string;  
        while ((part_feat = ask_next_feature(part, part_feat)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_exps_of_feature(part_feat, &n_feat_exps,  
                    &feat_exps));  
            for (ii = 0; ii < n_feat_exps; ii++)  
            {  
                UF_CALL(UF_MODL_ask_features_of_exp(feat_exps[ii], &n_exp_feats,  
                    &exp_feats));  
                for (jj = 0; jj < n_exp_feats; jj++)  
                    if (exp_feats[jj] == part_feat) break;  
                if (jj == n_exp_feats)  
                {  
                    WRITE("UF_MODL_ask_exps_of_feature says ");  
                    UF_CALL(UF_MODL_ask_feat_name(part_feat, &string));  
                    WRITE(string);  
                    WRITE(" uses ");  
                    UF_free(string);  
                    UF_CALL(UF_MODL_ask_exp_tag_string(feat_exps[ii], &string));  
                    WRITE(string);  
                    UF_free(string);  
                    UF_CALL(UF_MODL_ask_descriptor_of_exp(feat_exps[ii], &string));  
                    if (strcmp("", string))  
                    {  
                        WRITE(" (");  
                        WRITE(string);  
                        WRITE(")");  
                        UF_free(string);  
                    }  
                    WRITE("\n");  
                    mismatches++;  
                }  
                if (n_exp_feats > 0) UF_free(exp_feats);  
            }  
            if (n_feat_exps > 0) UF_free(feat_exps);  
        }  
        if (mismatches > 0)  
        {  
            WRITE("UF_MODL_ask_features_of_exp does NOT agree - ");  
            WRITE_D(mismatches);  
            WRITE("\n");  
        }  
        mismatches = 0;  
        UF_CALL(UF_MODL_ask_exps_of_part(part, &n_part_exps, &part_exps));  
        for (ii = 0; ii < n_part_exps; ii++)  
        {  
            UF_CALL(UF_MODL_ask_features_of_exp(part_exps[ii], &n_exp_feats,  
                &exp_feats));  
            for (jj = 0; jj < n_exp_feats; jj++)  
            {  
                UF_CALL(UF_MODL_ask_exps_of_feature(exp_feats[jj], &n_feat_exps,  
                    &feat_exps));  
                for (kk = 0; kk < n_feat_exps; kk++)  
                    if (feat_exps[kk] == part_exps[ii]) break;  
                if (kk == n_feat_exps)  
                {  
                    WRITE("UF_MODL_ask_features_of_exp says ");  
                    UF_CALL(UF_MODL_ask_exp_tag_string(part_exps[ii], &string));  
                    WRITE(string);  
                    UF_free(string);  
                    UF_CALL(UF_MODL_ask_descriptor_of_exp(part_exps[ii], &string));  
                    if (strcmp("", string))  
                    {  
                        WRITE(" (");  
                        WRITE(string);  
                        WRITE(")");  
                        UF_free(string);  
                    }  
                    WRITE(" is used by ");  
                    UF_CALL(UF_MODL_ask_feat_name(exp_feats[jj], &string));  
                    WRITE(string);  
                    WRITE("\n");  
                    UF_free(string);  
                    mismatches++;  
                }  
                if (n_feat_exps > 0) UF_free(feat_exps);  
            }  
            if (n_exp_feats > 0) UF_free(exp_feats);  
        }  
        if (n_part_exps > 0) UF_free(part_exps);  
        if (mismatches > 0)  
        {  
            WRITE("UF_MODL_ask_exps_of_feature does NOT agree - ");  
            WRITE_D(mismatches);  
            WRITE("\n");  
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
    static void check_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("\n%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这是段NX二次开发代码，主要实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义了写入列表窗口的宏WRITE和写入整数到列表窗口的函数write_integer_to_listing_window。
> 3. 定义了获取下一个特征的函数ask_next_feature。
> 4. 定义了主函数do_it，该函数遍历零件中的所有特征，检查特征与表达式之间的关联关系是否一致。如果发现不一致的情况，会打印出来。此外，函数还会检查表达式与特征之间的关联关系是否一致。
> 5. 定义了UFusr函数，初始化NX系统，调用do_it函数，然后终止NX系统。
> 6. 定义了检查加载状态的函数check_load_status。
> 7. 主函数main用于在命令行参数中指定零件文件，然后逐个打开零件文件，调用do_it函数进行检查，最后关闭所有零件文件。
>
> 总体来说，这段代码主要用于检查NX零件文件中特征与表达式之间关联关系的正确性。
>
