### 【2077】report drawing views of drafting reference components 报告绘图参考组件的绘图视图

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_draw.h>  
    #include <uf_modl.h>  
    #include <uf_so.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
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
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_all_drafting_reference_components(tag_t part, tag_t **children)  
    {  
        logical  
            is_ref;  
        int  
            ii,  
            n;  
        tag_t  
            *comps;  
        uf_list_p_t  
            child_list;  
        UF_CALL(UF_MODL_create_list(&child_list));  
        n = UF_ASSEM_ask_all_part_occ_children(  
            UF_ASSEM_ask_root_part_occ(part), &comps);  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_DRAW_is_drafting_component(comps[ii], &is_ref));  
            if (is_ref) UF_CALL(UF_MODL_put_list_item(child_list, comps[ii]));  
        }  
        return (make_an_array(&child_list, children));  
    }  
    static void ask_component_partname(tag_t component, char *partname)  
    {  
        tag_t  
            instance;  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        if ((instance = UF_ASSEM_ask_inst_of_part_occ(component)) != NULL_TAG)  
        {  
            UF_CALL(UF_ASSEM_ask_part_name_of_child(instance, partname));  
        }  
        else  
        {  
            UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
                instance_name, origin, csys_matrix, transform));  
        }  
    }  
    static void ask_component_shortname(tag_t component, char *shortname)  
    {  
        logical  
            nxman;  
        char  
            partname[UF_CFI_MAX_PATH_NAME_SIZE];  
        ask_component_partname(component, partname);  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(partname, shortname));  
        else  
            UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            jj,  
            n,  
            n_kids,  
            subtype,  
            type;  
        tag_t  
            *kids,  
            part = UF_PART_ask_display_part(),  
            *ref_comps;  
        char  
            comp_name[MAX_FSPEC_SIZE+1],  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        n = ask_all_drafting_reference_components(part, &ref_comps);  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_SO_ask_children(ref_comps[ii], UF_SO_ASK_ALL_CHILDREN,  
                &n_kids, &kids));  
            for (jj = 0; jj < n_kids; jj++)  
            {  
                if (UF_OBJ_ask_status(kids[jj]) != UF_OBJ_ALIVE) continue;  
                UF_CALL(UF_OBJ_ask_type_and_subtype(kids[jj], &type, &subtype));  
                if (type == UF_view_type)  
                {  
                    UF_CALL(UF_OBJ_ask_name(kids[jj], view_name));  
                    ask_component_shortname(ref_comps[ii], comp_name);  
                    ECHO("%s is in %s\n", comp_name, view_name);  
                }  
            }  
        }  
        if (n > 0) UF_free(ref_comps);  
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

> 这段代码是一个NX Open C++应用程序，用于查询装配体中的所有参考组件，并进一步查询这些参考组件中包含的所有视图。其主要功能包括：
>
> 1. 查询装配体的所有参考组件，并将它们存储在一个tag数组中。
> 2. 遍历每个参考组件，查询其包含的所有子项，并将这些子项存储在一个tag数组中。
> 3. 遍历子项数组，判断每个子项的类型。如果是视图类型，则获取该视图的名称和其所在的参考组件的名称。
> 4. 打印出每个视图和其所在的参考组件的名称，例如“component1 is in view1”。
> 5. 在应用程序结束时释放分配的内存。
>
> 代码中定义了一些辅助函数，例如allocate_memory、make_an_array、ask_all_drafting_reference_components等，用于实现主要功能。另外，代码还包含错误处理和内存管理机制。总的来说，这段代码实现了对NX装配体中所有参考组件及其包含视图的查询和打印功能。
>
