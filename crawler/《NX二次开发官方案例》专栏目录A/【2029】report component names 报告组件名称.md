### 【2029】report component names 报告组件名称

#### 代码

```cpp
    /*HEAD REPORT_COMPONENT_NAMES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的描述，UF_print_syslog 是一个在 V18 版本中新增的函数。文档中指出，在回答问题时，只需要提供译文，不需要添加其他无关的废话。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
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
            part = UF_PART_ask_display_part();  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            name[MAX_ENTITY_NAME_SIZE+1],  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            if (!UF_CALL(UF_ASSEM_ask_component_data(comp, part_name, refset_name,  
                instance_name, origin, csys_matrix, transform)))  
            {  
                WRITE("UF_ASSEM_ask_component_data:  ");  
                WRITE_S(part_name);  
                WRITE("UF_ASSEM_ask_component_data:  ");  
                WRITE_S(instance_name);  
            }  
            if (!UF_OBJ_ask_name(comp, name))  
            {  
                WRITE("UF_OBJ_ask_name: ");  
                WRITE_S(name);  
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

> 这段代码是NX Open API的二次开发示例代码，主要功能是遍历当前显示的零件中的所有组件，并获取每个组件的相关数据，例如组件名称、实例名称、参考集名称等，并将这些信息输出到列表窗口。
>
> 主要流程包括：
>
> 1. 定义错误报告函数report_error，用于打印出函数调用失败时的错误信息。
> 2. 定义写入列表窗口的宏WRITE和WRITE_S。
> 3. 定义获取下一个组件的函数ask_next_component。
> 4. 定义主体函数do_it，遍历零件中的所有组件，获取组件数据并输出。
> 5. 定义ufusr函数作为程序的入口，初始化并调用do_it函数，然后终止。
> 6. 定义卸载函数ufusr_ask_unload。
>
> 总体来说，这段代码通过遍历零件中的所有组件，获取每个组件的相关数据，并将其输出到列表窗口，展示了NX Open API在获取组件数据方面的基本用法。
>
