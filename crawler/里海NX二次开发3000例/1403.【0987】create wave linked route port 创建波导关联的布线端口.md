### 【0987】create wave linked route port 创建波导关联的布线端口

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_wave.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_so.h>  
    #include <uf_obj.h>  
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
    /* qq3123197280 */  
    static int mask_for_routing_ports(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_route_port_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_routing_port(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a port", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_routing_ports, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void create_wave_linked_route_port(tag_t port)  
    {  
        tag_t  
            feat,  
            part_occ,  
            proto,  
            xform;  
        if (UF_ASSEM_is_occurrence(port))  
            proto = UF_ASSEM_ask_prototype_of_occ(port);  
        else  
            proto = port;  
        part_occ = UF_ASSEM_ask_part_occurrence(port);  
        if (part_occ == NULL_TAG)  
            UF_CALL(UF_WAVE_create_linked_route_port(proto, NULL_TAG, port, &feat));  
        else  
        {  
            UF_CALL(UF_SO_create_xform_assy_ctxt(port, part_occ, NULL_TAG, &xform));  
            UF_CALL(UF_WAVE_create_linked_route_port(proto, xform, port, &feat));  
            UF_CALL(UF_OBJ_set_blank_status(port, UF_OBJ_BLANKED));  
        }  
        UF_CALL(UF_MODL_update());  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            port;  
        while ((port = select_a_routing_port("Wave link")) != NULL_TAG)  
        {  
            set_undo_mark("Wave link route port");  
            create_wave_linked_route_port(port);  
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

> 这段NX二次开发代码的主要功能是创建波导路线端口并链接到模型中的端口。
>
> 主要步骤包括：
>
> 1. 定义错误报告函数report_error，用于打印函数调用失败信息。
> 2. 定义选择函数select_a_routing_port，用于让用户选择一个波导路线端口。
> 3. 定义创建函数create_wave_linked_route_port，用于创建一个与用户选择的端口链接的波导路线端口。
> 4. 定义设置撤销标记函数set_undo_mark，用于设置一个撤销标记。
> 5. 定义主函数do_it，循环调用select_a_routing_port选择端口，然后调用create_wave_linked_route_port创建波导路线端口。
> 6. 定义ufusr入口函数，初始化NX环境，调用do_it，然后终止NX环境。
> 7. 定义卸载函数ufusr_ask_unload，用于卸载用户自定义程序。
>
> 整体上，这段代码实现了波导路线端口的自动创建和链接功能，通过用户交互选择端口，然后自动创建波导路线端口并链接到选择的端口上。
>
