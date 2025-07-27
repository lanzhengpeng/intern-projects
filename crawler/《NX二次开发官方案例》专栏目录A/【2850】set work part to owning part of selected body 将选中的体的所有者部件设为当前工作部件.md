### 【2850】set work part to owning part of selected body 将选中的体的所有者部件设为当前工作部件

#### 代码

```cpp
    /*HEAD SET_WORK_PART_TO_OWNING_PART_OF_SELECTED_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中新增的函数。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body,  
            owner,  
            proto;  
        body = select_a_body("Select a body:");  
        if( NULL_TAG == body ) return;  
        if(UF_ASSEM_is_occurrence(body))  
        {  
            proto = UF_ASSEM_ask_prototype_of_occ(body);  
        }  
        else  
        {  
            proto = body;  
        }  
        UF_CALL(UF_OBJ_ask_owning_part(proto, &owner));  
        UF_CALL(UF_ASSEM_set_work_part(owner));  
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

> 这段代码是NX二次开发代码，主要实现以下功能：
>
> 1. 定义错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义选择体函数select_a_body，用于提示用户选择一个体，并返回选择的体标签。
> 3. 定义主体函数do_it，用于获取用户选择的体，判断是实例还是原型，获取体的所有者部件，并设置当前工作部件为体的所有者部件。
> 4. 定义ufusr函数，用于初始化NX，调用do_it函数，然后终止NX。
> 5. 定义ufusr_ask_unload函数，用于在卸载用户函数时立即卸载。
>
> 整体上，该代码通过UI函数实现了交互式的选择体和设置工作部件功能，用户可以方便地选择一个体，然后将其所有者部件设置为当前工作部件。
>
