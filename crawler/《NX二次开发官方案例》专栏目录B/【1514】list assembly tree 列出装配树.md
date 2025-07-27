### 【1514】list assembly tree 列出装配树

#### 代码

```cpp
    /*HEAD LIST_ASSEMBLY_TREE CCC UFUN */  
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
    #include <uf_cfi.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static void ask_component_shortname(tag_t component, char *shortname)  
    {  
        logical  
            nxman;  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            partname[UF_CFI_MAX_PATH_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(partname, shortname));  
        else  
            UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static logical is_object_a_component_member(tag_t component, tag_t object)  
    {  
        tag_t  
            obj = NULL_TAG;  
        while (!UF_CALL(UF_ASSEM_cycle_objs_in_comp(component, &obj)) &&  
            (obj != NULL_TAG)) if (obj == object) return TRUE;  
        return FALSE;  
    }  
    static logical is_object_blanked(tag_t object)  
    {  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
        return (logical) disp_props.blank_status;  
    }  
    static void list_assembly_tree(tag_t component, int indent, char *fmt)  
    {  
        int  
            ii,  
            n_children;  
        tag_t  
            *child_components;  
        char  
            *check_color[2] = { "red", "grey" },  
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
                sprintf(msg, "%s|__%s (%s check)\n", push, comp_name,  
                    check_color[!is_object_a_component_member(component,  
                        child_components[ii]) ||  
                        is_object_blanked(child_components[ii])]);  
                ECHO(msg);  
                if (ii == (n_children - 1)) fmt[indent * 3] = ' ';  
                list_assembly_tree(child_components[ii], indent + 1, fmt);  
            }  
            UF_free(child_components);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            root_part_occ;  
        char  
            fmt[MAX_LINE_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1];  
        memset(fmt, ' ', MAX_LINE_SIZE);  
        if (part != NULL_TAG)  
        {  
            root_part_occ = UF_ASSEM_ask_root_part_occ(part);  
            UF_PART_ask_part_name(part, part_name);  
            UF_CALL(uc4574(part_name, 2, part_name));  
            strcat(part_name, "\n");  
            ECHO(part_name);  
            if (root_part_occ != NULL_TAG)  
                list_assembly_tree(root_part_occ, 0, fmt);  
        }  
        else  
            ECHO("Program requires an active displayed part");  
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
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                ECHO("  ");  
                ECHO(status->file_names[ii]);  
                ECHO(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO(msg);  
                ECHO("\n");  
            }  
            ECHO("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    /*  To run in external native mode NX:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external NX Manager mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是用于在NX中遍历装配树的二次开发代码。其主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于在发生错误时打印错误信息。
> 2. 定义了获取组件简称的函数ask_component_shortname。
> 3. 定义了检查对象是否为组件成员的函数is_object_a_component_member。
> 4. 定义了检查对象是否被隐藏的函数is_object_blanked。
> 5. 定义了递归遍历装配树的函数list_assembly_tree，用于输出组件的树形结构。
> 6. 定义了主函数do_it，用于获取当前显示的部件，然后从根组件开始递归调用list_assembly_tree来遍历装配树。
> 7. 定义了ufusr函数，作为NX的入口函数，用于初始化、调用do_it、终止。
> 8. 定义了主函数main，用于处理命令行参数，打开多个部件，调用do_it，最后关闭所有部件。
> 9. 定义了报告加载状态的函数report_load_status。
>
> 总的来说，这段代码的功能是遍历NX装配树，输出组件的树形结构。它支持命令行参数，可以打开多个部件进行遍历。
>
