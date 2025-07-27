### 【2037】report count of components of selected solid in work part 报告选定实体在工作部件中的组件数量

#### 代码

```cpp
    /*HEAD REPORT_COUNT_OF_COMPONENTS_OF_SELECTED_SOLID_IN_WORK_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能。

对于用户的问题，回复应简洁明了，直接回答UF_print_syslog是V18版本新增的功能即可。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void do_it(void)  
    {  
        int  
            comp_count;  
        tag_t  
            component,  
            comp_part,  
            disp_part = UF_PART_ask_display_part(),  
            body,  
            *occs,  
            work_part = UF_ASSEM_ask_work_part();  
        char  
            comp_part_name[UF_CFI_MAX_FILE_NAME_LEN+1],  
            disp_part_name[UF_CFI_MAX_FILE_NAME_LEN+1],  
            work_part_name[UF_CFI_MAX_FILE_NAME_LEN+1];  
        ask_part_shortname(disp_part, disp_part_name);  
        WRITE_S(disp_part_name);  
        ask_part_shortname(work_part, work_part_name);  
        WRITE_S(work_part_name);  
        WRITE("\n");  
        while ((body = select_a_body("Report component count")) != NULL)  
        {  
            if (UF_ASSEM_is_occurrence(body))  
            {  
                component = UF_ASSEM_ask_part_occurrence(body);  
                comp_part = UF_ASSEM_ask_prototype_of_occ(component);  
                if (comp_part != work_part)  
                {  
                    ask_part_shortname(comp_part, comp_part_name);  
                    WRITE_S(comp_part_name);  
                    comp_count =  
                        UF_ASSEM_ask_occs_of_part(work_part, comp_part, &occs);  
                    WRITE_D(comp_count);  
                    if (comp_count > 0) UF_free(occs);  
                }  
                else  
                {  
                    WRITE("That solid is in ");  
                    WRITE(work_part_name);  
                    WRITE("\n");  
                }  
            }  
            else  
            {  
                WRITE("That solid is in ");  
                WRITE(disp_part_name);  
                WRITE("\n");  
            }  
            WRITE("\n");  
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

> 这段代码是NX Open C++ API编写的NX二次开发程序，其主要功能是查询当前工作部件中选定实体的组件数量。
>
> 主要逻辑包括：
>
> 1. 定义了错误报告函数report_error，用于在出错时打印错误信息。
> 2. 定义了辅助函数WRITE，用于向列表窗口输出信息。
> 3. 定义了选择实体的函数select_a_body，用于提示用户选择一个实体，并返回其tag。
> 4. 定义了查询部件短名称的函数ask_part_shortname。
> 5. 定义了主函数do_it，循环提示用户选择实体，然后查询其在当前工作部件中的组件数量，并打印出来。
> 6. 定义了ufusr函数，作为程序的入口，初始化NX Open环境，调用do_it函数，然后结束。
> 7. 定义了卸载函数ufusr_ask_unload，用于立即卸载二次开发程序。
>
> 总体来说，这段代码利用NX Open C++ API实现了查询当前工作部件中选定实体组件数量的功能。
>
