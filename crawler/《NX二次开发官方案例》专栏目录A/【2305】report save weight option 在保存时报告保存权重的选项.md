### 【2305】report save weight option 在保存时报告保存权重的选项

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdbool.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_attr.h>  
    #include <uf_weight.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN + 1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error(char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static void ask_component_partname(tag_t component, char *partname)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE + 1];  
        /* This returns the component part's fullpath based on the current load  
        options whether it is loaded or not.  
        */  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
    }  
    #define INDENT(X)   { int jj; for (jj = 0; jj < X; jj++) ECHO(" "); }  
    static void report_component_part_names(tag_t component, int level,  
        logical nxman)  
    {  
        bool   
            partSaveOption = false;  
        int  
            ii,  
            n;  
        tag_t  
            *children;  
        const tag_t  
            *partTag;  
        char  
            partname[MAX_FSPEC_SIZE + 1],  
            child_pn[MAX_FSPEC_SIZE + 1];  
        ask_component_partname(component, partname);  
        UF_CALL(UF_PART_name_for_display(partname, child_pn));  
        INDENT(level);  
        // check update weight on save  
        partTag = UF_PART_ask_part_tag(partname);  
        UF_CALL(UF_WEIGHT_ask_part_save_option(partTag, &partSaveOption));  
        ECHO("Partname: %s Part Tag: %d Update Weight on save? %s\n", partname, partTag, partSaveOption ? "True" : "False");  
        n = UF_ASSEM_ask_all_part_occ_children(component, &children);  
        for (ii = 0; ii < n; ii++)  
            report_component_part_names(children[ii], level + 1, nxman);  
        if (n > 0) UF_free(children);  
    }  
    static void do_it(void)  
    {  
        logical  
            nxman;  
        tag_t  
            root = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_UI_open_listing_window();  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if ((root = UF_ASSEM_ask_root_part_occ(part)) != NULL_TAG)  
            report_component_part_names(root, 0, nxman);  
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

> 这段代码是一个NX二次开发程序，主要实现了以下功能：
>
> 1. 定义了一个ECHO函数，用于在NX的Listing窗口和系统日志中输出信息。
> 2. 定义了一个报告错误函数report_error，用于在出现错误时输出错误码、错误信息以及调用失败的函数。
> 3. 定义了一个ask_component_partname函数，用于获取组件的完整路径、引用集名称、实例名称等信息。
> 4. 定义了一个递归函数report_component_part_names，用于遍历组件树，并输出每个组件的名称、Tag、是否在保存时更新重量等信息。
> 5. 定义了do_it函数，用于获取当前显示部件的根组件，然后递归输出组件树中的所有组件信息。
> 6. 定义了ufusr函数，作为NX二次开发的入口函数，用于初始化NX、调用do_it函数执行主要逻辑，然后终止NX。
> 7. 定义了ufusr_ask_unload函数，用于设置卸载该程序的选项。
>
> 总体来说，这段代码主要实现了遍历当前显示部件的组件树，并输出每个组件的相关信息，以了解组件树的结构和组件的详细信息。
>
