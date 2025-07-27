### 【1241】extract body 提取实体

#### 代码

```cpp
    /*HEAD EXTRACT_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_wave.h>  
    #include <uf_so.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是一个在V18版本中新增的函数。它用于在系统日志中打印消息。 */  
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
    /* qq3123197280 */  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t ask_part_occ_of_prom_occ(tag_t prom_object)  
    {  
        tag_t  
            owning_part,  
            part_occ = prom_object,  
            proto;  
        proto = UF_ASSEM_ask_prototype_of_occ(prom_object);  
        UF_CALL(UF_OBJ_ask_owning_part(proto, &owning_part));  
        while (!UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ))  
            && (UF_ASSEM_ask_prototype_of_occ(part_occ) != owning_part));  
        return part_occ;  
    }  
    static void create_wave_linked_body(tag_t body)  
    {  
        tag_t  
            feat,  
            part_occ,  
            proto,  
            xform;  
        if (UF_ASSEM_is_occurrence(body))  
            proto = UF_ASSEM_ask_prototype_of_occ(body);  
        else  
            proto = body;  
        if (UF_OBJ_is_object_a_promotion(proto))  
            part_occ = ask_part_occ_of_prom_occ(body);  
        else  
            part_occ = UF_ASSEM_ask_part_occurrence(body);  
        if (part_occ == NULL_TAG)  
            UF_CALL(UF_WAVE_create_linked_body(proto,NULL_TAG,body,FALSE,&feat));  
        else  
        {  
            UF_CALL(UF_SO_create_xform_assy_ctxt(body, part_occ, NULL_TAG, &xform));  
            UF_CALL(UF_WAVE_create_linked_body(proto, xform, body, FALSE, &feat));  
        }  
        UF_CALL(UF_MODL_update());  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body;  
        while ((body = select_a_body("Select body to link")) != NULL_TAG)  
            create_wave_linked_body(body);  
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

> 这段NX二次开发代码的主要功能是创建一个与NX模型中的体关联的波浪特征。以下是代码的详细解释：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在调用NX API函数失败时打印错误信息。
> 2. 选择体：定义了一个函数mask_for_bodies，用于设置选择模式，只选择实体类型。然后定义了一个函数select_a_body，用于通过对话框让用户选择一个体，并返回其tag。
> 3. 创建波浪特征：定义了一个函数create_wave_linked_body，用于为选中的体创建波浪特征。首先判断体是否是组合体，如果是，则找到其原型体。然后判断原型体是否是提升体，如果是，则找到对应的组合体。最后，根据原型体和组合体，调用UF_WAVE_create_linked_body函数创建波浪特征。
> 4. 主函数：定义了一个函数do_it，用于循环调用select_a_body函数选择体，并为每个选中的体调用create_wave_linked_body函数创建波浪特征。
> 5. 入口函数：定义了ufusr函数，用于在NX中启动该应用程序。首先调用UF_initialize进行初始化，然后调用do_it函数，最后调用UF_terminate进行清理。
> 6. 卸载函数：定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 综上所述，这段代码实现了在NX中为用户选择的实体创建波浪特征的功能，提供了用户友好的交互界面。
>
