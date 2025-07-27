### 【1758】print ANT to info window print ANT to info window

#### 代码

```cpp
    /*HEAD PRINT_ANT_TO_INFO_WINDOW CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是在 V18 版本中新增的。翻译内容如下：

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
    static tag_t ask_current_cset(void)  
    {  
        tag_t  
            cset = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, "CurrentComponents",  
            UF_component_set_type, FALSE, &cset));  
        return cset;  
    }  
    static logical is_blanked(tag_t object)  
    {  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
        return (logical) disp_props.blank_status;  
    }  
    static logical is_component_active(tag_t parent_occ, tag_t component)  
    {  
        logical  
            is_member;  
        tag_t  
            current_cset = ask_current_cset(),  
            member = NULL_TAG;  
        if (is_blanked(component)) return FALSE;  
        UF_CALL(UF_ASSEM_is_member_of_cset(current_cset, component, &is_member));  
        if (!is_member) return FALSE;  
        while (!UF_CALL(UF_ASSEM_cycle_objs_in_comp(parent_occ, &member)) &&  
            (member != NULL_TAG)) if (member == component) return TRUE;  
        return FALSE;  
    }  
    static void list_assembly_tree(tag_t component, int indent, char *fmt)  
    {  
        int  
            ii,  
            n_children;  
        tag_t  
            *child_components;  
        char  
            *check_color[2] = { "gray", "red " },  
            comp_name[30+1],  
            msg[133],  
            push[133];  
        n_children = UF_ASSEM_ask_part_occ_children(component, &child_components);  
        if (n_children > 0)  
        {  
            fmt[indent * 3] = '|';  
            for (ii = 0; ii < n_children; ii++)  
            {  
                ask_component_shortname(child_components[ii], comp_name);  
                strncpy(push, fmt, (indent * 3));  
                push[indent * 3] = '\0';  
                sprintf(msg, "%s|_[%s] %s\n", push, check_color[  
                    is_component_active(component,  
                    child_components[ii])], comp_name);  
                strcat(push, "|\n");  
                UF_CALL(UF_UI_write_listing_window(push));  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (ii == (n_children - 1))  
                    fmt[indent * 3] = ' ';  
                list_assembly_tree(child_components[ii], indent + 1, fmt);  
            }  
            UF_free(child_components);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            root_part_occ = UF_ASSEM_ask_root_part_occ(part);  
        char  
            fmt[MAX_LINE_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1];  
        memset(fmt, ' ', MAX_LINE_SIZE);  
        UF_PART_ask_part_name(part, part_name);  
        UF_CALL(uc4574(part_name, 2, part_name));  
        strcat(part_name, "\n");  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_UI_write_listing_window(part_name));  
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

> 这段代码是一个NX Open的UF程序，其主要功能是打印当前显示部件的装配树结构到信息窗口。具体来说，代码的主要流程如下：
>
> 1. 初始化：调用UF_initialize进行初始化，并定义了一个report_error函数用于报告UF函数调用错误。
> 2. 获取组件数据：定义了ask_component_shortname函数，用于获取组件的简称。
> 3. 获取当前组件集：定义了ask_current_cset函数，用于获取当前的组件集。
> 4. 判断组件是否被隐藏：定义了is_blanked函数，用于判断组件是否被隐藏。
> 5. 判断组件是否激活：定义了is_component_active函数，用于判断组件是否激活。
> 6. 打印装配树结构：定义了list_assembly_tree函数，递归打印装配树结构，并使用不同的颜色表示组件是否激活。
> 7. 执行主体：定义了do_it函数，获取当前显示部件的根组件，并调用list_assembly_tree打印装配树。
> 8. UFusr入口函数：定义了ufusr函数作为UF程序的入口函数，调用do_it执行主体功能，并最终调用UF_terminate终止。
> 9. 卸载函数：定义了ufusr_ask_unload函数，返回立即卸载。
>
> 总体来说，这段代码实现了从NX当前显示的装配模型中获取装配树结构，并以递归的方式打印到信息窗口中，同时用不同颜色标识激活和未激活的组件，实现了对装配结构的可视化展示。
>
