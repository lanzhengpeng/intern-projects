### 【1009】delete all mating conditions 删除所有配合条件

#### 代码

```cpp
    /* This operates on components loaded in the current assembly 里海译:翻译为：这适用于当前程序集中加载的组件。 */  
    #include <stdlib.h>  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_defs.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_cfi.h>  
    #include <uf_retiring.h>  
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
    static void convert_to_shortname(char *partname, char *shortname)  
    {  
        logical  
            nxman;  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(partname, shortname));  
        else  
            UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void delete_all_mating_conditions_of_all_component(void)  
    {  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_spec[MAX_FSPEC_SIZE+1];  
        tag_t  
            comp = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        int  
            no_un = 0,  
            jj = 0,  
            ii = 0;  
        logical  
            removed;  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            ask_component_partname(comp, part_spec);  
            if (UF_PART_is_loaded(part_spec))  
            {  
                convert_to_shortname(part_spec, instance_name);  
                UF_CALL(UF_ASSEM_delete_mc(comp, &removed));  
                if (removed)  
                {  
                    ECHO("Removed mating for component: %s\n", instance_name);  
                    ii++;  
                }  
                else  
                {  
                    ECHO("NO mating to remove for component: %s\n", instance_name);  
                    jj++;  
                }  
            }  
            else  
                no_un++;  
        }  
        if(ii > 0) UF_CALL(UF_MODL_update());  
        ECHO("\n%d Components had mating removed\n", ii);  
        ECHO("\n%d Components had NO mating to remove\n", jj);  
        ECHO("\n%d components were not loaded\n", no_un);  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            delete_all_mating_conditions_of_all_component();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是删除当前程序集中所有组件的配合关系。
>
> 关键功能点包括：
>
> 1. 遍历当前程序集中的所有组件，使用ask_next_component函数获取下一个组件，直到没有更多组件为止。
> 2. 对每个组件，使用ask_component_partname获取组件的完整路径名，并使用convert_to_shortname将路径名转换为短名称。
> 3. 如果组件已加载，使用UF_ASSEM_delete_mc删除组件的配合关系，并使用UF_MODL_update更新模型。
> 4. 输出删除配合关系的组件数量、没有配合关系的组件数量以及未加载的组件数量。
> 5. 在UF_initialize和UF_terminate之间调用删除配合关系的函数。
> 6. 提供一个卸载函数ufusr_ask_unload。
> 7. 使用宏UF_CALL封装UF函数调用，以便于错误处理。
> 8. 使用ECHO宏在列表窗口和日志输出错误和提示信息。
>
> 总的来说，这段代码实现了删除当前程序集中所有组件的配合关系的功能，具有一定的错误处理和提示信息输出。
>
