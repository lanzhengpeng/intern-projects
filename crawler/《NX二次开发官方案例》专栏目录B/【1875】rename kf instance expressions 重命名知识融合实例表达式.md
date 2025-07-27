### 【1875】rename kf instance expressions 重命名知识融合实例表达式

#### 代码

```cpp
    /*HEAD RENAME_KF_INSTANCE_EXPRESSIONS CCC UFUN */  
    /*  This example demonstrates renaming the expressions of features created in  
        Knowledge Fusion.  
        This is an example dfa file demonstrating its use:  
        #! NX/KF 3.0  
        DefClass: ir5145656 (ug_base_part);  
        (Child) block: {  
             Class, ug_block;  
        };  
        (Boolean Modifiable) Once: False;  
        (Boolean) go:  
            if (Once: = False)  
            then @{  
                rename_exps(block:, Block);  
                storeValue( True, self:, once);  
            }  
            else FALSE;  
        (child) do_this_last: {  
             Class, ug_point;  
             Suppress?, True;  
             DemandValue, { go: };  
             DemandOrder, { block: };  
        };  
        Defun: rename_exps( Instance $feature, Name $prefix,  
            string ($libname, "rename_kf_instance_expressions"),  
            string ($funcname, "rename_exps"))  
            @{ Cfunc("UF_KF_invoke_user_function"; "libufun"); } Integer;  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_kf.h>  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static int rename_feature_expressions(tag_t feat, char *prefix)  
    {  
        int  
            err,  
            ii,  
            n_exps;  
        tag_t  
            exp,  
            *exps;  
        char  
            **descs,  
            *lhs,  
            new_exp[UF_MAX_EXP_LENGTH+1],  
            *rhs,  
            *string;  
        err = UF_CALL(UF_MODL_ask_exp_desc_of_feat(feat, &n_exps, &descs, &exps));  
        if (!err)  
        {  
            for (ii = 0; ii < n_exps; ii++)  
            {  
                err = UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &string));  
                if (!err)  
                {  
                    err = UF_CALL(UF_MODL_dissect_exp_string(string, &lhs, &rhs,  
                        &exp));  
                    if (!err)  
                    {  
                        sprintf(new_exp, "%s_%d", prefix, ii+1);  
                        WRITE("renaming ");  
                        WRITE(lhs);  
                        WRITE(" to ");  
                        WRITE(new_exp);  
                        WRITE("...\n");  
                        UF_CALL(UF_MODL_rename_exp(lhs, new_exp));  
                        UF_free(lhs);  
                        UF_free(rhs);  
                    }  
                    UF_free(string);  
                }  
            }  
            UF_free(exps);  
            UF_free_string_array(n_exps, descs);  
        }  
        return err;  
    }  
    /* qq3123197280 */  
    DllExport extern void rename_exps(UF_KF_value_t *data,  
                                      int num,  
                                      UF_KF_value_t *rv)  
    {  
        int  
            err = 0;  
        unsigned int  
            name;  
        tag_t  
            feat;  
        char  
            *prefix;  
        UF_KF_instance_t  
            instance;  
        *rv = NULL;  
        if (UF_CALL(UF_initialize())) return;  
        err = UF_CALL(UF_KF_ask_instance(data[0], &instance));  
        if (!err)  
            err = UF_CALL(UF_KF_ask_ug_object(instance, &feat));  
        if (!err)  
            err = UF_CALL(UF_KF_ask_name(data[1], &name));  
        if (!err)  
            err = UF_CALL(UF_KF_ask_string_of_name(name, &prefix));  
        if (!err)  
        {  
            err = rename_feature_expressions(feat, prefix);  
            UF_free(prefix);  
        }  
        UF_CALL(UF_KF_make_integer(err, rv));  
        UF_CALL(UF_terminate());  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这是NX Open Knowledge Fusion的一个二次开发示例代码，主要实现了以下功能：
>
> 1. 定义了一个Knowledge Fusion类ir5145656，它有一个子特征block，以及一个布尔特征go，用于控制是否执行表达式重命名。
> 2. 定义了一个用户函数rename_exps，用于重命名指定特征的表达式。它接收特征实例、前缀作为参数，并调用rename_feature_expressions函数实现重命名。
> 3. rename_feature_expressions函数遍历特征的所有表达式，为每个表达式的左侧添加前缀和序号，然后调用UF_MODL_rename_exp函数进行重命名。
> 4. 在rename_exps函数中，首先获取参数，然后调用rename_feature_expressions函数进行重命名，最后将结果作为整数返回。
> 5. 定义了report_error函数用于输出错误信息，以及WRITE宏用于输出日志信息。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载。
>
> 通过这个示例代码，展示了如何在Knowledge Fusion中定义类、特征、表达式，以及如何通过二次开发函数来操作这些表达式。这为在NX中进行参数化特征建模提供了基础。
>
