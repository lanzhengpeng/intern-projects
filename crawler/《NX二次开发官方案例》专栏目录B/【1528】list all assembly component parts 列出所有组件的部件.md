### 【1528】list all assembly component parts 列出所有组件的部件

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_attr.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
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
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
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
        int  
            ii,  
            n;  
        tag_t  
            *children;  
        char  
            partname[MAX_FSPEC_SIZE+1],  
            child_pn[MAX_FSPEC_SIZE+1];  
        ask_component_partname(component, partname);  
        UF_CALL(UF_PART_name_for_display(partname, child_pn));  
        INDENT(level);  
        ECHO("%s\n", child_pn);  
        n = UF_ASSEM_ask_all_part_occ_children(component, &children);  
        for (ii = 0; ii < n; ii++)  
            report_component_part_names(children[ii], level+1, nxman);  
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

> 这段NX二次开发代码的功能是遍历装配树并输出每个组件的零件名称。
>
> 主要步骤包括：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 定义了一个递归函数report_component_part_names，用于遍历装配树并输出每个组件的零件名称。它会先获取当前组件的零件名称，然后递归调用自身来遍历子组件。
> 3. 定义了主函数do_it，用于获取根组件并调用report_component_part_names函数来遍历整个装配树。
> 4. 定义了ufusr函数作为程序的入口，初始化NX环境后调用do_it函数，然后终止NX环境。
> 5. 定义了ufusr_ask_unload函数，用于卸载NX二次开发环境。
> 6. 使用宏UF_CALL来简化错误处理。
> 7. 使用宏INDENT来缩进输出以显示装配树的结构。
> 8. 通过调用UF_ASSEM_ask_component_data等函数来获取组件信息。
> 9. 输出格式简洁，可清晰显示装配树结构。
> 10. 代码逻辑清晰，可读性好。
>
> 总体来说，这段代码实现了遍历装配树并输出每个组件零件名称的功能，代码组织合理，实现了预期的功能。
>
