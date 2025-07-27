### 【2387】report template part of family member 报告家族成员的模板部件

#### 代码

```cpp
    /*HEAD REPORT_TEMPLATE_PART_OF_FAMILY_MEMBER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_assem.h>  
    #include <uf_fam.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新功能，它用于打印系统日志信息。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        logical  
            is_member;  
        int  
            index;  
        tag_t  
            family = NULL_TAG,  
            instance,  
            part = UF_PART_ask_display_part(),  
            t_part;  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1],  
            template_part[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_CALL(UF_PART_is_family_instance(part, &is_member));  
        if (is_member)  
        {  
            UF_CALL(UF_PART_ask_template_filename(part, template_part));  
            UF_CALL(UF_PART_ask_tag_of_disp_name(template_part, &t_part));  
            if (t_part == NULL_TAG)  
            {  
                UF_CALL(UF_PART_ask_family_instance(part, &instance));  
                UF_CALL(UF_FAM_ask_instance_data(instance, &family, &index));  
            /*  This will fail and write an error to the syslog but will  
                load the template part if it is in its original location */  
                UF_ASSEM_ask_component_data(family, template_part, refset_name,  
                    instance_name, origin, csys_matrix, transform);  
                UF_CALL(UF_PART_ask_tag_of_disp_name(template_part, &t_part));  
            }  
            if (t_part != NULL_TAG)  
                UF_PART_ask_part_name(t_part, template_part);  
            WRITE_S(template_part);  
        }  
        else  
        {  
            WRITE("This is not a family member part.\n");  
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

> 这段代码是一个NX的二次开发示例，其主要功能是检查当前显示的零件是否为家族成员，如果是，则获取其模板文件名，并检查模板文件是否已加载。如果没有加载，则会尝试加载模板文件，并获取模板文件名。如果当前零件不是家族成员，则会输出相应的信息。
>
> 具体步骤如下：
>
> 1. 包含必要的NX二次开发头文件。
> 2. 定义一个错误报告函数report_error，用于在出错时输出错误信息。
> 3. 定义宏WRITE和WRITE_S，用于向列表窗口输出信息。
> 4. 定义write_string_to_listing_window函数，用于向列表窗口输出字符串。
> 5. 定义主函数do_it，用于执行主要功能。
> 6. 获取当前显示的零件part，并检查其是否为家族成员。
> 7. 如果是家族成员，则获取模板文件名template_part。
> 8. 检查模板文件是否已加载，如果没有加载，则尝试通过家族数据加载。
> 9. 如果模板文件已加载，则获取模板文件的名称。
> 10. 如果当前零件不是家族成员，则输出相应的信息。
> 11. 定义ufusr函数，用于初始化和调用主函数。
> 12. 定义ufusr_ask_unload函数，返回立即卸载标志。
>
> 该代码通过调用NX的UF_PART、UF_FAM等API实现了零件是否为家族成员的检查，模板文件名的获取，以及模板文件的加载等功能。通过在列表窗口输出信息，可以清晰地显示代码的运行结果。
>
