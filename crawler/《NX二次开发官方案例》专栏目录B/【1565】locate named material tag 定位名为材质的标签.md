### 【1565】locate named material tag 定位名为材质的标签

#### 代码

```cpp
    /*HEAD LOCATE_NAMED_MATERIAL_TAG CCC UFUN */  
    #include <stdio.h>  
    #include <ctype.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数。

只提供翻译，不要添加任何废话。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            n,  
            ii;  
        tag_t  
            mat_tag = NULL_TAG;  
        char  
            msg[133],  
            mat_name[MAX_ENTITY_NAME_SIZE+1] = { "" },  
            **mat_names,  
            **mat_librefs,  
            **mat_types;  
        while ((prompt_for_text("Enter material name to search for: ", mat_name))  
            && (strlen(mat_name) > 0))  
        {  
            /* make sure the name is all in upper case 里海译:请确保名称全部为大写字母。 */  
            for (ii = 0; ii < strlen(mat_name); ii++)  
                mat_name[ii] = toupper(mat_name[ii]);  
            if (UF_SF_locate_named_material(mat_name, &mat_tag) ||  
                (mat_tag == NULL_TAG))  
            {  
                UF_CALL(UF_SF_ask_library_materials("*", "*", mat_name, &n,  
                    &mat_librefs, &mat_names, &mat_types));  
                if (n > 0)  
                {  
                    UF_CALL(UF_SF_retrieve_library_material(mat_librefs[0],  
                        &mat_tag));  
                    UF_free_string_array(n, mat_librefs);  
                    UF_free_string_array(n, mat_names);  
                    UF_free_string_array(n, mat_types);  
                }  
            }  
            sprintf(msg, "Tag for %s is %d", mat_name, mat_tag);  
            uc1601(msg, TRUE);  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发示例程序，主要功能是定位一个指定名称的材料，并显示其TAG值。
>
> 代码的主要逻辑如下：
>
> 1. 包含必要的头文件，定义了UF_CALL宏用于错误报告。
> 2. 定义了report_error函数，用于打印错误信息。
> 3. 定义了prompt_for_text函数，用于获取用户输入的材料名称。
> 4. 定义了do_it函数，是程序的主要逻辑：循环获取用户输入的材料名称将名称转为大写调用UF_SF_locate_named_material函数定位材料如果定位失败，调用UF_SF_ask_library_materials询问库材料如果找到材料，调用UF_SF_retrieve_library_material获取其TAG值打印材料名称和TAG值
> 5. 循环获取用户输入的材料名称
> 6. 将名称转为大写
> 7. 调用UF_SF_locate_named_material函数定位材料
> 8. 如果定位失败，调用UF_SF_ask_library_materials询问库材料
> 9. 如果找到材料，调用UF_SF_retrieve_library_material获取其TAG值
> 10. 打印材料名称和TAG值
> 11. 定义了ufusr函数，是程序的入口点：调用UF_initialize初始化调用do_it执行主要逻辑调用UF_terminate终止
> 12. 调用UF_initialize初始化
> 13. 调用do_it执行主要逻辑
> 14. 调用UF_terminate终止
> 15. 定义了ufusr_ask_unload函数，用于询问立即卸载
>
> 该程序实现了在NX中根据材料名称定位材料，并显示其TAG值的功能，通过循环获取用户输入，实现了交互式操作。
>
