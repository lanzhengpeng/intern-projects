### 【2302】report parameters of selected instances set 报告选定点集的参数

#### 代码

```cpp
    /*HEAD REPORT_PARAMETERS_OF_SELECTED_INSTANCES_SET CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_exps,  
            n_sets;  
        tag_t  
            *exps,  
            feature,  
            *sets;  
        char  
            *angle,  
            **descs,  
            *d_x,  
            *d_y,  
            *n_x,  
            *n_y,  
            msg[MAX_LINE_SIZE+1],  
            *number,  
            *radius,  
            *string,  
            *type;  
        while ((feature = select_a_feature("Report instance set parameters"))  
            != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_feat_type(feature, &type));  
            if (!strcmp(type, "INSTANCE"))  
            {  
                UF_CALL(UF_MODL_ask_instance_iset(feature, &feature));  
                UF_free(type);  
                UF_CALL(UF_MODL_ask_feat_type(feature, &type));  
            }  
            if (!strcmp(type, "INSTNACE_SET") || !strcmp(type, "INSTANCE_SET"))  
            {  
                UF_CALL(UF_MODL_ask_sets_of_member(feature, &sets, &n_sets));  
                if (n_sets > 0)  
                {  
                    feature = sets[0];  
                    UF_free(sets);  
                    UF_free(type);  
                    UF_CALL(UF_MODL_ask_feat_type(feature, &type));  
                }  
            }  
            if (!strcmp(type, "LINEAR_ISET"))  
            {  
                UF_CALL(UF_MODL_ask_linear_iset_parms(feature, 0, &n_x, &n_y,  
                    &d_x, &d_y));  
                WRITE_S(n_x);  
                WRITE_S(n_y);  
                WRITE_S(d_x);  
                WRITE_S(d_y);  
                UF_free(n_x);  
                UF_free(n_y);  
                UF_free(d_x);  
                UF_free(d_y);  
            }  
            if ( !strcmp(type, "CIRCULAR_ISET"))  
            {  
                UF_CALL(UF_MODL_ask_circular_iset_parms(feature, 0, &radius,  
                    &number, &angle));  
                WRITE_S(radius);  
                WRITE_S(number);  
                WRITE_S(angle);  
                UF_free(radius);  
                UF_free(number);  
                UF_free(angle);  
            }  
            if (!strcmp(type, "LINEAR_PATTERN") ||  
                !strcmp(type, "CIRCULAR_PATTERN"))  
            {  
                UF_CALL(UF_MODL_ask_exp_desc_of_feat(feature, &n_exps, &descs,  
                    &exps));  
                if (n_exps > 0)  
                {  
                    UF_CALL(UF_UI_open_listing_window());  
                    for (ii = 0; ii < n_exps; ii++)  
                    {  
                        UF_CALL(UF_UI_open_listing_window());  
                        UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &string));  
                        sprintf(msg, "%s = \"%s\"\n", descs[ii], string);  
                        UF_free(string);  
                        UF_CALL(UF_UI_write_listing_window(msg));  
                    }  
                    UF_free_string_array(n_exps, descs);  
                    UF_free(exps);  
                }  
            }  
            UF_free(type);  
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

> 这段代码是NX的二次开发代码，其主要功能是获取NX中特征实例集的参数信息并打印出来。代码的主要流程包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印UF函数的错误信息。
> 2. 定义了一个选择特征的函数select_a_feature，用于让用户选择一个特征。
> 3. 定义了一个写入字符串到列表窗口的函数write_string_to_listing_window。
> 4. 在do_it函数中，使用循环让用户不断选择特征，然后判断特征类型：如果是线性实例集，则获取其参数并打印。如果是圆形实例集，则获取其参数并打印。如果是线性或圆形阵列，则获取其表达式描述并打印。
> 5. 如果是线性实例集，则获取其参数并打印。
> 6. 如果是圆形实例集，则获取其参数并打印。
> 7. 如果是线性或圆形阵列，则获取其表达式描述并打印。
> 8. 主函数ufusr初始化NX，调用do_it函数，然后终止NX。
> 9. 定义了一个询问卸载函数ufusr_ask_unload，用于在卸载用户函数时立即卸载。
>
> 通过该代码，用户可以方便地查看NX中特征实例集的参数信息，有助于了解特征的具体设置。
>
