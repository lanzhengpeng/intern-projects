### 【1555】load assembly component part by leaf part name 加载组件叶部件名称的组件部分

#### 代码

```cpp
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        int  
            ii;  
        char  
            msg[133];  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO("  %s - %s\n", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_CALL(UF_PART_free_load_status(status));  
        }  
    }  
    static tag_t ask_next_component(tag_t part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp))  
            && (comp != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(comp, &type, &subtype));  
            if (subtype == UF_part_occurrence_subtype) return comp;  
        }  
        return comp;  
    }  
    static void load_assembly_component_part_by_leaf_part_name(char *leaf)  
    {  
        tag_t  
            component = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            full_path[MAX_FSPEC_SIZE+1],  
            instance_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1],  
            this_leaf[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            load_status;  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        while ((component = ask_next_component(part, component)) != NULL_TAG)  
        {  
        /* This returns the component part's fullpath based on the current load  
            options whether it is loaded or not.  */  
            UF_CALL(UF_ASSEM_ask_component_data(component, full_path, refset_name,  
                instance_name, origin, csys_matrix, transform));  
            uc4574(full_path, 2, this_leaf);  
            if (strcmp(leaf, this_leaf) == 0)  
            {  
                if (UF_PART_is_loaded(full_path) != 1)  
                {  
                   UF_CALL(UF_PART_open_quiet(full_path, &part, &load_status));  
                   report_load_status(&load_status);  
                   return;  
                }  
            }  
        }  
    }

```

#### 代码解析

> 这段代码是用于NX（Siemens NX）的二次开发，主要实现了以下功能：
>
> 1. report_load_status函数：用于报告加载部件的状态。它会遍历加载状态结构体中的所有部件，获取每个部件的加载失败信息，并打印出来。
> 2. ask_next_component函数：用于在指定的部件中循环查询下一个组件，直到返回NULL_TAG。在循环过程中，它会对每个组件进行类型和子类型查询，如果子类型是部件实例，则返回该组件。
> 3. load_assembly_component_part_by_leaf_part_name函数：用于根据叶部件的名称加载装配中的组件部件。它会遍历装配中的所有组件，获取每个组件的数据，并与目标叶部件名称进行比较。如果名称匹配且该部件尚未加载，则会调用UF_PART_open_quiet函数来加载该部件，并报告加载状态。
>
> 总的来说，这段代码的目的是为了在装配中根据指定的叶部件名称加载对应的组件部件。
>
