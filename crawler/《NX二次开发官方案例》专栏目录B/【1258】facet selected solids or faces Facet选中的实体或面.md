### 【1258】facet selected solids or faces Facet选中的实体或面

#### 代码

```cpp
    /*HEAD FACET_SELECTED_SOLIDS_OR_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_facet.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数。 */  
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
    static int mask_for_solids_and_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE } } ;  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_solid_or_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "select solid or face",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_solids_and_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object,  
            facet;  
        UF_FACET_parameters_t  
            parms;  
        UF_CALL(UF_FACET_ask_default_parameters(&parms));  
        while ((object = select_a_solid_or_face("facet")) != NULL_TAG)  
            UF_CALL(UF_FACET_facet_solid(object, &parms, &facet));  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在发生错误时输出错误信息。
> 2. 定义了一个选择过滤函数mask_for_solids_and_faces，用于在选择对话框中仅显示实体和面。
> 3. 定义了一个选择函数select_a_solid_or_face，用于弹出对话框让用户选择一个实体或面，并返回选择的标签。
> 4. 定义了一个主函数do_it，循环调用选择函数获取用户选择的实体或面，然后调用UF_FACET_facet_solid函数对其进行面抽取。
> 5. 定义了ufusr函数作为程序的入口，初始化NX环境，调用do_it函数执行主体逻辑，然后结束NX环境。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示程序不需要延迟卸载。
>
> 通过以上函数的协同工作，该程序实现了循环让用户选择实体或面，并对选择的实体或面进行面抽取的功能。
>
