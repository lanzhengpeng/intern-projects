### 【1410】import a create udf v14 导入创建UDF程序V14

#### 代码

```cpp
    /*HEAD IMPORT_A_CREATE_UDF CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

请注意，UF_print_syslog是V18版本新增的函数。 */  
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
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            num_values,  
            mark;  
        tag_t  
            meta_id,  
            udf;  
        double  
            loc[3] = { 0,0,0 },  
            csys[9] = { 1,0,0, 0,1,0, 0,0,1 };  
        char  
            **udf_prompt,  
            **udf_values,  
            *udf_name;  
        while (specify_point("Select position for udf", loc))  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, NULL, &mark));  
            if (!UF_CALL(UF_MODL_import_udf("parts/udf.udf", csys, loc,  
                &udf)))  
            {  
                UF_CALL(UF_MODL_ask_udf_parms(udf, &udf_name, &udf_prompt,  
                    &udf_values, &num_values));  
                UF_CALL(UF_MODL_set_udf_parms(udf, udf_prompt, udf_values,  
                    num_values));  
                UF_free_string_array(num_values,udf_values);  
                UF_free_string_array(num_values,udf_prompt);  
                UF_free(udf_name);  
                if (UF_CALL(UF_MODL_boolean_udf(udf, NULL_TAG, NULL_TAG, csys,  
                        0, NULL, 0, NULL, FALSE, &meta_id)))  
                    UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
                else  
                {  
                    UF_CALL(UF_OBJ_set_name(meta_id, "UDF_1"));  
                    UF_CALL(UF_MODL_update());  
                }  
            }  
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

> 这段代码是一个NX Open C++的二次开发示例，用于在NX中导入用户定义特征(UDF)。
>
> 主要步骤如下：
>
> 1. 包含必要的NX Open头文件。
> 2. 定义一个报告错误函数report_error，用于在出错时打印错误信息。
> 3. 定义一个指定点的函数specify_point，用于让用户选择UDF的位置。
> 4. 定义主体函数do_it，其中循环调用specify_point函数让用户选择UDF位置，然后调用UF_MODL_import_udf函数导入UDF。
> 5. 在导入UDF后，调用UF_MODL_ask_udf_parms函数获取UDF参数，然后调用UF_MODL_set_udf_parms设置参数。
> 6. 调用UF_MODL_boolean_udf布尔化UDF，如果失败则撤销操作，否则更新模型。
> 7. 定义ufusr函数作为主入口点，初始化NX Open，调用do_it函数，然后终止NX Open。
> 8. 定义ufusr_ask_unload函数，用于卸载用户库。
>
> 这段代码实现了在NX中导入UDF的功能，通过交互式选择UDF位置并设置参数，然后布尔化UDF并更新模型。
>
