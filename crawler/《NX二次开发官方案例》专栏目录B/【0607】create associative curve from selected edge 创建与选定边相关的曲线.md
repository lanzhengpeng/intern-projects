### 【0607】create associative curve from selected edge 创建与选定边相关的曲线

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_so.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
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
    /* 里海 */  
    static int mask_for_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t ask_object_xform(tag_t object)  
    {  
        tag_t  
            base_tag,  
            part_occ,  
            prom_part,  
            part_proto,  
            proto,  
            xform = NULL_TAG;  
        if (!UF_ASSEM_is_occurrence(object)) return NULL_TAG;  
        proto = UF_ASSEM_ask_prototype_of_occ(object);  
        if (UF_OBJ_is_object_a_promotion(proto))  
        {  
            base_tag = proto;  
            while (base_tag != NULL_TAG)  
            {  
                UF_CALL(UF_MODL_prom_map_object_down(proto, &base_tag));  
                if (base_tag != NULL_TAG) proto = base_tag;  
            }  
            UF_CALL(UF_OBJ_ask_owning_part(proto, &prom_part));  
            part_occ = UF_ASSEM_ask_part_occurrence(object);  
            part_proto = UF_ASSEM_ask_prototype_of_occ(part_occ);  
            while (prom_part != part_proto)  
            {  
                UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ));  
                part_proto = UF_ASSEM_ask_prototype_of_occ(part_occ);  
            }  
            UF_CALL(UF_SO_create_xform_assy_ctxt(object, part_occ, NULL_TAG,  
                &xform));  
        }  
        else  
            UF_CALL(UF_SO_create_xform_assy_ctxt(object,  
                UF_ASSEM_ask_part_occurrence(object), NULL_TAG, &xform));  
        return xform;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve,  
            edge = select_an_edge("Extract associative Curve from Edge"),  
            xform;  
        if (edge == NULL_TAG) return;  
        xform = ask_object_xform(edge);  
        UF_CALL(UF_SO_create_curve_extract(edge, UF_SO_update_within_modeling,  
            UF_ASSEM_ask_prototype_of_occ(edge), 0, UF_all_subtype, xform, &curve));  
        UF_CALL(UF_SO_set_visibility_option(curve, UF_SO_visible));  
    }  
    /* 里海 */  
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

> 根据代码内容，这是段NX的二次开发代码，其主要功能是从NX中的边提取关联曲线。
>
> 具体步骤如下：
>
> 1. 定义了一些辅助函数，如ECHO用于打印输出，UF_CALL用于调用NX函数并处理错误，mask_for_edges用于设置选择掩码为边，select_an_edge用于选择一个边。
> 2. do_it函数是主函数，首先调用select_an_edge选择一个边，然后调用ask_object_xform获取边的变换，最后调用UF_SO_create_curve_extract提取出关联曲线，并设置曲线可见。
> 3. ufusr函数是NX二次开发程序的入口函数，在这里初始化NX环境，调用do_it函数执行提取曲线功能，最后终止NX环境。
> 4. ufusr_ask_unload函数用于处理程序卸载，这里设置为立即卸载。
>
> 总体来说，这段代码实现了从NX中的边提取关联曲线的功能，包括选择边、获取变换、提取曲线等步骤，利用NX的二次开发接口实现。
>
