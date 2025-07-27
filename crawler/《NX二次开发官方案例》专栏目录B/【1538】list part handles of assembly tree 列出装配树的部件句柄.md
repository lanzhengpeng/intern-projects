### 【1538】list part handles of assembly tree 列出装配树的部件句柄

#### 代码

```cpp
    /*HEAD LIST_PART_HANDLES_OF_ASSEMBLY_TREE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是在V18版本中新增的函数。 */  
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
    static void ask_component_shortname(tag_t component, char *shortname)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            partname[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
        UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void list_assembly_tree(tag_t component, int indent, char *fmt)  
    {  
        unsigned int  
            sub_file_id,  
            version;  
        int  
            ii,  
            n_children;  
        tag_t  
            *child_components;  
        char  
            comp_name[30+1],  
            *file_data,  
            *handle,  
            msg[133],  
            push[133];  
        n_children = UF_ASSEM_ask_part_occ_children(component, &child_components);  
        if (n_children > 0)  
        {  
            fmt[indent * 3] = '|';  
            for (ii = 0; ii < n_children; ii++)  
            {  
                ask_component_shortname(child_components[ii], comp_name);  
                handle = UF_TAG_ask_handle_of_tag(child_components[ii]);  
                UF_CALL(UF_TAG_decompose_handle(handle, &file_data, &sub_file_id,  
                    &version));  
                strncpy(push, fmt, (indent * 3));  
                push[indent * 3] = '\0';  
                sprintf(msg, "%s|_%s = %s - %s - %d - %d\n", push, comp_name,  
                    handle, file_data, sub_file_id, version);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                UF_free(file_data);  
                UF_free(handle);  
                list_assembly_tree(child_components[ii], indent + 1, fmt);  
            }  
            UF_free(child_components);  
        }  
    }  
    static void do_it(void)  
    {  
        unsigned int  
            sub_file_id,  
            version;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            root_part_occ = UF_ASSEM_ask_root_part_occ(part);  
        char  
            fmt[MAX_LINE_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1],  
            *handle,  
            *file_data,  
            msg[MAX_LINE_SIZE+1];  
        memset(fmt, ' ', MAX_LINE_SIZE);  
        UF_PART_ask_part_name(part, part_name);  
        UF_CALL(uc4574(part_name, 2, part_name));  
        handle = UF_TAG_ask_handle_of_tag(root_part_occ);  
        UF_CALL(UF_TAG_decompose_handle(handle, &file_data, &sub_file_id,  
            &version));  
        sprintf(msg, "%s = %s - %s - %d - %d\n", part_name, handle, file_data,  
            sub_file_id, version);  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_UI_write_listing_window(msg));  
        UF_free(file_data);  
        UF_free(handle);  
        if (root_part_occ != NULL_TAG) list_assembly_tree(root_part_occ, 0, fmt);  
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

> 根据代码内容，这是一段基于NX的二次开发代码，其主要功能是列出装配树中的所有组件的句柄信息。
>
> 代码的主要逻辑包括以下几个步骤：
>
> 1. 定义了一个错误报告函数report_error，用于在调用UF函数出错时报告错误信息。
> 2. 定义了一个获取组件短名称的函数ask_component_shortname，通过调用UF函数获取组件的相关信息，并提取短名称。
> 3. 定义了一个递归函数list_assembly_tree，用于遍历装配树，对每个组件获取其句柄信息并打印输出。
> 4. 定义了主函数do_it，首先获取当前显示的零件及其根组件，然后打印根组件的句柄信息，并递归调用list_assembly_tree函数打印所有子组件的句柄信息。
> 5. 定义了ufusr函数，作为NX二次开发的入口函数，在初始化后调用do_it函数，完成功能后终止。
> 6. 定义了ufusr_ask_unload函数，用于卸载NX二次开发环境。
>
> 总的来说，这段代码通过递归遍历装配树，获取每个组件的句柄信息，并格式化输出，实现了列出装配树组件句柄信息的功能。
>
