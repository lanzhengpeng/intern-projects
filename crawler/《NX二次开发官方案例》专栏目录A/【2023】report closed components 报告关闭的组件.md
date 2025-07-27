### 【2023】report closed components 报告关闭的组件

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
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
    static int ask_all_components(tag_t part, tag_t **components)  
    {  
        tag_t  
            component = NULL_TAG;  
        uf_list_p_t  
            comp_list;  
        UF_CALL(UF_MODL_create_list(&comp_list));  
        while ((component = ask_next_component(part, component)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(comp_list, component));  
        return make_an_array(&comp_list, components);  
    }  
    static void ask_component_name(tag_t component_tag, char *instance_name)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            part_name[UF_CFI_MAX_PATH_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_component_data(component_tag, part_name,  
            refset_name, instance_name, origin, csys_matrix, transform));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *components,  
            proto,  
            part = UF_PART_ask_display_part();  
        char  
            instance_name[UF_CFI_MAX_PATH_NAME_SIZE];  
        n = ask_all_components(part, &components);  
        for (ii = 0; ii < n; ii++)  
        {  
            proto = UF_ASSEM_ask_prototype_of_occ(components[ii]);  
            if (proto == NULL_TAG)  
            {  
                ask_component_name(components[ii], instance_name);  
                ECHO("%s is closed\n", instance_name);  
            }  
        }  
        if (n > 0) UF_free(components);  
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

> 这段代码是用于NX二次开发，其主要功能是遍历当前显示部件中的所有组件，并打印出封闭组件的名称。下面是对代码的详细介绍：
>
> 1. 头文件包含：代码包含了NX Open API的UF头文件，如uf.h, uf_ui.h等，这些头文件定义了代码中使用的UF函数。
> 2. 宏定义：代码定义了两个宏：UF_CALL和report_error。其中，UF_CALL用于调用UF函数，并在调用失败时报告错误；report_error用于输出错误信息。
> 3. 函数定义：ECHO：用于输出信息到日志窗口。ask_next_component：用于遍历部件中的下一个组件。allocate_memory：用于分配内存。make_an_array：用于将UF列表转换为数组。ask_all_components：用于获取部件中的所有组件，并返回组件数组。ask_component_name：用于获取组件的名称。do_it：用于遍历组件并打印出封闭组件的名称。ufusr：程序入口，初始化NX，调用do_it函数，然后终止NX。
> 4. ECHO：用于输出信息到日志窗口。
> 5. ask_next_component：用于遍历部件中的下一个组件。
> 6. allocate_memory：用于分配内存。
> 7. make_an_array：用于将UF列表转换为数组。
> 8. ask_all_components：用于获取部件中的所有组件，并返回组件数组。
> 9. ask_component_name：用于获取组件的名称。
> 10. do_it：用于遍历组件并打印出封闭组件的名称。
> 11. ufusr：程序入口，初始化NX，调用do_it函数，然后终止NX。
> 12. 主要逻辑：在do_it函数中，先获取当前显示部件，然后调用ask_all_components获取所有组件。遍历组件数组，对于每个组件，先判断是否为封闭组件（原型组件为空），如果是，则获取其名称并打印。
> 13. 内存释放：代码中分配的内存会在使用后释放，避免内存泄露。
> 14. 错误处理：代码中大量使用了UF_CALL宏，在UF函数调用失败时会报告错误，有助于定位问题。
> 15. 注释：代码中有一些中文注释，有助于理解代码逻辑。
>
> 总的来说，这段代码实现了遍历部件组件并打印出封闭组件名称的功能，并包含错误处理、内存管理等方面的考量。
>
