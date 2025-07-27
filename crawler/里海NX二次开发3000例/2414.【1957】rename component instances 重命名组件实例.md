### 【1957】rename component instances 重命名组件实例

#### 代码

```cpp
    /*  
    Workaround for a pre-NX75 issue where the names   
    in the mating ui didn't match the component name   
    column in ANT after substituting components.  
    The code simply compares the comp object name   
    with the instance name and renames the instance, if necessary.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITENZ(X) if (X != 0) ECHO("%s = %d\n", #X, X);  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static void do_it(void)  
    {  
        tag_t  
            comp = NULL_TAG,  
            part_occ = NULL_TAG,  
            instance = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            obj_name[MAX_ENTITY_NAME_SIZE+1],  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        logical  
            is_occ = false;  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            ECHO("\n");  
            WRITE_D(comp);  
            if (!UF_CALL(UF_ASSEM_ask_component_data(comp, part_name, refset_name,  
                instance_name, origin, csys_matrix, transform)))  
            {  
                WRITE_S(part_name);  
                WRITE_S(instance_name);  
            }  
            if (!UF_OBJ_ask_name(comp, obj_name))  
            {  
                WRITE_S(obj_name);  
            }  
            if( (UF_ASSEM_is_occurrence(comp)) && (strcmp(obj_name, instance_name) != 0) )  
            {  
                instance = UF_ASSEM_ask_inst_of_part_occ( comp );  
                WRITE_D(instance);  
                ECHO(" -> renaming instance %s to %s\n", instance_name, obj_name);  
                UF_CALL( UF_ASSEM_rename_instance (instance, obj_name) );  
            }  
        }  
    }  
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

> 这段代码是NX的二次开发代码，主要用于解决在NX75之前版本中，配对UI中的名称与ANT中组件名称列在替换组件后不匹配的问题。主要实现的功能包括：
>
> 1. 组件名称比较和重命名：循环遍历当前显示部件中的所有组件，获取每个组件的名称和实例名称，如果两者不同，则重命名实例名称，使其与组件名称一致。
> 2. 获取组件信息：使用UF_ASSEM_ask_component_data获取组件的名称、引用集名称、实例名称等信息，并使用UF_OBJ_ask_name获取组件对象名称。
> 3. 重命名实例：如果组件是实例，并且对象名称与实例名称不同，则使用UF_ASSEM_rename_instance进行实例的重命名。
> 4. 错误处理和日志输出：使用宏UF_CALL进行函数调用，并在出错时记录错误信息。同时，使用宏WRITE和WRITE_S输出信息到日志窗口。
> 5. 初始化和终止：在ufusr函数中初始化和终止NX Open，并在do_it函数中执行主要功能。
> 6. 卸载函数：提供了卸载函数ufusr_ask_unload。
>
> 总体来说，这段代码通过比较组件名称和实例名称，实现了实例的重命名，以确保配对UI中的名称与ANT中的组件名称一致，解决了预NX75版本的一个问题。
>
