### 【1540】list promoted component parts 列出提升的组件部分

#### 代码

```cpp
    /*HEAD LIST_PROMOTED_COMPONENT_PARTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_object_types.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的功能。 */  
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
    static tag_t ask_next_promotion(tag_t part, tag_t obj)  
    {  
        int  
            subtype,  
            type;  
        while ((obj = UF_OBJ_cycle_all(part, obj)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(obj, &type, &subtype));  
            if ((type == UF_solid_type) && (subtype == UF_solid_body_subtype) &&  
                UF_OBJ_is_object_a_promotion(obj)) return obj;  
        }  
        return NULL_TAG;  
    }  
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            base_body,  
            component_part,  
            part = UF_PART_ask_display_part(),  
            prom_body = NULL_TAG;  
        char  
            component_part_name[UF_CFI_MAX_FILE_NAME_SIZE];  
        while ((prom_body = ask_next_promotion(part, prom_body)) != NULL_TAG)  
        {  
            WRITE_D(prom_body);  
            UF_CALL(UF_MODL_prom_map_object_down(prom_body, &base_body));  
            WRITE_D(base_body);  
            UF_CALL(UF_OBJ_ask_owning_part(base_body, &component_part));  
            WRITE_D(component_part);  
            ask_part_shortname(component_part, component_part_name);  
            WRITE_S(component_part_name);  
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

> 这段NX二次开发代码的主要功能是遍历当前显示的部件，找出所有被提升为组件的实体，并输出这些实体对应的组件信息。
>
> 具体步骤如下：
>
> 1. 定义一些宏和错误处理函数，用于输出错误信息和变量值。
> 2. 定义一个函数ask_next_promotion，用于遍历部件，找出下一个被提升为组件的实体。
> 3. 定义一个函数ask_part_shortname，用于获取部件的短名称。
> 4. 在do_it函数中，首先获取当前显示的部件，然后遍历部件，找到所有被提升为组件的实体。对于每个找到的实体，获取其基实体，然后获取基实体所属的组件。最后，输出组件的短名称。
> 5. 在ufusr函数中，首先初始化NX，然后调用do_it函数，最后终止NX。
> 6. 定义一个函数ufusr_ask_unload，用于卸载用户函数模块。
>
> 总的来说，这段代码通过遍历部件，获取被提升为组件的实体，并输出对应组件的信息，实现了对NX部件提升为组件信息的查询功能。
>
