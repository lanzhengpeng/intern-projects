### 【2197】report master model part name 报告主模型部件名称

#### 代码

```cpp
    /*HEAD REPORT_MASTER_MODEL_PART_NAME CCC UFUN */  
    /*Note: Because a master model part is not a "true"   
    assembly component, therefore UF_ASSEM_ask_component_data  
    cannot report a component or instance name. However it  
    can report the part name of the master model part.*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    #include <uf_object_types.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18版本中是新增的。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_component(tag_t scenario_part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(scenario_part, UF_component_type, &comp))  
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
            scenario_part = UF_PART_ask_display_part(),  
        comp = NULL_TAG;  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        while ((comp = ask_next_component(scenario_part, comp)) != NULL_TAG)  
        {  
            WRITE_D(comp);  
            UF_CALL(UF_ASSEM_ask_component_data(comp, part_name, refset_name,  
            instance_name, origin, csys_matrix, transform));  
            WRITE_S(part_name);  
        }   
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

> 这段NX二次开发代码的主要功能是从当前显示的组件中遍历所有组件实例，并输出每个组件实例的名称和所在组件的名称。以下是代码的详细说明：
>
> 1. 头文件和宏定义：代码包含了必要的NX Open头文件，并定义了错误报告宏和用于在列表窗口中输出信息的方法。
> 2. 组件遍历：ask_next_component函数用于遍历组件实例。它通过UF_OBJ_cycle_objs_in_part函数循环获取当前组件中的所有对象，并判断对象是否为组件实例。如果是，则返回该组件实例的tag。
> 3. 组件数据获取：do_it函数首先获取当前显示组件的tag，然后循环调用ask_next_component获取每个组件实例。对于每个组件实例，调用UF_ASSEM_ask_component_data函数获取其名称、所在组件的名称、位置等信息，并输出到列表窗口。
> 4. 用户子函数：ufusr是用户子函数的入口，初始化NX Open环境后调用do_it函数，最后终止NX Open环境。ufusr_ask_unload用于设置立即卸载二次开发程序。
> 5. 注意事项：代码中使用了NX 18及以上版本中的UF_print_syslog函数来输出信息，需要注意NX版本兼容性。
>
> 总体来说，这段代码实现了从当前显示组件中遍历所有组件实例，并输出每个实例的名称和所在组件的名称，对于了解NX组件结构非常有帮助。
>
