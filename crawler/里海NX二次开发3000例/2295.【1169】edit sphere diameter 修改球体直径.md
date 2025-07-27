### 【1169】edit sphere diameter 修改球体直径

#### 代码

```cpp
    /*HEAD EDIT_SPHERE_DIAMETER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，关于 UF_print_syslog 的说明如下：

UF_print_syslog 是在 V18 版本中新增的。 */  
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
    /* qq3123197280 */  
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
    /* qq3123197280 */  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    /* qq3123197280 */  
    static void do_it(void)  
    {  
        tag_t  
            sphere_feat = NULL_TAG,  
                exp_tag = NULL_TAG;  
        char  
            *diameter = NULL,  
            *lhs_str = NULL,  
            *rhs_str = NULL,  
            new_exp_str[UF_MAX_EXP_LENGTH+1] = { "" };  
        double  
            number = 0.0;  
        if(( (sphere_feat = select_a_feature("Select a Sphere:")) != NULL_TAG ) &&  
                prompt_for_a_number( "Enter New Sphere Diameter:","size", &number ))   
        {  
            UF_CALL( UF_MODL_ask_sphere_parms ( sphere_feat, 0, &diameter ));  
            UF_CALL( UF_MODL_dissect_exp_string(diameter, &lhs_str,   
                                                &rhs_str, &exp_tag ));  
            sprintf( new_exp_str, "%s=%f", lhs_str, number );  
            UF_CALL( UF_MODL_edit_exp( new_exp_str ));  
            UF_MODL_update();  
            if( diameter != NULL ) UF_free( diameter );  
            if( lhs_str != NULL ) UF_free( lhs_str );  
            if( rhs_str != NULL ) UF_free( rhs_str );  
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

> 这段代码是用于NX的二次开发，主要功能是选择一个球特征，然后修改其直径。
>
> 主要步骤包括：
>
> 1. 包含必要的头文件，定义错误报告宏，用于输出错误信息。
> 2. 定义选择特征的函数select_a_feature，用于选择一个球特征。
> 3. 定义提示输入数值的函数prompt_for_a_number，用于提示用户输入新的球直径。
> 4. 定义主体函数do_it，用于执行修改直径的流程：选择一个球特征提示输入新的直径获取当前直径的表达式构建新的直径表达式修改直径更新模型
> 5. 选择一个球特征
> 6. 提示输入新的直径
> 7. 获取当前直径的表达式
> 8. 构建新的直径表达式
> 9. 修改直径
> 10. 更新模型
> 11. 定义ufusr函数，初始化NX，调用do_it函数，然后终止NX。
> 12. 定义卸载函数ufusr_ask_unload，用于立即卸载用户函数。
>
> 整体来说，这段代码通过选择特征、获取参数、构建新表达式、修改参数、更新模型的方式实现了球直径的修改功能，结构清晰，流程完整。
>
